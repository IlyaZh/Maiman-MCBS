#ifndef CONNECTIONPOLLING_H
#define CONNECTIONPOLLING_H

#include <QObject>

class ConnectionPolling : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionPolling(QObject *parent = nullptr);
    void reset();
    bool needAction();
signals:

public slots:

private:
    int m_connectionPolling = 0;
    constexpr static const std::array<int, 6> m_fibonachi = {1, 1, 2, 3, 5, 8};
    int fibonachiIndex = 0;
};

#endif // CONNECTIONPOLLING_H
