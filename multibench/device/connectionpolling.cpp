#include "connectionpolling.h"
#include <QDebug>
ConnectionPolling::ConnectionPolling(QObject *parent) : QObject(parent)
{

}

void ConnectionPolling::fibonachiCount(){
    int fibonachi=0;
    fibonachi = m_F0 + m_F1;
    if(fibonachi > 8) fibonachi = 8;
    m_F0 = m_F1;
    m_F1 = fibonachi;
}

bool ConnectionPolling::get(){
    m_connectionPolling++;
    if(m_connectionPolling == m_F1){
        fibonachiCount();
        m_connectionPolling = 0;
        return true;
    }
        return false;
}

void ConnectionPolling::setConnectionState(bool state){
    if(m_isDisconnected != state){
        m_isDisconnected = state;
        if(!m_isDisconnected){
            m_connectionPolling = 0;
            m_F0 = 0;
            m_F1 = 1;
        }
    }
}

bool ConnectionPolling::isDisconnected(){
    return m_isDisconnected;
}

bool ConnectionPolling::isRequestReady() {
    ++counter;
    if(counter >= m_intervalMs) {
        counter = 0;
        return true;
    }
    return false;
}
