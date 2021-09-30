#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTest>
#include "model/device/devicewidget.h"

namespace Ui {
class BinaryWidget;
}

class BinaryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BinaryWidget(const Checkbox &settings, QWidget *parent = nullptr);
    ~BinaryWidget();

signals:
    void valueChanged(quint16 cmd, quint8 bit, bool state);

private slots:
    void checkBoxClicked();

private:
    Ui::BinaryWidget *ui;
    const Checkbox m_settings;

};
#endif // BINARYWIDGET_H
