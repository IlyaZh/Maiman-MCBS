#include "devicefactory.h"
#include <QDebug>
#include "../device/devcommand.h"

bool sortCmdsByCode(const DevCommandBuilder* p1, const DevCommandBuilder* p2) {
    return p1->code() < p2->code();
}

DeviceFactory::DeviceFactory(QString fileName, AppSettings& settings, QObject* parent) :
    QObject(parent),
    m_settings(settings),
    m_fileName(fileName)

{

}

DeviceFactory::~DeviceFactory() {
    //    m_parser->stop();
    //    if(m_parseWorker) m_parseWorker->deleteLater();
}

void DeviceFactory::start() {
    if(!m_parseWorker.isNull()) m_parseWorker->deleteLater();
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
        return devModel->createDevice(addr, this);
    }
}

QStringList DeviceFactory::getBaudrate() {
    return m_baudrates;
}

QVector<QPair<uint, QString>> DeviceFactory::getCommonDeviceIDs() {
    return m_commondDevicesId;
}


// private slots
void DeviceFactory::parsingFinished() {
    TreeItem* parserTree = m_parseWorker->data();
    qDebug() << "m_parseWorker.reset();";
//    m_parseWorker->deleteLater();
    if(parseTree(*parserTree)) {
        qDebug() << "Parse tree is ok!";
    } else {
        qDebug() << "Can't parse tree";
    }
    m_parseWorker->deleteLater();
    delete parserTree;
}

void DeviceFactory::threadError(const QString& str) {
    qDebug() << "Error while parsing. " << str;
}

// private methods
DeviceModel* DeviceFactory::findModel(quint16 id) {
    for(auto device : m_DeviceModels) {
        if(device->id() == id) {
            return device;
        }
    }

    return nullptr;
}

bool DeviceFactory::parseTree(TreeItem& tree) {
    if(tree.name() == "DB") {
        for(int i = 0; i < tree.childCount(); i++) {
            TreeItem& childDb = *tree.child(i);

            QString tagName = childDb.name();
            if(tagName == "BaudRate") {
                m_baudrates << parseBaudRate(childDb);
            }
            if(tagName == "CommonIDDevices") {
                m_commondDevicesId = parseCommonDevId(childDb);
            }
            if(tagName == "Device") {
                DeviceModel* devModel = parseDevice(childDb);
                if(devModel != nullptr)
                    m_DeviceModels << devModel;
            }

        }
        return true;
    }

    return false;
}

QString DeviceFactory::parseBaudRate(TreeItem& item) {
    for(int i = 0; i < item.childCount(); i++) {
        TreeItem* child = item.child(i);
        if(child->name() == "value") {
            return child->value().toString();
        }
    }
    return QString("");
}

QVector<QPair<uint, QString>> DeviceFactory::parseCommonDevId(TreeItem& item) {
    QVector<QPair<uint, QString>> list;

    for(int i = 0; i < item.childCount(); i++) {
        TreeItem* tagItem = item.child(i);
        QString devName = tagItem->value().toString();
        uint devId = 0;
        for(int j = 0; j < tagItem->childCount(); j++) {
            TreeItem* attr = tagItem->child(j);
            if(attr->name() == "id") {
                devId = attr->value().toUInt();
                break;
            }
        }
        if(devId != 0)
            list << qMakePair(devId, devName);
    }

    return list;
}

DeviceModel* DeviceFactory::parseDevice(TreeItem& item) {
    quint16 id = 0;
    QString name = "";
    DeviceDelays *delays = nullptr;
    QVector<DevCommandBuilder*> *cmdBuilders = nullptr;
    quint16 stopDelayMs = DeviceDelays::COM_STOP_DELAY_MS;
    quint16 minCommandDelayMs = DeviceDelays::COM_COMMAND_MIN_SEND_DELAY;
    quint16 maxCommandDelayMs = DeviceDelays::COM_COMMAND_MAX_SEND_DELAY;

    bool hasId = false;
    bool hasName = false;
    bool hasCommands = false;


    for(int i = 0; i < item.childCount(); i++) {
        TreeItem* child = item.child(i);

        if(child->name() == "id") {
            id = child->value().toString().toUInt(nullptr, 16);
            hasId = true;
        }

        if(child->name() == "name") {
            name = child->value().toString();
            hasName = true;
        }

        if(child->name() == "stopCommandDelayMs")
            stopDelayMs = child->value().toUInt();

        if(child->name() == "minCommandDelayMs")
            minCommandDelayMs = child->value().toUInt();

        if(child->name() == "maxCommandDelayMs")
            maxCommandDelayMs = child->value().toUInt();

        if(child->name() == "Commands") {
            cmdBuilders = parseCommands(*child);
            if(!cmdBuilders->isEmpty()) hasCommands = true;
        }
    }
    if(hasCommands && hasId && hasName) {
        delays = new DeviceDelays(stopDelayMs, minCommandDelayMs, maxCommandDelayMs);
        return new DeviceModel(id, name, delays, cmdBuilders);
    } else {
        return nullptr;
    }
}

QVector<DevCommandBuilder*>* DeviceFactory::parseCommands(TreeItem& item) {
    quint16 code = 0;
    QString unit = "";
    double divider = 1;
    quint8 tol = 0;
    uint interval = 1;
    bool isSigned = false;
    bool isTemperature = false;

    QVector<DevCommandBuilder*>* list = new QVector<DevCommandBuilder*>();

    bool hasCode = false;

    for(int c = 0; c < item.childCount(); c++) {
        TreeItem* cmd = item.child(c);

        for(int i = 0; i < cmd->childCount(); i++) {
            TreeItem* child = cmd->child(i);

            if(child->name() == "code") {
                code = child->value().toUInt();
                hasCode = true;
            }

            if(child->name() == "unit") {
                unit = child->value().toString();
                unit.replace("(deg)", QString::fromRawData(new QChar('\260'), 1));
            }

            if(child->name() == "divider")
                divider = child->value().toDouble();

            if(child->name() == "tol")
                tol = child->value().toUInt();

            if(child->name() == "interval")
                interval = child->value().toUInt();

            if(child->name() == "isSigned")
                isSigned = true;

            if(child->name() == "isTemperature")
                isTemperature = true;

        }

        if(hasCode) {
            list->append(new DevCommandBuilder(code, unit, divider, tol, interval, isSigned, isTemperature));
        }
    }


    std::sort(list->begin(), list->end(), sortCmdsByCode);
    // test it

    return list;
}
