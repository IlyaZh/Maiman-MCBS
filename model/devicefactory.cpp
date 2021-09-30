#include "devicefactory.h"
#include <QDebug>
#include "../device/commandsettings.h"
#include <QScopedPointer>

DeviceFactory::DeviceFactory(QString fileName, AppSettings& settings, QObject* parent) :
    QObject(parent),
    m_thread(new QThread()),
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
//    m_thread = new QThread();
    m_parseWorker.reset(new ParserWorker(m_fileName, ParserType::XmlParser));
    m_parseWorker->moveToThread(m_thread.get());

    connect(m_thread.get(), SIGNAL(started()), m_parseWorker.get(), SLOT(process()));
    connect(m_parseWorker.get(), SIGNAL(finished()), m_thread.get(), SLOT(quit()));
    connect(m_parseWorker.get(), SIGNAL(finished()), this, SLOT(parsingFinished()));
    //    //connect(...,m_parser, SLOT(stop()));
    // проверь сигналы по статье
    //    connect(m_parseWorker, SIGNAL(finished()), m_parseWorker, SLOT(deleteLater()));
//    connect(m_thread.get(), SIGNAL(finished()), m_thread.get(), SLOT(deleteLater()));
    connect(m_parseWorker.get(), SIGNAL(errorOccured(QString)), this, SLOT(threadError(QString)));
    connect(m_parseWorker.get(), SIGNAL(errorOccured(QString)), m_thread.get(), SLOT(quit()));
//    connect(m_parseWorker.get(), SIGNAL(errorOccured(QString)), m_thread.get(), SLOT(deleteLater()));
    m_thread->start();
}

Device* DeviceFactory::createDevice(quint8 addr, quint16 id) {
    const DeviceModel& devModel = findModel(id);
    if(devModel.id == 0) {
        qDebug() << "Device model with id =" << id << "has not found!";
        return nullptr;
    } else {
//        return devModel->createDevice(addr, this);
        return new Device(addr, devModel, this);
    }
}

QStringList DeviceFactory::getBaudrate() {
    return m_baudrates;
}

const QMultiMap<uint, QString>& DeviceFactory::getCommonDeviceIDs() const {
    return m_commondDevicesId;
}


// private slots
void DeviceFactory::parsingFinished() {
    QScopedPointer<TreeItem> parserTree(m_parseWorker->data());
//    m_parseWorker->deleteLater();
    if(parseTree(*parserTree)) {
        qDebug() << "Parse tree is ok!";
    } else {
        qDebug() << "Can't parse tree";
    }
    m_parseWorker.reset();
}

void DeviceFactory::threadError(const QString& str) {
    qDebug() << "Error while parsing. " << str;
    m_thread.reset();
}

// private methods
const DeviceModel DeviceFactory::findModel(quint16 id) {
    if(m_DeviceModels.contains(id)) {
        auto model = m_DeviceModels.value(id);
        return model;
    }
    return DeviceModel();
}

bool DeviceFactory::parseTree(const TreeItem& tree) {
    if(tree.name() == "DB") {
        for(int i = 0; i < tree.childCount(); i++) {
            const TreeItem& childDb = tree.child(i);

            QString tagName = childDb.name();
            if(tagName == "BaudRate") {
                m_baudrates << parseBaudRate(childDb);
            }
            if(tagName == "CommonIDDevices") {
                m_commondDevicesId = parseCommonDevId(childDb);
            }
            if(tagName == "Device") {
                const DeviceModel& deviceModel = parseDevice(childDb);
                if(deviceModel.id != 0) // Null id is invalid
                    m_DeviceModels.insert(deviceModel.id, deviceModel);
            }

        }
        return true;
    }

    return false;
}

QString DeviceFactory::parseBaudRate(const TreeItem& item) {
    for(int i = 0; i < item.childCount(); i++) {
        const TreeItem& child = item.child(i);
        if(child.name() == "value") {
            return child.value().toString();
        }
    }
    return QString("");
}

QMultiMap<uint, QString> DeviceFactory::parseCommonDevId(const TreeItem& item) {
    QMultiMap<uint, QString> list;

    for(int i = 0; i < item.childCount(); i++) {
        const TreeItem& tagItem = item.child(i);
        QString devName = tagItem.value().toString();
        uint devId = 0;
        for(int j = 0; j < tagItem.childCount(); j++) {
            const TreeItem& attr = tagItem.child(j);
            if(attr.name() == "id") {
                devId = attr.value().toUInt();
                break;
            }
        }
        if(devId != 0)
            list.insert(devId, devName);
    }

    return list;
}

const DeviceModel DeviceFactory::parseDevice(const TreeItem& item) {
    quint16 id = 0;
    QString name = "";
    QMap<quint16, CommandSettings> cmdBuilders;
    quint16 stopDelayMs = DeviceDelays::COM_STOP_DELAY_MS;
    quint16 minCommandDelayMs = DeviceDelays::COM_COMMAND_MIN_SEND_DELAY;
    quint16 maxCommandDelayMs = DeviceDelays::COM_COMMAND_MAX_SEND_DELAY;

    bool hasId = false;
    bool hasName = false;
    bool hasCommands = false;


    for(int i = 0; i < item.childCount(); i++) {
        const TreeItem& child = item.child(i);

        if(child.name() == "id") {
            id = child.value().toString().toUInt(nullptr, 16);
            hasId = true;
        }

        if(child.name() == "name") {
            name = child.value().toString();
            hasName = true;
        }

        if(child.name() == "stopCommandDelayMs")
            stopDelayMs = child.value().toUInt();

        if(child.name() == "minCommandDelayMs")
            minCommandDelayMs = child.value().toUInt();

        if(child.name() == "maxCommandDelayMs")
            maxCommandDelayMs = child.value().toUInt();

        if(child.name() == "Commands") {
            cmdBuilders = parseCommands(child);
            if(!cmdBuilders.isEmpty()) hasCommands = true;
        }
    }
    if(hasCommands && hasId && hasName) {
        DeviceDelays delays = DeviceDelays(stopDelayMs, minCommandDelayMs, maxCommandDelayMs);
        return DeviceModel(id, name, delays, cmdBuilders);
    } else {
        return DeviceModel();
    }
}

const QMap<quint16, CommandSettings> DeviceFactory::parseCommands(const TreeItem& item) {
    quint16 code = 0;
    QString unit = "";
    double divider = 1;
    quint8 tol = 0;
    uint interval = 1;
    bool isSigned = false;
    bool isTemperature = false;

    QMap<quint16, CommandSettings> list;

    bool hasCode = false;

    for(int c = 0; c < item.childCount(); c++) {
        const TreeItem& cmd = item.child(c);

        for(int i = 0; i < cmd.childCount(); i++) {
            const TreeItem& child = cmd.child(i);

            if(child.name() == "code") {
//                code = child.value().toUInt();
                code = child.value().toString().toUInt(nullptr, 16);
                hasCode = true;
            }

            if(child.name() == "unit") {
                unit = child.value().toString();
                unit.replace("(deg)", QString::fromRawData(new QChar('\260'), 1));
//                unit.replace("(deg)", QString('\370'), Qt::CaseInsensitive); // TODO: What's the difference????
            }

            if(child.name() == "divider")
                divider = child.value().toDouble();

            if(child.name() == "tol")
                tol = child.value().toUInt();

            if(child.name() == "interval")
                interval = child.value().toUInt();

            if(child.name() == "isSigned")
                isSigned = true;

            if(child.name() == "isTemperature")
                isTemperature = true;

        }

        if(hasCode) {
            list.insert(code, CommandSettings(code, unit, divider, tol, interval, isSigned, isTemperature));
        }
    }

    return list;
}
