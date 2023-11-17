#pragma once
#include <QWidget>

#include "commandconverter.h"
#include "gui/guiinterface.h"
#include "model/device/devicewidget.h"

struct Button;

class ButtonWidget : public GuiWidgetBase {
 public:
  explicit ButtonWidget(const QString &text, const Button &button,
                        QSharedPointer<CommandConverter> converter,
                        QWidget *parent = nullptr);
  ~ButtonWidget() override;
  void setData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;

 private:
  QHBoxLayout *m_layout{nullptr};
  QSharedPointer<CommandConverter> m_converter;
  const Button &m_description;
  QVector<quint16> m_codes;
  QPushButton *m_button{nullptr};
 private slots:
  void buttonClicked();
};
