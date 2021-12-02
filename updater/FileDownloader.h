#pragma once

#include <QObject>
#include <QtNetwork>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    enum DownloadTypeFlag {
        NoOverwrite,
        Overwrite,
        OverwriteIfNotTheSame
    };
    Q_DECLARE_FLAGS(DownloadType, DownloadTypeFlag);

    enum DownloaderErrorFlag {
        NoError,
    };
    Q_DECLARE_FLAGS(DownloaderError, DownloaderErrorFlag);

    explicit FileDownloader(const QString& url, const QString& dirName, QObject *parent = nullptr);
    explicit FileDownloader(const QString& url, QObject *parent = nullptr);
    QString filePath();
    QString globaltoLocalPath(const QString& file);
    QByteArray hash();
    QString errorString();
    DownloaderError error();
    void clear();
    void setTimeout(int);
public slots:
    void start(DownloadType type = OverwriteIfNotTheSame);
    void stop();
signals:
    void started();
    void finished();
    void errorOccured(QString errorString);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void slot_requestFinished(QNetworkReply* reply);
    void slot_readyRead();


private:
    QString m_url;
    QSharedPointer<QFile> m_tempFile;
    QNetworkAccessManager* m_manager;
    QPointer<QNetworkReply> m_reply;
    DownloadType m_type {NoOverwrite};
    QDir m_dir;
    QString m_filePath;
    QScopedPointer<QCryptographicHash> m_hash;
    QString m_errorString;
    DownloaderError m_error {NoError};

    void rewriteFile();
    bool needRewrite();
};

