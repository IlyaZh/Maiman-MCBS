#include "devicefactory.h"
#include <QString>

DeviceFactory::DeviceFactory(Parser *parser)
{
    m_parser = parser;
    start();
}
DeviceFactory::~DeviceFactory()
{

}

void DeviceFactory::start()
{
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
}

void DeviceFactory::CommandReader(int row)
{
    QModelIndex indexID = SForward(row,0,m_parser->index(1, 0));
    QModelIndex indexCommands = findTag("Commands",indexID);

    device_t device;

    device.deviceID = AttributesValue("id",indexID).toString();
    device.devName = AttributesValue("name",indexID).toString();
    device.maxCommDelay = AttributesValue("maxCommandDelayMs",indexID).toUInt();
    device.minCommDelay = AttributesValue("minCommandDelayMs",indexID).toUInt();
    device.stopCommandDelay = AttributesValue("stopCommandDelayMs",indexID).toUInt();

    Command fastCommand;
    for (int iCount=0 ;iCount<=(m_parser->rowCount(indexCommands));iCount++ ) {
        QModelIndex indexCommand = SForward(iCount,0,indexCommands);
        if(indexCommand.data().toString() == "Command"){
            fastCommand.code = AttributesValue("code",indexCommand).toString();
            fastCommand.divider = AttributesValue("divider",indexCommand).toUInt();
            fastCommand.interval = AttributesValue("interval",indexCommand).toUInt();
            fastCommand.unit = AttributesValue("unit",indexCommand).toString();
            fastCommand.tol = AttributesValue("tol",indexCommand).toBool();
            fastCommand.isTemperature =  AttributesValue("isTemperature",indexCommand).toBool();

            device.commands.insert(fastCommand.code,fastCommand);
        }
    }
    m_device.append(device);
}

QVariant DeviceFactory::AttributesValue(QString attribName,QModelIndex index)
{
    m_parser->attribFind(attribName);
    return index.siblingAtColumn(3).data(Qt::DisplayRole);

}

QMap<QString, Command> DeviceFactory::getCommands(QString id)
{
    QVector<device_t>::const_iterator iter = m_device.constBegin();
    QMap<QString, Command> commandID;
    while(iter != m_device.constEnd()){
        if (iter->deviceID == id){
            commandID = iter->commands;
        }
        iter++;
    }
    return commandID;
}

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

QMap<QString, QString> DeviceFactory::getIDDevices()
{
    QModelIndex index = findTag("CommonIDDevices",m_parser->index(1, 0));
    for (int iCount=0 ;iCount<=(m_parser->rowCount(index));iCount++ ) {
        QModelIndex indexID = SForward(iCount,0,index);
        if (indexID.data().toString() =="CIDD"){
            for(int i=0;i<=(m_parser->rowCount(indexID));i++){
                if(SForward(i,0,indexID).siblingAtColumn(2).data().toString()!="")
                    IDDevice.insert(AttributesValue("id",indexID).toString(),
                                    SForward(i,0,indexID).siblingAtColumn(2).data().toString());
            }
        }
    }
    return IDDevice;
}


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

// private slots
void DeviceFactory::parsingFinished()
{
    QString error;
    emit m_parser->errorOccured(error);
    if(error == ""){
        start();
    }
    else
        qDebug()<<"Error: "<<error;
}
