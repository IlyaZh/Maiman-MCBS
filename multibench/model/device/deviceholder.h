#ifndef DEVICEHOLDER_H
#define DEVICEHOLDER_H

#include <QVBoxLayout>
#include <QWidget>

#include "model/device/devicefoldedwidget.h"
#include "model/device/devicewidget.h"

class DeviceWidget;
class ControlWidget;
class BinaryWidget;
class HiddenWidget;
class DeviceCondition;
class ButtonWidget;
class CommandConverter;
class FoldedControlWidget;

class DeviceHolder : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit DeviceHolder(
      const DeviceWidgetDesc& description,
      const QMap<quint16, QSharedPointer<CommandConverter>>& converters,
      QWidget* parent = nullptr);
  ~DeviceHolder();
  void setAddress(int addr);
  int getAddress() const;
  int getId() const;
  QString getModel() const;
  QString getName() const;
  void setConstraint(bool state);
  void updateValue(const model::Event& event);
  void updateStyle() override;
  void setDevicesStatus(quint8 addr, QSharedPointer<DeviceStatusGroup> desc);
  bool isHide();
  QSize m_widgetSize;
 public slots:
  void setLink(bool link);
  void hideControlsButtonClicked();
  void showWidgetButtonClicked();
 signals:
  void nameEdited(QString name, int addr);
  void dataIncome(QSharedPointer<CommandConverter> command);
  void acceptDataFromWidget(quint16 code, quint16 value);
  void statusChanged(DeviceStatusGroup& status);
  void hideStatus(bool state);

 private:
  const DeviceWidgetDesc& m_description;
  QMap<quint16, QSharedPointer<CommandConverter>> m_converters;
  QVBoxLayout* m_widgetLayout;
  QStackedLayout* m_stacked;
  QPointer<DeviceWidget> m_expandedWidget;
  QPointer<DeviceFoldedWidget> m_foldedWidget;
  QVector<ButtonWidget*> m_buttonWidgets;
  QVector<BinaryWidget*> m_binaryWidgets;
  DeviceCondition* m_deviceCondition;
  QSet<GuiWidgetBase*> m_widgetsTable;
  QVector<HiddenWidget*> m_widgets;
  QVector<FoldedControlWidget*> m_foldedWidgets;
  int m_address = 0;
  int m_id = 0;
  QString m_name;
  DeviceStatusGroup m_status;

  bool m_isHide = false;
};

#endif  // DEVICEHOLDER_H
