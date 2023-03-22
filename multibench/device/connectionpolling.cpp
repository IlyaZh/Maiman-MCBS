#include "connectionpolling.h"
#include <QDebug>
ConnectionPolling::ConnectionPolling(QObject *parent) : QObject(parent)
{

}

bool ConnectionPolling::needAction(){
    m_connectionPolling++;
    if(m_fibonachi.at(fibonachiIndex) == m_connectionPolling){
        fibonachiIndex++;
        fibonachiIndex = (fibonachiIndex >= 5 ) ? 5 : fibonachiIndex;
        m_connectionPolling = 0;
        return true;
    }
        return false;
}

void ConnectionPolling::reset() {
    fibonachiIndex = 0;
    m_connectionPolling = 0;
}
