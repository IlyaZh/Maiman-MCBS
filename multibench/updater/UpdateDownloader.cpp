#include "UpdateDownloader.h"
#ifdef QT_DEBUG
#include <QDebug>
#endif
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>
#include <QString>
#include <QCryptographicHash>

/*!
 * \brief UpdateDownloader::DefaultTimeout
 */
const int UpdateDownloader::DefaultTimeout = 10000;

/*!
 * \brief UpdateDownloader::bytesToString
 * \param value
 * \return
 */
QString UpdateDownloader::bytesToString(qint64 value) {
    static const QString units = "KMGT";
    int idx = -1;
    while(value > 1024) {
        value /= 1024;
        ++idx;
    }

    if(idx == -1)
        return QString::number(value);
    else
        return QString::number(value) + units.at(idx);

}

/*!
 * \brief UpdateDownloader::UpdateDownloader
 * \param versionFileUrl
 * \param parent
 */
UpdateDownloader::UpdateDownloader(const QString& versionFileUrl, QObject* parent) :
    QObject(parent),
    m_versionFile(versionFileUrl),
//    m_dir(dir),
    m_size(0),
    m_queue(FileQueue::Description),
    m_state(State::None)
{

}

// TODO:: Добавиь где надо const к функциям
/*!
 * \brief UpdateDownloader::errorString
 * \return
 */
QString UpdateDownloader::errorString() {
    return m_errorString;
}

/*!
 * \brief UpdateDownloader::error
 * \return
 */
UpdateDownloader::UpdaterError UpdateDownloader::error() {
    return m_error;
}

/*!
 * \brief UpdateDownloader::size
 * \return
 */
qint64 UpdateDownloader::size() {
    return m_size;
}

/*!
 * \brief UpdateDownloader::description
 * \return
 */
QString UpdateDownloader::whatsNew() {
    return m_whatsnew;
}

/*!
 * \brief UpdateDownloader::instructions
 * \return
 */
QString UpdateDownloader::instructions() {
    return m_instructions;
}

/*!
 * \brief UpdateDownloader::url
 * \return
 */
QString UpdateDownloader::url() {
    return m_installerPath;
}

/*!
 * \brief UpdateDownloader::releaseNumber
 * \return
 */
int UpdateDownloader::releaseNumber() {

    return m_remoteReleaseNum;
}

/*!
 * \brief UpdateDownloader::sha256
 * \return
 */
QString UpdateDownloader::sha256() {
    return m_hash;
}

/*!
 * \brief UpdateDownloader::setTimeout
 * \param value
 */
void UpdateDownloader::setTimeout(int value) {
    m_timeout = value;
}

/*!
 * \brief UpdateDownloader::checkForUpdate
 * \param releaseNum
 */
void UpdateDownloader::checkForUpdate(qint64 releaseNum) {
    qDebug() << "checkForUpdate()";
    m_releaseNum = releaseNum;
    m_queue = FileQueue::Description;
    m_state = State::Check;
    startDownload(m_versionFile);
    emit started();
}

/*!
 * \brief UpdateDownloader::download
 */
void UpdateDownloader::download() {
    qDebug() << "download()";
    if(m_state == State::Available && !m_installerPath.isEmpty()) {
        m_queue = FileQueue::Installer;
        QString filePath = m_downloader->globaltoLocalPath(m_installerPath);

        QScopedPointer<QFile> file(new QFile(filePath));
        qDebug() << "download() check " << filePath;
        if(!tryToUpdate(file.data())) {
            qDebug() << "DOWNLOAD ONE MORE TIME";
            startDownload(m_installerPath);
        } else {
            qDebug() << "START LOCALLY";
        }
    }
}

/*!
 * \brief UpdateDownloader::startUpdate
 * \param app
 */
void UpdateDownloader::startUpdate(QCoreApplication* app) {
    qDebug() << "startUpdate";
    if(m_state == State::ReadyForInstall && !m_installerLocalPath.isEmpty()) {
        if(m_downloader)
            m_downloader->deleteLater();
        QProcess* updater = new QProcess();
        qDebug() << "startUpdate" <<  m_installerLocalPath;
        updater->setProgram(m_installerLocalPath);
        if(updater->startDetached()) {
            qDebug() << "QUIT";
            app->quit();
        } else {
            updater->deleteLater();
            qDebug() << "Failed to start an updater";
            emit errorOccured("Failed to start an updater");
        }
    }
}

/*!
 * \brief UpdateDownloader::stop
 */
void UpdateDownloader::stop() {
    m_downloader->stop();
}

// private slots

// TODO: сделай ошибки в виде перечислений и метод errorString()

void UpdateDownloader::slot_stepFinished() {
    qDebug() << "slot_stepFinished";
//    auto file = m_downloader->file();
    QScopedPointer<QFile> file(new QFile(m_downloader->filePath()));
    if(m_queue == FileQueue::Description) {
        if(!file->open(QIODevice::ReadWrite)) {
            emit errorOccured(file->errorString());
            m_state = State::None;
            return;
        }

        auto fileData = file->readAll();
        QJsonParseError jError;
        jError.error = QJsonParseError::NoError;
        auto jData = QJsonDocument::fromJson(fileData, &jError);
        if(jError.error != QJsonParseError::NoError) {
            emit errorOccured("Parsing error");
            m_state = State::None;
            return;
        }

        auto jDataMap = jData.toVariant().toMap();
        if(jDataMap.contains("data")) {
            auto updateMap = jDataMap.value("data").toMap();
            if(updateMap.contains("release_number")
                    && updateMap.contains("installer")
                    && updateMap.contains("sha-256")
                    && updateMap.contains("size")) {
                m_remoteReleaseNum = updateMap.value("release_number").toLongLong();
                bool available = (m_remoteReleaseNum > m_releaseNum);
                if(available) {
                    m_installerPath = updateMap.value("installer").toString();
                    m_hash = updateMap.value("sha-256").toString();
                    m_state = State::Available;
                    m_size = updateMap.value("size").toLongLong();
                    m_whatsnew = updateMap.value("whats_new", QString()).toString();
                    m_instructions = updateMap.value("instructions", QString()).toString();
                }
                emit updatesAvailable(available);
            } else {
                emit errorOccured("Wrong data received from update server");
                m_state = State::None;
            }
        } else {
            emit errorOccured("Wrong data received from update server");
            m_state = State::None;
        }
    }
    else if(m_queue == FileQueue::Installer) {
        if(!tryToUpdate(file.data())) {
            emit errorOccured("Checksum is incorrect!");
            m_state = State::None;
        }
    }
    if(m_downloader)
        m_downloader->deleteLater();
}

// private method

void UpdateDownloader::startDownload(const QString& file) {
    qDebug() << "startDownload" << file;
    if(m_downloader) {
        m_downloader->disconnect();
        m_downloader->deleteLater();
    }
    m_downloader = new FileDownloader(file, "updates");
    m_downloader->setTimeout(m_timeout);

    connect(m_downloader.data(), &FileDownloader::downloadProgress, this, &UpdateDownloader::downloadProgress);
    connect(m_downloader.data(), &FileDownloader::errorOccured, this, &UpdateDownloader::errorOccured);
//    connect(m_downloader.data(), &FileDownloader::started, this, &UpdateDownloader::started);
    connect(m_downloader.data(), &FileDownloader::finished, this, &UpdateDownloader::slot_stepFinished);

    m_downloader->start(FileDownloader::OverwriteIfNotTheSame);
}

bool UpdateDownloader::hashIsEquals(QFile* file) {
    qDebug() << "hashIsEquals" << file->fileName() << file->isOpen();
    if(!file->exists()) {
        return false;
    }
    if(!file->isOpen() && !file->open(QIODevice::ReadOnly)) {
        emit errorOccured(file->errorString());
        qDebug() << "hashIsEquals error" << file->errorString();
//        m_state = State::None; // TODO: может откоментить обратно
        return false;
    }
    QCryptographicHash hash(QCryptographicHash::Sha256);
    file->seek(0);
    hash.addData(file);

    auto fileHash = hash.result().toHex().toUpper();
    qDebug() << "HASH" << fileHash;
    qDebug() << "desc HASH" << m_hash;
    qDebug() << "HASHEs = " << (m_hash == fileHash);
    file->close();

    return (m_hash == fileHash);
}

bool UpdateDownloader::tryToUpdate(QFile* file) {
    bool equals = hashIsEquals(file);
    if(equals) {
        m_state = State::ReadyForInstall;
        m_installerLocalPath = file->fileName();
        qDebug() << "m_installerLocalPath in stepFInished()" << m_installerLocalPath;
        emit readyForUpdate();
    }
    return equals;
}
