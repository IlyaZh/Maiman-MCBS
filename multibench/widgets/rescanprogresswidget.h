#pragma once

#include <QWidget>

namespace Ui {
class RescanProgressWidget;
}

class RescanProgressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RescanProgressWidget(QWidget *parent = nullptr);
    ~RescanProgressWidget();

public slots:
    void setProgress(int current, int total);

private:
    Ui::RescanProgressWidget *ui;
};

