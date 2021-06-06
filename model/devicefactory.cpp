#include "devicefactory.h"
#include <QDebug>
#include "../device/devcommand.h"

bool sortCmdsByCode(const DevCommandBuilder* p1, const DevCommandBuilder* p2) {
    return p1->code() < p2->code();
}

DeviceFactory::DeviceFactory(Parser* parser, QSharedPointer<AppSettings> settings, QObject* parent) :
    QObject(parent),
    m_settings(settings),
    m_parser(parser)

{

}

DeviceFactory::~DeviceFactory() {
//    m_parser->stop();
}

void DeviceFactory::start() {
    m_thread = new QThread();

    m_parser->moveToThread(m_thread);

    connect(m_thread, SIGNAL(started()), m_parser.get(), SLOT(process()));
    connect(m_parser.get(), SIGNAL(finished()), m_thread, SLOT(quit()));
    connect(m_parser.get(), SIGNAL(finished()), this, SLOT(parsingFinished()));
//    //connect(...,m_parser, SLOT(stop()));
//    connect(m_parser, SIGNAL(finished()), m_parser, SLOT(deleteLater()));
//    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
    connect(m_parser.get(), SIGNAL(errorOccured(QString)), this, SLOT(threadError(QString)));
    connect(m_parser.get(), SIGNAL(errorOccured(QString)), m_thread, SLOT(quit()));
    connect(m_parser.get(), SIGNAL(errorOccured(QString)), m_thread, SLOT(deleteLater()));
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
