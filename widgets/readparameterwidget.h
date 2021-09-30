#ifndef READPARAMETERWIDGET_H
#define READPARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTest>
#include <QSharedPointer>

struct Control;
class DevCommand;

namespace Ui {
class ReadParameterWidget;
}

class ReadParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReadParameterWidget(const Control &settings, QSharedPointer<DevCommand> cmd, /*int value, QString name,*/ QWidget *parent = nullptr);
//    explicit ReadParameterWidget(const Control &settings, int decimal, QString name, /*double value,*/ QWidget *parent = nullptr);
    ~ReadParameterWidget();
    void setValue(quint16 cmd, double value, int decimal);
    void setValue(quint16 cmd, int value);
    void setUnits(const QString& unit);
    void setDecimals(quint16 cmd, int decimals);

private:
    Ui::ReadParameterWidget *ui;
    const Control& m_settings;
    QMap<quint16, int> m_decimals;
    QMap<quint16, double> m_values;
    QString m_unit;
    QSharedPointer<DevCommand> m_command;


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
