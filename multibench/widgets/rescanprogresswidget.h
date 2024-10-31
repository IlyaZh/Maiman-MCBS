#pragma once

#include <QWidget>

#include "gui/guiinterface.h"

namespace Ui {
class RescanProgressWidget;
}

class RescanProgressWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit RescanProgressWidget(QWidget *parent = nullptr);
  ~RescanProgressWidget() override;
  void updateStyle() override;

 public slots:
  void notFound();
  void setProgress(int current, int total, int success);

 private:
  Ui::RescanProgressWidget *ui;
};
