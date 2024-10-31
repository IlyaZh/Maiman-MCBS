#pragma once

#include <QtWidgets>

#include "gui/guiinterface.h"

class InLineControl : public QLineEdit, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit InLineControl();
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void setText(const QString& text);
  void setActiveStyle();
  void setInactiveStyle();
  void setErrorStyle();
  QString text();
  void updateStyle() override;
 signals:
  void nameEdited(QString name, int addr);

 public slots:
  void finishedChanges();

 private:
  QString m_value = "0";
  QPushButton* m_icon;
  QHBoxLayout* m_layout;
};
