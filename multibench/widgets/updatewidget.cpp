#include "updatewidget.h"
#include "version.h"

const QString UpdateWidget::m_Url = "https://telegraphy.ru/updates.txt";

UpdateWidget::UpdateWidget(QWidget *parent) : QWidget(parent),
    m_updater(new UpdatesChecker())
{
    connect(m_updater,&UpdatesChecker::updatesAvailable,this,&UpdateWidget::updateCheck);
    connect(m_updater,&UpdatesChecker::errorOccured,this,&UpdateWidget::acceptError);
    m_updater->checkForUpdates();
}

void UpdateWidget::updateCheck(bool state){
    if (state){
        QMessageBox msgBox;
        QString message = QString("Do you want to download and install new update?");
        msgBox.setWindowTitle("Updates Available");
        msgBox.setText(message);
        msgBox.setContentsMargins(0,0,0,0);
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
            m_updater->startUpdate(qApp);
            break;
        case QMessageBox::No:
            qDebug()<<"No";
            break;
        default:
            break;
        }
    }
}

void UpdateWidget::acceptError(const QString& errorMsg){
    qDebug()<< "[ERROR]" << errorMsg;
}
