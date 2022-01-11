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

//namespace Ui {
//class ReadParameterWidget;
//}

class ReadParameterWidget : public QWidget
{
    Q_OBJECT

public:
//    explicit ReadParameterWidget(QStringView name, QSharedPointer<DevCommand> cmd, /*int value, QString name,*/ QWidget *parent = nullptr);
//    explicit ReadParameterWidget(const Control &settings, int decimal, QString name, /*double value,*/ QWidget *parent = nullptr);
    ReadParameterWidget();
    virtual ~ReadParameterWidget() = default;

    virtual void setup(QStringView name, QSharedPointer<DevCommand> cmd);
    int getUnitslength();
    void setUnitsLength(int length);
protected:
//    Ui::ReadParameterWidget *ui;
//    const Control& m_settings;
    QString m_unit;
    QSharedPointer<DevCommand> m_command;
    QHBoxLayout* m_layout{nullptr};
    QLabel* m_labelParameter{nullptr};
    QLabel* m_labelValue{nullptr};
    QLabel* m_labelUnit{nullptr};

    void setValue(double value, int decimal);
    void setValue(int value);
    void setUnit(QStringView unit);



#ifdef QT_DEBUG
friend class TestDeviceParameter;
#endif
};

#ifdef QT_DEBUG
//class TestDeviceParameter : public QObject
//{
//    Q_OBJECT
//public:
//    explicit TestDeviceParameter(QObject *parent = nullptr);
//    ~TestDeviceParameter();
//private slots:
//    void testLabel();
//private:
//    QScopedPointer<Control> settings;
//    QScopedPointer<ReadParameterWidget> widget;
//};
#endif


#endif // READPARAMETERWIDGET_H
