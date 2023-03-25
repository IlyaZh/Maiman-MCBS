#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QMap>
#include <QSharedPointer>
#include <QWidget>

#include "model/device/devicewidget.h"

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BinaryWidget(const Checkbox &settings,
                        QSharedPointer<DevCommand> cmd,
                        QWidget *parent = nullptr);
  ~BinaryWidget() override;

 private slots:
  void checkBoxClicked(bool checked);

 private:
  Ui::BinaryWidget *ui;
  const Checkbox &m_settings;
  QSharedPointer<DevCommand> m_cmd;

  void setValue(quint16 value);
};
#endif  // BINARYWIDGET_H
