#pragma once

#include <QWidget>

namespace Ui {
class RescanProgressWidget;
}

class RescanProgressWidget : public QWidget {
  Q_OBJECT

 public:
  explicit RescanProgressWidget(QWidget *parent = nullptr);
  ~RescanProgressWidget() override;

 public slots:
  void notFound();
  void setProgress(int current, int total, int success);

 private:
  Ui::RescanProgressWidget *ui;
};
