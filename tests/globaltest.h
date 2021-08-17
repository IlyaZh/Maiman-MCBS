#pragma once
#include <QObject>

class GlobalTest : public QObject
{
    Q_OBJECT
public:
    explicit GlobalTest(int argc, char* argv[], QObject *parent = nullptr);

signals:

};
