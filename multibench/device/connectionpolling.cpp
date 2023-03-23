#include "connectionpolling.h"
#include <QDebug>
ConnectionPolling::ConnectionPolling(QObject *parent) : QObject(parent)
{

}

bool ConnectionPolling::isNeedAction(){
    counter++;
    if(m_fibonachi.at(idx) == counter){
        if(idx < m_fibonachi.size()) { ++idx; }
        counter = 0;
        return true;
    }
        return false;
}

void ConnectionPolling::reset() {
    idx = 0;
    counter = 0;
}
