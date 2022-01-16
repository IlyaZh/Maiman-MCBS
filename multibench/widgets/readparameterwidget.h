#ifndef READPARAMETERWIDGET_H
#define READPARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTest>
#include <QSharedPointer>
#include <QLabel>
#include <QLayout>
#include <QSpacerItem>

struct Control;
class DevCommand;

class ReadParameterWidget : public QWidget
{
    Q_OBJECT

public:
    ReadParameterWidget();
    virtual ~ReadParameterWidget() = default;

    virtual void setup(QStringView name, QSharedPointer<DevCommand> cmd);
    int getUnitslength();
    void setUnitsLength(int length);
protected:
    QString m_unit;
    QSharedPointer<DevCommand> m_command;
    QHBoxLayout* m_layout{nullptr};
    QLabel* m_labelParameter{nullptr};
    QLabel* m_labelValue{nullptr};
    QLabel* m_labelUnit{nullptr};

    void setValue(double value, int decimal);
    void setValue(int value);
    void setUnit(QStringView unit);
};


#endif // READPARAMETERWIDGET_H
