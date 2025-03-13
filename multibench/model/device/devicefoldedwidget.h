#ifndef DEVICEFOLDEDWIDGET_H
#define DEVICEFOLDEDWIDGET_H

#include <QWidget>

namespace Ui {
class DeviceFoldedWidget;
}

class DeviceFoldedWidget : public QWidget {
  Q_OBJECT

 public:
  explicit DeviceFoldedWidget(QWidget *parent = nullptr);
  ~DeviceFoldedWidget();
 signals:
  void showWidget();

 private:
  Ui::DeviceFoldedWidget *ui;
};

#endif  // DEVICEFOLDEDWIDGET_H
