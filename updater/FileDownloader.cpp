#include "FileDownloader.h"
#include <QtCore>
#include <QtGlobal>
#ifdef QT_DEBUG
#include <QDebug>
#endif

FileDownloader::FileDownloader(const QString& url, const QString& dirName, QObject *parent)
    : QObject(parent),
      m_url(url),
      m_manager(new QNetworkAccessManager(this)),
      m_hash(new QCryptographicHash(QCryptographicHash::Sha256))
{
    QString dir = dirName;
    m_dir.setPath(dir);
    if(!m_dir.exists())
        m_dir.mkpath(m_dir.absolutePath());

    m_manager->setTransferTimeout(10000);
    m_manager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    m_filePath = globaltoLocalPath(m_url);
}

FileDownloader::FileDownloader(const QString& url, QObject* parent) :
    FileDownloader(url, "updates", parent)
{

}

QString FileDownloader::filePath() {
    return m_filePath;
}

QString FileDownloader::globaltoLocalPath(const QString& file) {
    QString fileName = file.split("/").last();
    auto path = m_dir.absolutePath();
    if(!path.endsWith("\\") || !path.endsWith("/")) {
//        path.append(QDir::separator());
    }
    return path+QDir::separator()+fileName;
}

QByteArray FileDownloader::hash() {
    return m_hash->result();
}

QString FileDownloader::errorString() {
    return m_errorString;
}

FileDownloader::DownloaderError FileDownloader::error() {
    return m_error;
}

void FileDownloader::clear() {
    if(m_tempFile) {
        if(m_tempFile->isOpen())
            m_tempFile->close();
        m_tempFile->reset();
    }
    m_url.clear();
    m_filePath.clear();
    m_hash->reset();
}

void FileDownloader::setTimeout(int value) {
    m_manager->setTransferTimeout(value);
}

void FileDownloader::start(DownloadType type) {
    m_type = type;

    connect(m_manager, &QNetworkAccessManager::finished, this, &FileDownloader::slot_requestFinished);

    QNetworkRequest request;
    request.setUrl(QUrl(m_url));
    qDebug() << "REQUEST" << m_url  << __FILE__ << __LINE__;
//    request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, true);


    switch(m_type) {
    case NoOverwrite:
        if(QFile::exists(m_filePath)) {
            QString errorStr = QString("File %1 is exist! Can't overwrite it. Break! %2 %3").arg(m_filePath).arg(__FILE__).arg(__LINE__);
            emit errorOccured(errorStr);
            clear();
            return;
        }
        m_tempFile.reset(new QFile(m_filePath));
        break;
    case Overwrite:
        m_tempFile.reset(new QFile(m_filePath));
        break;
    case OverwriteIfNotTheSame:
        m_tempFile.reset(new QTemporaryFile());
        break;
    }


    if(!m_tempFile->open(QIODevice::ReadWrite)) {
#ifdef QT_DEBUG
        qDebug() << "Can't create temp file" << m_tempFile->fileName();
#endif
        emit errorOccured(QString("Can't create a temporary file!").arg(__FILE__).arg(__LINE__));
        return;
    }

    m_reply = m_manager->get(request);
    if(m_reply)
        qDebug() << "REPLY GOOD";
    else
        qDebug() << "REPLY BAD";
    bool st = connect(m_reply, &QNetworkReply::downloadProgress, this, &FileDownloader::downloadProgress);
    Q_ASSERT(st);
    st = connect(m_reply, &QIODevice::readyRead, this, &FileDownloader::slot_readyRead);
    Q_ASSERT(st);
    st = connect(m_reply, &QNetworkReply::errorOccurred, this, [this](){
        emit errorOccured(m_reply->errorString());
        qDebug() << "REQUEST ERROR " << m_reply->errorString()  << __FILE__ << __LINE__;
        clear();
    });
    Q_ASSERT(st);
    st = connect(m_reply, &QNetworkReply::sslErrors, this, [this](const QList<QSslError> &errors){
        QString errorString;
        qDebug() << errors;

        for(const auto& error : errors) {
            errorString += error.errorString()+ "; ";
        }
        if(errorString.isEmpty()) {
            emit errorOccured(errorString + __FILE__ +QString::number(__LINE__));
        }
        clear();
    });
    Q_ASSERT(st);
    emit started();
    m_reply->dumpObjectInfo();
    qDebug() << "started()";
}

void FileDownloader::stop() {
    if(m_reply)
        m_reply->abort();
}

// private slots

void FileDownloader::slot_requestFinished(QNetworkReply* reply) {
#ifdef QT_DEBUG
    qDebug() << "slot_requestFinished";
#endif
    if(reply->error())
#ifdef QT_DEBUG
        qDebug() << "ERR" << reply->errorString();
#endif
    if (reply->error() == QNetworkReply::NoError)
    {
        switch(m_type) {
        case OverwriteIfNotTheSame:
            if(!needRewrite()) {
#ifdef QT_DEBUG
                qDebug() << "NO REWRITE";
#endif
                break;
// Если перезапись не нужна, то покидаем switch, иначе не попадаем в этот if и проходим в следующий case
            }
#ifdef QT_DEBUG
            qDebug() << "REWRITE";
#endif
        case Overwrite:
            rewriteFile();
            break;
        default:
            break;
        }
        m_tempFile->close();
        emit finished();
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() << "ERROR 3";
#endif
        emit errorOccured(reply->errorString() +  __FILE__ + QString::number(__LINE__));
        m_tempFile->close();
        m_tempFile->remove();
    }
    m_reply->reset();
    m_reply->deleteLater();
}

void FileDownloader::slot_readyRead() {
    QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
    qDebug() << "ReadyRead" << __FILE__ << __LINE__;
    if(m_tempFile) {
        auto data = reply->readAll();
        if(m_tempFile && m_tempFile->isOpen())
            m_tempFile->write(data);
    }

}

// private methods

void FileDownloader::rewriteFile() {
#ifdef QT_DEBUG
    qDebug() << "rewriteFile()";
#endif
    QScopedPointer<QFile> m_file;
    m_file.reset(new QFile(m_filePath));
    if(!m_file->open(QIODevice::WriteOnly)) {
        emit errorOccured(QString("Can't open fiele [%1] %2 %3").arg(m_filePath).arg(__FILE__).arg(__LINE__));
        return;
    }
    m_tempFile->seek(0);

    while(!m_tempFile->atEnd()) {
        auto s = m_tempFile->read(1024);
        m_file->write(s);
    }
    m_file->close();
}

bool FileDownloader::needRewrite() {
    QCryptographicHash outHash(QCryptographicHash::Sha256);
    m_tempFile->seek(0);
    m_hash->addData(m_tempFile.data());

    QFile outFile(m_filePath);
    if(outFile.exists() && outFile.open(QIODevice::ReadOnly)) {
        outHash.addData(&outFile);
        return outHash.result() != m_hash->result();
    }
    return true;

}
