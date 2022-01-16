#include "UpdatesChecker.h"
#include <QProcess>
#include <QDebug>
#include <QtXml>

#if defined(QT_DEBUG)
    const QString UpdatesChecker::m_updaterFile = "C:/Program Files/Maiman Electronics/Multi Workbench/MaintenanceTool.exe";
#else
    const QString UpdatesChecker::m_updaterFile = "./MaintenanceTool.exe";
#endif

UpdatesChecker::UpdatesChecker(QObject *parent)
    : QObject{parent}
{

}

QString UpdatesChecker::version() const {
    return m_version;
}
qint64 UpdatesChecker::size() const {
    return m_size;
}

QString UpdatesChecker::sizeFormated() const {
    static const QString units = "KMGT";
    auto size = m_size;
    int idx = -1;
    while(size > 1000) {
        size /= 1000;
        ++idx;
    }
    if(idx == -1)
        return QString::number(size);
    else
        return QString::number(size) + units.at(idx);
}

void UpdatesChecker::checkForUpdates() {
    m_maintanceProcess = new QProcess(this);
    QStringList args;
    args << "check-updates";

    connect(m_maintanceProcess, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error){
        Q_UNUSED(error)
        qDebug() << m_maintanceProcess->errorString();
        emit errorOccured(m_maintanceProcess->errorString());
    });
    connect(m_maintanceProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &UpdatesChecker::analyzeOutput);
    m_maintanceProcess->start(m_updaterFile, args);
}

void UpdatesChecker::startUpdate(QCoreApplication* app) {
    QScopedPointer<QProcess> updater(new QProcess());
    updater->setProgram(m_updaterFile);
    if(updater->startDetached()) {
        app->quit();
    } else {
        emit errorOccured("Failed to start an updater");
    }

}

void UpdatesChecker::analyzeOutput() {
    auto output = QString(m_maintanceProcess->readAllStandardOutput()).split('\n');

    bool enableSave = false;
    QString xml;
    for(auto& str : output) {
        str.remove('\r');
        if(!str.isEmpty()) {
            if(str == "<updates>") {
                enableSave = true;
            }
            if(enableSave) {
                xml += str;
            }
            if(str == "</updates>") {
                break;
            }
        }
    }

    bool isOk = false;
    if(!xml.isEmpty()) {
        QScopedPointer<QXmlStreamReader> xmlStream(new QXmlStreamReader(xml));
        while(!xmlStream->atEnd() && !xmlStream->hasError()) {
            auto token = xmlStream->readNext();
            if(token == QXmlStreamReader::StartElement
                    && !xmlStream->hasError()) {
                auto tagName = xmlStream->name();
                if(tagName == "update") {
                    const auto& attributes = xmlStream->attributes();
                    for(const auto& item : attributes) {
                        if(item.name() == "size") {
                            m_size = item.value().toLongLong();
                            isOk = true;
                        } else if (item.name() == "version") {
                            m_version = item.value().toString();
                            isOk = true;
                        }
                    }
                }
            }
        }
    }

    emit updatesAvailable(isOk);
}
