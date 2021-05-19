#ifndef BINARYWIDGET_H
#define BINARYWIDGET_H

#include <devicebinaryoption.h>

class BinaryWidget
{
public:
    explicit BinaryWidget(DeviceBinaryOption* option);
    QWidget widget();

private slots:
    void buttonClicked(bool newState);

private:
    DeviceBinaryOption* option;
    QAbstractButton* button;
};

#endif // BINARYWIDGET_H
