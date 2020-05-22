#include "mainwindow.h"

#include <QApplication>
#include <QSerialPort>
#include "modbus/modbus.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QSerialPort comPort;

    Modbus modbus(&comPort, "ComPort", Modbus::TIMEOUT_DEFAULT);

    return a.exec();
}
