#pragma once

#include <QWidget>
#include <QVector>
#include <QVBoxLayout>

namespace Ui {
class HiddenWidget;
}

class HiddenWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HiddenWidget(QWidget *parent = nullptr);
    ~HiddenWidget();
    void addWidget(QWidget* widget);
    QVector<QWidget*> widgets();
    bool isVisible();
    void setVisible(bool show);
    bool isPinned();
    void setPinned(bool pin);


private:
    Ui::HiddenWidget *ui;
    QVector<QWidget*> m_widgets; // TODO: Сделай нормальный тип
    bool m_isVisible = true;
    bool m_isPinned = true;
    QVBoxLayout* m_layout;
};

