#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QInputDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include <QTest>

struct Control;

namespace Ui {
class ParameterWidget;
}

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterWidget(const Control &settings, QWidget *parent = nullptr);
    ~ParameterWidget();
    const Control& settings();
    void setValue(quint16 cmd, double value);
    void setUnit(const QString& unit);
    void setDecimals(quint16 cmd, int decimals);
signals:
    void valueChanged(quint16 param, double value);


private:
    Ui::ParameterWidget *ui;
    const Control& m_settings;
    QMap<quint16, int> m_decimals;
    QMap<quint16, double> m_values;
//    MaimanSpinBox* valueSpinbox;
    QString m_unit;
    QLabel *doubleLabel;
    QLabel *textLabel;
    QInputDialog m_dialog;

private slots:
    void setButtonPressed();
#ifdef QT_DEBUG
friend class TestWidget;
#endif
};

class TestWidget : public QObject
{
    Q_OBJECT
public:
    explicit TestWidget(QObject *parent = nullptr);
    ~TestWidget();
private slots:
    void testValuesInt();
    void testValuesDouble();
private:
    QScopedPointer<Control> settings;
    QScopedPointer<ParameterWidget> widget;
};

#endif // PARAMETERWIDGET_H
