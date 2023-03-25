#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QWidget>

#include "constants.h"

namespace Ui {
class ConnectionWidget;
}  // namespace Ui

class ConnectionWidget : public QWidget {
  Q_OBJECT

 public:
  explicit ConnectionWidget(QWidget* parent = nullptr);
  ~ConnectionWidget() override;

  void setBaudList(const QStringList& baudrateList);
  void setPortList(const QStringList& portList);
  void setCurrentComPort(QString port);
  void setCurrentBautRate(QString baudrate);
  void setProtocol(Const::PortType type);
  void setCurrentIp(QString ip);
  void setCurrentTcpPort(int port);

  void setConnectMessage(QStringView msg);
  void setConnected(bool isConnected);

  QString getCurrentIp();
  int getCurrentTcpPort();
 private slots:
  void connectClicked(Const::PortType type);

 signals:
  void changeConnectState(Const::PortType type, QVariantMap value);
  void refreshComPorts();
  void connectToCOM();
  void connectToTCP();

  void comPortIsChanged(QString port);
  void baudRateIsChanged(QString port);

 private:
  Ui::ConnectionWidget* ui;
  void paintEvent(QPaintEvent*) override;
};

#endif  // CONNECTIONWIDGET_H
