#include "updatewidget.h"

const QString UpdateWidget::m_Url = "https://telegraphy.ru/updates.txt";

UpdateWidget::UpdateWidget(QWidget *parent) : QWidget(parent),
    m_downloader(new UpdateDownloader(m_Url))
{
    connect(m_downloader,&UpdateDownloader::updatesAvailable,this,&UpdateWidget::updateCheck);
    connect(m_downloader,&UpdateDownloader::readyForUpdate,this,&UpdateWidget::downloadFinished);
    connect(m_downloader,&UpdateDownloader::errorOccured,this,&UpdateWidget::acceptError);
    connect(m_downloader, &UpdateDownloader::downloadProgress, this, [this](auto received, auto total){
        qDebug() << "progress" << received << total;
    });
    m_downloader->checkForUpdate(0);
}

void UpdateWidget::updateCheck(bool state){
    if (state){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Update");
        msgBox.setText("Do you want to update software?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setStyleSheet("* {	\
                background: rgb(51,51,51);\
                color: rgb(255, 255, 255);\
                font: 12pt Share Tech Mono;\
            }\
            QMessageBox *{\
                background: rgb(51,51,51);\
                color: rgb(255, 255, 255);\
            }\
            QPushButton {\
                color: rgb(255, 255, 255);\
                background-color: rgb(51, 51, 51);\
                border: 1px solid #999999;\
                border-radius: 4px;\
                padding: 4px;\
            }");
        int answer = msgBox.exec();
        switch (answer) {
        case QMessageBox::Yes:
            qDebug()<<"Yes";
            m_downloader->download();
            break;
        case QMessageBox::No:
            qDebug()<<"No";
            break;
        default:
            break;
        }
    }
}

void UpdateWidget::acceptError(QString errorMsg){
    qDebug()<< "[ERROR]" << errorMsg;
}
