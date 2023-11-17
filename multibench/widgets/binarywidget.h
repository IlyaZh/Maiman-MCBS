#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QMap>
#include <QSharedPointer>
#include <QWidget>

#include "gui/guiinterface.h"
#include "model/device/devicewidget.h"
class GuiWidgetBase;

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public GuiWidgetBase {
 public:
  explicit BinaryWidget(const Checkbox &settings,
                        QSharedPointer<DevCommand> cmd,
                        QWidget *parent = nullptr);
  explicit BinaryWidget(const Checkbox &settings,
                        QSharedPointer<CommandConverter> converter,
                        QWidget *parent = nullptr);
  ~BinaryWidget() override;
  void setData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;
 private slots:
  void checkBoxClicked(bool checked);
  // signals:
  //  void setDataFromWidget(quint16 code, quint16 data);

 private:
  Ui::BinaryWidget *ui;
  const Checkbox &m_settings;
  QSharedPointer<DevCommand> m_cmd;
  QSharedPointer<CommandConverter> m_converter;
  QVector<quint16> m_codes;

  void setValue(quint16 value);
};
#endif  // BINARYWIDGET_H
