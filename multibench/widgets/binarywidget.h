#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QMap>
#include <QSharedPointer>
#include <QWidget>

#include "gui/guiinterface.h"
#include "model/device/devicewidget.h"
class GuiInterface;

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public QWidget, public GuiInterface {
  Q_OBJECT

 public:
  explicit BinaryWidget(const Checkbox &settings,
                        QSharedPointer<DevCommand> cmd,
                        QWidget *parent = nullptr);
  ~BinaryWidget() override;
  void getData(QSharedPointer<CommandConverter> data) override;
  QVector<quint16> Subscribe() override;
 private slots:
  void checkBoxClicked(bool checked);

 private:
  Ui::BinaryWidget *ui;
  const Checkbox &m_settings;
  QSharedPointer<DevCommand> m_cmd;

  void setValue(quint16 value);
};
#endif  // BINARYWIDGET_H
