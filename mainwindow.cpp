#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setConnections();

    refreshMenuPortList();
    refreshMenuPortBaudsList();

    emit mainWindowReady();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// private methods
void MainWindow::setConnections() {
    // network connection button
    connect(ui->networkConnectButton, &QPushButton::click, [=]{
        emit this->networkConnectButtonSignal();
    });
}

void MainWindow::refreshMenuPortList() {

}

void MainWindow::refreshMenuPortBaudsList() {

}
