#include "mainwindowcontroller.h"

#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSerialPort>

MainWindowController::MainWindowController(QObject *parent) : QObject(parent)
{
    removeModel();
}

void MainWindowController::addModel(ModelInterface *model) {
    m_model = model;
}

void MainWindowController::removeModel() {
    m_model = nullptr;
}

// public slots
void MainWindowController::networkConnectClicked(CONNECT_PROTOCOL protocol, QString host, int port) {
    QTcpSocket* tcpSocket;
    QSerialPort *serialPort;

    if(m_model == nullptr) {
        // model is not assigned
        return;
    }

    if(m_model->isStart()) {
        m_model->stop();
        emit connected(false);
    } else {
        switch(protocol) {
        case TCP_PROTOCOL:
            tcpSocket = new QTcpSocket();
            tcpSocket->connectToHost(QHostAddress(host), port, QIODevice::ReadWrite);
            m_model->start(tcpSocket);
            emit connected(tcpSocket->isOpen());
            break;
        case COM_PORT_PROTOCOL:
            serialPort = new QSerialPort();
            serialPort->setPortName(host);
            serialPort->setBaudRate(port);
            serialPort->open(QIODevice::ReadWrite);
            emit connected(serialPort->isOpen());
            m_model->start(serialPort);
            break;
        case UNKNOWN_PROTOCOL:
        default:
            break;
        }
    }

}
