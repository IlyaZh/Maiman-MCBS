#include "tcpdatasource.h"

TcpDataSource::TcpDataSource() :
    m_device(new QTcpSocket()),
    m_host(QString()),
    m_port(0)
{

}

void TcpDataSource::init(const QVariantMap& portSettings) {
    auto host = portSettings["host"];
    auto port = portSettings["port"];
    if(host.canConvert(QMetaType::QString) && port.canConvert(QMetaType::Int)) {
        m_host = host.toString();
        m_port = port.toInt();
    }
}

bool TcpDataSource::open() {
    return m_dev
}

QIODevice* TcpDataSource::device() {
    return m_device.get();
}
