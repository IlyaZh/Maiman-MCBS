#ifndef DEVICEPOLLREQUEST_H
#define DEVICEPOLLREQUEST_H

#include <QObject>
#include <QDebug>

class DevicePollRequest
{
public:
    DevicePollRequest(quint8 addr, quint16 code, quint8 count, quint8 interval = 1) {
        if(interval >= 1) {
            m_code = code;
            m_addr = addr;
            m_count = count;
            m_interval = interval;
            m_counter = m_interval - 1;
        } else {
            qDebug() << "DevicePollRequest interval is less than 1, value = " << interval;
        }
    }
    quint16 code() { return m_code; }
    quint8 addr() { return m_addr; }
    quint8 count() { return m_count; }
    bool isRequestReady() {
        ++m_counter;
        if(m_counter == m_interval) {
            m_counter = 0;
            return true;
        }
        return false;
    }
private:
    quint16 m_code;
    quint8 m_addr;
    quint8 m_count;
    quint8 m_counter;
    quint8 m_interval;

};

#endif // DEVICEPOLLREQUEST_H
