#include "networkmodel.h"

#include <QDateTime>
#include <QDebug>
#include <QTcpSocket>

#include "DataThread.h"
#include "SoftProtocol.h"
#include "appsettings.h"
#include "datasource.h"
#include "device/devicemodel.h"
#include "model/device/devicepollrequest.h"
#include "model/devicefactory.h"
#include "network/IDataSource.h"
#include "protocols/modbusprotocol.h"

const quint16 NetworkModel::TIMEOUT_MS = 50 * 10;
const quint16 NetworkModel::IDENTIFY_REG_ID_DEFAULT = 0x0001;  // debug замени

NetworkModel::NetworkModel(DeviceFactory& deviceModelFactory,
                           SoftProtocol& protocol, QObject* parent)
    : QObject(parent),
      m_deviceModelFactory(deviceModelFactory),
      m_protocol(protocol) {
  m_isStart = false;
  m_deviceModelFactory.start();
  connect(&m_deviceModelFactory, &DeviceFactory::parsingIsFinished, this,
          &NetworkModel::getBaudrate);
  setDelay(AppSettings::getNetworkDelay());
  setTimeout(AppSettings::getNetworkTimeout());
}

NetworkModel::~NetworkModel() { clear(); }
// controller to model interface overrides

void NetworkModel::getBaudrate() {
  emit signal_setBaudrateToWindow(m_deviceModelFactory.getBaudrate());
}

void NetworkModel::setDelay(int delay) {
  if (m_worker) {
    m_worker->setDelay(delay);
    AppSettings::setNetworkDelay(delay);
  }
}

void NetworkModel::setTimeout(int timeout) {
  if (m_worker) {
    m_worker->setTimeout(timeout);
    AppSettings::setNetworkTimeout(timeout);
  }
}

void NetworkModel::start(QScopedPointer<IDataSource>& source) {
  m_worker = new DataThread;
  m_worker->setTimeout(AppSettings::getNetworkTimeout());
  m_worker->setDelay(AppSettings::getNetworkDelay());
  m_worker->configure(source);

  rescanNetwork();
//  pollRequest();

  connect(m_worker, &DataThread::connected, this, [this]() {
    emit signal_connected(true);
    m_isStart = true;
  });
  connect(m_worker, &DataThread::readyToWrite, this,
          &NetworkModel::pollRequest);
  connect(m_worker, &DataThread::timeout, this, &NetworkModel::timeout);
  connect(m_worker, &DataThread::errorOccured, this,
          &NetworkModel::signal_errorOccured);
  connect(m_worker, &DataThread::readyRead, this, &NetworkModel::readyRead);
  connect(m_worker, &DataThread::finished, this, [this]() {
    emit signal_connected(false);
    m_isStart = false;
    m_worker->deleteLater();
  });
  m_worker->start();
}

bool NetworkModel::isStart() { return m_isStart; }

void NetworkModel::stop() {
  if (m_worker) {
    m_worker->stop();
  }
  clear();
  m_isStart = false;
}

QMap<quint16, QSharedPointer<DevCommand>> NetworkModel::getCommands(
    quint8 addr) {
  return m_devices.value(addr)->commands();
}

void NetworkModel::rescanNetwork() {
  clear();
  QMap<quint8, quint8> addresses(
      AppSettings::getDeviceAddresses());  // TODO: dont work
  if (!AppSettings::getKeepAddresses()) {
    addresses.clear();
    clearNetwork();
  }
  if (addresses.isEmpty()) {
    for (quint8 iAddr = 1; iAddr <= AppSettings::getNetworkMaxAddress();
         ++iAddr) {
      addresses.insert(iAddr, 0);
    }
  }
  m_rescanCommandsCount = addresses.keys().size();
  emit signal_rescanProgress(0, m_rescanCommandsCount, m_successConnect);
  if (m_worker) {
    const auto& keys = addresses.keys();
    m_rescanCommandsCount = keys.size();
    for (const auto item : keys) {
      auto package =
          m_protocol.getDataValue(item, NetworkModel::IDENTIFY_REG_ID_DEFAULT);
      m_priorityQueue.enqueue(package);
    }
  }
  pollRequest();
}

void NetworkModel::clearNetwork() {
  QMap<quint8, quint8> addresses;
  AppSettings::setDeviceAddresses(addresses);
}

// private methods
void NetworkModel::clear() {
  m_priorityQueue.clear();
  m_queue.clear();
  for (auto& item : m_devices) {
    item->disconnect();
    item->deleteLater();
  }
  m_devices.clear();
  m_rescanCommandsCount = 0;
  m_rescanCommandsDone = 0;
  m_successConnect = 0;
}

void NetworkModel::initDevice(quint8 addr, quint16 id) {
  qInfo() << "Init device " << addr << " with id " << id;
  QSharedPointer<Device> newDevice =
      m_deviceModelFactory.createDevice(addr, id);
  if (newDevice.isNull()) {
    qWarning() << "Device with id " << id << "hasn't found in config";
    return;
  }

  if (m_devices.contains(addr)) {
    auto& oldDev = m_devices[addr];
    oldDev->disconnect();
    oldDev.reset();
    m_devices.remove(addr);
  }

  m_devices.insert(addr, newDevice);
  m_successConnect++;
  QMap<quint8, quint8> addresses(AppSettings::getDeviceAddresses());
  addresses.insert(addr, 0);
  AppSettings::setDeviceAddresses(addresses);

  connect(newDevice.get(), SIGNAL(dataToModel(quint8, quint16, quint16)), this,
          SLOT(dataOutcome(quint8, quint16, quint16)));

  emit signal_createWidgetFor(newDevice.get());
}

// public slots
// modbus interface overrides

void NetworkModel::dataOutcome(quint8 addr, quint16 reg, quint16 value) {
  if (m_worker) {
    auto package = m_protocol.setDataValue(addr, reg, value);
    m_priorityQueue.enqueue(package);
  }
}

void NetworkModel::temperatureUnitsChanged(Const::TemperatureUnitId id) {
  for (auto& device : m_devices) {
    device->changeTemperatureUnit(id);
  }
}

// private slots
void NetworkModel::timeout(const QByteArray& lastPackage) {
  if (m_worker) {
    if (!lastPackage.isEmpty()) {
      auto devAddr = static_cast<quint8>(lastPackage.at(0));
      auto device = m_devices.value(devAddr);
      if (device) {
        qWarning() << "Unlink device" << device->addr();
        device->unlink();
        m_disconnectedDevices.insert(devAddr);
      }
    }
    if (m_isRescan) {
      emit signal_rescanProgress(++m_rescanCommandsDone, m_rescanCommandsCount,
                                 m_successConnect);
      m_isRescan = false;
    }
  }
}

void NetworkModel::pollRequest() {
  if (m_worker) {
    QByteArray package;
    // poll devices state

    if (!m_priorityQueue.isEmpty()) {
      package = m_priorityQueue.dequeue();

      if (m_rescanCommandsDone < m_rescanCommandsCount) {
        m_isRescan = true;
      }
    } else if (!m_queue.isEmpty()) {
      package = m_queue.dequeue();
    } else {
      for (const auto& dev : qAsConst(m_devices)) {
        const auto& is_disconnected_device =
            m_disconnectedDevices.contains(dev->addr());
        const auto request = dev->nextPollRequest(!is_disconnected_device);
        if (request.has_value()) {
          auto wrotePack = m_protocol.getDataValue(request->addr, request->code,
                                                   request->count);
          m_queue.enqueue(wrotePack);
        }
      }
      if (!m_queue.isEmpty() && m_isStart) {
        package = m_queue.dequeue();
      }
    }
    if (!package.isEmpty()) {
      qint64 waitForBytes = m_protocol.waitForBytes(package);
      bool isDisconnected =
          m_disconnectedDevices.contains(static_cast<quint8>(package.at(0)));
      m_worker->writeAndWaitBytes(package, waitForBytes, isDisconnected);
    }
    else if(package.isEmpty() and m_successConnect > 0) emit signal_emptyNetwork();
  }
}

void NetworkModel::readyRead(const QByteArray& rxPackage,
                             const QByteArray& lastPackage) {
  auto result = m_protocol.execute(rxPackage, lastPackage);
  if (m_protocol.isError()) {
    qWarning() << "PROTOCOL ERROR" << m_protocol.errorString();
    // INFO: Считаем устройство пропащим только при таймауте.
    // TODO: Может тут сообщение выводить?
    //        quint8 addr = static_cast<quint8>(rxPackage.at(0));
    //        if(m_devices.contains(addr)) {
    //            m_devices[addr]->unlink();
    //        }
  } else {
    for (const auto& item : qAsConst(result)) {
      if (item.reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT and !m_devices.contains(item.addr)) {
        initDevice(item.addr, item.value);
      } else {
        if (m_devices.contains(item.addr)) {
            if(item.reg == NetworkModel::IDENTIFY_REG_ID_DEFAULT){
                m_devices[item.addr]->setReceivedID(item.value);
                if(m_devices[item.addr]->id() != item.value){
                    m_devices[item.addr]->unlink();
                    m_disconnectedDevices.insert(item.addr);
                }
            }
            m_devices[item.addr]->dataIncome(item.reg, item.value);
          if (m_disconnectedDevices.contains(item.addr))
            m_disconnectedDevices.remove(item.addr);
        }
      }
    }
  }
  if (m_isRescan) {
    emit signal_rescanProgress(++m_rescanCommandsDone, m_rescanCommandsCount,
                               m_successConnect);
    m_isRescan = false;
  }
}
