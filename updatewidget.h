#ifndef UPDATEWIDGET_H
#define UPDATEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "constants.h"
#include "UpdateDownloader.h"
#include "version.h"

class UpdateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateWidget(QWidget *parent = nullptr);
private:
    static const QString m_Url;
    UpdateDownloader* m_downloader;

signals:
    void updateApp(QCoreApplication* app);
    void downloadApp();
    void downloadFinished();

private slots:
    void acceptError(QString errorMsg);
    void updateCheck(bool state);
};

#endif // UPDATEWIDGET_H
