#pragma once

#include "UpdateDownloader_global.h"
#include <QObject>
#include <QtNetwork>
#include "FileDownloader.h"

class UpdateDownloader : public QObject
{
    Q_OBJECT
    QString m_versionFile;
    QString m_dir;
    QString m_installerPath;
    QString m_installerLocalPath;
    QString m_hash;
    qint64 m_releaseNum = 0;
    QString m_errorString;
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
    enum UpdaterErrorFlag {
        NoError = 0
    };
    Q_DECLARE_FLAGS(UpdaterError, UpdaterErrorFlag)

    static QString bytesToString(qint64 value);

    explicit UpdateDownloader(const QString& versionFileUrl, QObject* parent = nullptr);
    QString errorString();
    UpdaterError error();
public slots:
    void checkForUpdate(qint64 releaseNum);
    void download();
    void startUpdate(QCoreApplication* app);
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
