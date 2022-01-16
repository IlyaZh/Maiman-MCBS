#pragma once

#include <QWidget>
#include <QVector>
#include <QVBoxLayout>
#include <QScopedPointer>

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
    bool isShown() const;
    void setShown(bool show);
    bool isPinned() const;
    void setPinned(bool pin);


private:
    Ui::HiddenWidget *ui;
    QVector<QWidget*> m_widgets;
    bool m_isVisible {true};
    bool m_isPinned {false};
    QVBoxLayout* m_layout;
};

