#pragma once

#include <QVariant>
#include <QWidget>

#include "commandconverter.h"
#include "gui/guimediator.h"

class GuiWidgetBase : public QWidget {
  Q_OBJECT
 public:
  explicit GuiWidgetBase(QWidget *parent = nullptr) : QWidget(parent) {}
  virtual ~GuiWidgetBase() {}
  virtual void setData(quint16 code, quint16 data) = 0;
  virtual QVector<quint16> Subscribe() = 0;
 signals:
  void setDataFromWidget(quint16 code, quint16 value);
};
