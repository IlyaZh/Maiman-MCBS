#ifndef CONNECTIONPOLLING_H
#define CONNECTIONPOLLING_H

#include <QObject>

class ConnectionPolling : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionPolling(QObject *parent = nullptr);
    bool get();
    void setConnectionState(bool state);
    bool isDisconnected();
    bool isRequestReady();
signals:

public slots:

private:
    void fibonachiCount();
    int m_F0=0;
    int m_F1=1;
    int m_connectionPolling = 0;
    bool m_isDisconnected = false;
    uint counter;
    const uint m_intervalMs = 100;
};

#endif // CONNECTIONPOLLING_H
