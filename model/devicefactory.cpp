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
    if (m_parser != nullptr){
        QModelIndex indexFirst = m_parser->index(1, 0);
        QString check = indexFirst.data().toString();
        if (check == "DB"){
            for (int iCount=0 ;iCount<=(m_parser->rowCount(indexFirst));iCount++ ) {
                QModelIndex indexID = SForward(iCount,0,indexFirst);
                if (indexID.data().toString() =="Device")
                    CommandReader(iCount);
            }
        }
    }

    m_parser->deleteLater();
    m_parser = nullptr;
    m_thread->deleteLater();
    m_thread = nullptr;
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

void DeviceFactory::CommandReader(int row)
{
    QModelIndex indexID = SForward(row,0,m_parser->index(1, 0));
    QModelIndex indexCommands = findTag("Commands",indexID);

    uint deviceID = AttributesValue("id",indexID).toString().toUInt(nullptr, 16);
    QString devName = AttributesValue("name",indexID).toString();
    uint maxCommDelay = AttributesValue("maxCommandDelayMs",indexID).toUInt();
    uint minCommDelay = AttributesValue("minCommandDelayMs",indexID).toUInt();
    uint stopCommandDelay = AttributesValue("stopCommandDelayMs",indexID).toUInt();
    uint timeoutMs = m_settings->getDeviceTimeout();

    DeviceDelays* delay = new DeviceDelays(stopCommandDelay, minCommDelay, maxCommDelay, timeoutMs);

    QVector<DevCommandBuilder*>* cmdVector = new QVector<DevCommandBuilder*>();

    Command fastCommand;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(indexCommands));iCount++ ) {
        QModelIndex indexCommand = SForward(iCount,0,indexCommands);
        if(indexCommand.data().toString() == "Command"){
            quint16 code = 0;
            uint divider = 0;
            uint interval = 1;
            QString unit = "";
            uint tol = 0;
            bool isTemperature = false;
            bool isSigned = false;

            code = AttributesValue("code",indexCommand).toUInt();
            divider = AttributesValue("divider",indexCommand).toUInt();
            interval = AttributesValue("interval",indexCommand).toUInt();
            unit = AttributesValue("unit",indexCommand).toString();
            tol = AttributesValue("tol",indexCommand).toUInt();
            isTemperature = AttributesValue("isTemperature",indexCommand).toBool();
            isSigned = AttributesValue("isSigned",indexCommand).toBool();

            if(code == 0 && divider == 0) {
                continue;
            }

            DevCommandBuilder* cmdBuilder = new DevCommandBuilder(code, unit, divider, tol, interval, isSigned, isTemperature);

            if(cmdBuilder != nullptr) cmdVector->append(cmdBuilder);
        }
    }

    std::stable_sort(cmdVector->begin(), cmdVector->end(), sortCmdsByCode);
    DeviceModel* dev = new DeviceModel(deviceID,
                                       devName,
                                       delay,
                                       cmdVector);
    m_DeviceModels.append(QSharedPointer<DeviceModel>(dev));
}

QVariant DeviceFactory::AttributesValue(QString attribName,QModelIndex index)
{
    m_parser->attribFind(attribName);
    return index.siblingAtColumn(3).data(Qt::DisplayRole);

}

//QMap<QString, Command> DeviceFactory::getCommands(QString id)
//{
//    QVector<device_t>::const_iterator iter = m_device.constBegin();
//    QMap<QString, Command> commandID;
//    while(iter != m_device.constEnd()){
//        if (iter->deviceID == id){
//            commandID = iter->commands;
//        }
//        iter++;
//    }
//    return commandID;
//}

QStringList DeviceFactory::getBaudrate()
{
    QModelIndex indexBaudrate = findTag("BaudRate",m_parser->index(1, 0));
    for (int iCount=indexBaudrate.row() ;iCount<=(m_parser->rowCount(m_parser->index(1, 0)));iCount++ ) {
        indexBaudrate = indexBaudrate.siblingAtRow(iCount);
        if (indexBaudrate.data().toString() =="BaudRate"){
            m_parser->attribFind("value");
            baudrate.append(indexBaudrate.siblingAtColumn(3).data().toString());
        }
    }
    return baudrate;
}

//QMap<QString, QString> DeviceFactory::getIDDevices()
//{
//    QModelIndex index = findTag("CommonIDDevices",m_parser->index(1, 0));
//    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
//        QModelIndex indexID = SForward(iCount,0,index);
//        if (indexID.data().toString() =="CIDD"){
//            for(int i=0;i<=(m_parser->rowCount(indexID));i++){
//                if(SForward(i,0,indexID).siblingAtColumn(2).data().toString()!="")
//                    IDDevice.insert(AttributesValue("id",indexID).toString(),
//                                    SForward(i,0,indexID).siblingAtColumn(2).data().toString());
//            }
//        }
//    }
//    return IDDevice;
//}


QModelIndex DeviceFactory::indexStepBack(int row,int column)
{
    return m_parser->parent(m_parser->index(row, column, m_parser->index(1, 0)));
}

QModelIndex DeviceFactory::SForward(int row,int column,QModelIndex parent)
{
    return m_parser->index(row, column, parent);
}

QString DeviceFactory::findDeviceID(QString id,int* row)
{
    QString findID = "id=" + id ;
    for (int iCount=0 ;iCount<=(m_parser->rowCount( m_parser->index(1, 0)));iCount++ ) {
        QModelIndex index = m_parser->index(iCount, 1,m_parser->index(1, 0));
        QString text = index.data(Qt::DisplayRole).toString();
        if(text.contains(findID, Qt::CaseInsensitive)){
            *row = iCount;
            return text;
        }
    }
    return QString();
}

QModelIndex DeviceFactory::findTag(QString tag, QModelIndex parent)
{
    QModelIndex index;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(parent));iCount++ ) {
        index = SForward(iCount,0,parent);
        if (index.data().toString() == tag)
            return index;
    }
    return QModelIndex();
}