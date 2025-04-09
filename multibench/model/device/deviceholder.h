#ifndef DEVICEHOLDER_H
#define DEVICEHOLDER_H

#include <QVBoxLayout>
#include <QWidget>

#include "model/device/devicefoldedwidget.h"
#include "model/device/devicewidget.h"

class DeviceWidget;

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
 public slots:
  void setLink(bool link);
 signals:
  void nameEdited(QString name, int addr);
  void dataIncome(QSharedPointer<CommandConverter> command);
  void acceptDataFromWidget(quint16 code, quint16 value);
  void statusChanged(DeviceStatusGroup& status);
 private slots:
  void hideControlsButtonClicked(QMap<QString, bool>& widgets);
  void showWidgetButtonClicked();

 private:
  const DeviceWidgetDesc& m_description;
  QMap<quint16, QSharedPointer<CommandConverter>> m_converters;
  QVBoxLayout* m_widgetLayout;
  QPointer<DeviceWidget> m_expandedWidget;
  QPointer<DeviceFoldedWidget> m_foldedWidget;
  int m_address = 0;
  int m_id = 0;
  QString m_name;
  DeviceStatusGroup m_status;
};

#endif  // DEVICEHOLDER_H
