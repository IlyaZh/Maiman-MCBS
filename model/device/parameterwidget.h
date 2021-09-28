#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QLabel>

struct Control;
class DevCommand;

namespace Ui {
class ParameterWidget;
}

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterWidget(const Control& settings, QWidget *parent = nullptr);
    ~ParameterWidget();
    const Control& settings();
    void setValue(quint16 cmd, double value);
    void setValue(quint16 cmd, int value);
    void setUnit(const QString& unit);
    void setDecimals(quint16 cmd, int decimals);

signals:
    void valueChanged(quint16 param, double value);

private:
    Ui::ParameterWidget *ui;
    const Control& m_settings;
    QString m_name;
    QMap<quint16, int> m_decimals;
    QMap<quint16, double> m_values;
    QMap<quint16, DevCommand*> m_commands;
//    MaimanSpinBox* valueSpinbox;
    QString m_unit;

private slots:
    void spinBoxEditingFinished();
};

#endif // PARAMETERWIDGET_H
