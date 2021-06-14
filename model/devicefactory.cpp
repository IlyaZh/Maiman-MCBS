#include "devicefactory.h"
#include <QDebug>
#include "../device/devcommand.h"

bool sortCmdsByCode(const DevCommandBuilder* p1, const DevCommandBuilder* p2) {
    return p1->code() < p2->code();
}

DeviceFactory::DeviceFactory(QString fileName, QSharedPointer<AppSettings> settings, QObject* parent) :
    QObject(parent),
    m_settings(settings),
    m_fileName(fileName)

{

}

DeviceFactory::~DeviceFactory() {
//    m_parser->stop();
    if(m_parseWorker) m_parseWorker->deleteLater();
}

void DeviceFactory::start() {
    m_parseWorker = new ParserWorker(m_fileName, ParserType::XmlParser);
    m_thread = new QThread();
    m_parseWorker->moveToThread(m_thread);

    connect(m_thread, SIGNAL(started()), m_parseWorker, SLOT(process()));
    connect(m_parseWorker, SIGNAL(finished()), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(finished()), this, SLOT(parsingFinished()));
//    //connect(...,m_parser, SLOT(stop()));
    // проверь сигналы по статье
//    connect(m_parseWorker, SIGNAL(finished()), m_parseWorker, SLOT(deleteLater()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), this, SLOT(threadError(QString)));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(deleteLater()));
    m_thread->start();
}

Device* DeviceFactory::createDevice(quint8 addr, quint16 id) {
    DeviceModel* devModel = findModel(id);
    if(devModel == nullptr) {
        qDebug() << "Device model with id =" << id << "has not found!";
        return nullptr;
    } else {
        return devModel->createDevice(addr);
    }
}


// private slots
void DeviceFactory::parsingFinished() {
    TreeItem* parserTree = m_parseWorker->data();
    m_parseWorker->deleteLater();
    m_parseWorker.clear();
    if(parseTree(parserTree)) {
        qDebug() << "Parse tree is ok!";
    } else {
        qDebug() << "Can't parse tree";
    }
}

void DeviceFactory::threadError(QString str) {
    qDebug() << "Error while parsing. " << str;
}

// private methods
DeviceModel* DeviceFactory::findModel(quint16 id) {
    for(auto device : m_DeviceModels) {
        if(device->id() == id) {
            return device.data();
        }
    }

    return nullptr;
}

bool DeviceFactory::parseTree(TreeItem* tree) {
    if(tree != nullptr) {
        if(tree->name() == "DB") {
            for(int i = 0; i < tree->childCount(); i++) {
                TreeItem* childDb = tree->child(i);

                QString tagName = childDb->name();
                if(tagName == "BaudRate") {
                    m_baudrates << parseBaudRates(childDb);
                }
                if(tagName == "CommonIDDevices") {
                    parseCommonDevId(childDb);
                }

            }
        }
    }

    return false;
}

QStringList DeviceFactory::parseBaudRates(TreeItem* item) {
    QStringList list;

    for(int i = 0; i < item->childCount(); i++) {
        TreeItem* child = item->child(i);
        if(child->name() == "value") {
            list.append(child->value().toString());
        }
    }

    return list;
}

QVector<QPair<uint, QString>> DeviceFactory::parseCommonDevId(TreeItem* item) {
    QVector<QPair<uint, QString>> list;

    for(int i = 0; i < item->childCount(); i++) {
        TreeItem* child = item->child(i);
    }

    return list;
}
