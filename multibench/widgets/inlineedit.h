#ifndef INLINEEDIT_H
#define INLINEEDIT_H

#include <QtWidgets>

#include "gui/guiinterface.h"

class InLineEdit : public QLineEdit, public GuiWidgetInterface {
  Q_OBJECT
 public:
  explicit InLineEdit(int addr, bool isDevice = true);
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void setText(const QString& text);
  void setLink(bool link);
  void setAddress(int addr);
  QString text();
  void updateStyle() override;
 signals:
  void nameEdited(QString name, int addr);

 private slots:
  void finishedChanges();
  void checkTextLenght();

 private:
  QString m_name;
  int m_address;
  QPushButton* m_icon;
  QIcon m_darkButton;
  QIcon m_lightButton;
  QHBoxLayout* m_layout;
  bool m_isDevice = true;
  bool m_isConnected = true;
};

#endif  // INLINEEDIT_H
