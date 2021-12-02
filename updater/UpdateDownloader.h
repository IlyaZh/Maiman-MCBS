#pragma once

#include <QObject>
#include <QtNetwork>
#include "FileDownloader.h"

class UpdateDownloader : public QObject
{
    Q_OBJECT
    static const int DefaultTimeout;

    QString m_versionFile;
    QString m_dir;
    QString m_installerPath;
    QString m_installerLocalPath;
    QString m_hash = "";
    qint64 m_size {0};
    QString m_whatsnew = "";
    QString m_instructions = "";
    qint64 m_releaseNum = 0;
    qint64 m_remoteReleaseNum = 0;
    QString m_errorString;
    int m_timeout = DefaultTimeout;

    QPointer<FileDownloader> m_downloader;

    enum FileQueue {
        Description,
        Installer
    };
    enum State {
        None,
        Check,
        ReadyForInstall,
        Available,
        Done
    };

    FileQueue m_queue;
    State m_state;

    void startDownload(const QString& file);
    bool hashIsEquals(QFile* file);
    bool needToDownload();
    bool tryToUpdate(QFile* file);
public:
    /*!
     * \brief The UpdaterErrorFlag enum
     */
    enum UpdaterErrorFlag {
        NoError = 0
    };
    Q_DECLARE_FLAGS(UpdaterError, UpdaterErrorFlag)

    static QString bytesToString(qint64 value);

    explicit UpdateDownloader(const QString& versionFileUrl, QObject* parent = nullptr);
    QString errorString();
    UpdaterError error();
    qint64 size();
    QString whatsNew();
    QString instructions();
    QString url();
    int releaseNumber();
    QString sha256();
    void setTimeout(int value);
public slots:
    void checkForUpdate(qint64 releaseNum);
    void download();
    void startUpdate(QCoreApplication* app);
    void stop();
signals:
    void started();
    void updatesAvailable(bool state);
    void readyForUpdate();
    void errorOccured(QString errorMsg);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void slot_stepFinished();

private:
    UpdaterError m_error {NoError};
};
