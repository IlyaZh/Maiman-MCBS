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

UpdateDownloader::UpdateDownloader(const QString& versionFileUrl, QObject* parent) :
    QObject(parent),
    m_versionFile(versionFileUrl),
//    m_dir(dir),
    m_queue(FileQueue::Description),
    m_state(State::None)
{

}

// TODO:: Добавиь где надо const к функциям
QString UpdateDownloader::errorString() {
    return m_errorString;
}

UpdateDownloader::UpdaterError UpdateDownloader::error() {
    return m_error;
}

void UpdateDownloader::checkForUpdate(qint64 releaseNum) {
    qDebug() << "checkForUpdate()";
    m_releaseNum = releaseNum;
    m_queue = FileQueue::Description;
    m_state = State::Check;
    startDownload(m_versionFile);
}

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
                    && updateMap.contains("sha-256")) {
                bool available = (updateMap.value("release_number").toLongLong() > m_releaseNum);
                if(available) {
                    m_installerPath = updateMap.value("installer").toString();
                    m_hash = updateMap.value("sha-256").toString();
                    m_state = State::Available;
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

    connect(m_downloader.data(), &FileDownloader::downloadProgress, this, &UpdateDownloader::downloadProgress);
    connect(m_downloader.data(), &FileDownloader::errorOccured, this, &UpdateDownloader::errorOccured);
    connect(m_downloader.data(), &FileDownloader::started, this, &UpdateDownloader::started);
    connect(m_downloader.data(), &FileDownloader::finished, this, &UpdateDownloader::slot_stepFinished);

    m_downloader->start(FileDownloader::OverwriteIfNotTheSame);
}

bool UpdateDownloader::hashIsEquals(QFile* file) {
    if(!file->open(QIODevice::ReadOnly)) {
        emit errorOccured(file->errorString());
        m_state = State::None;
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
