#ifndef INLINEEDIT_H
#define INLINEEDIT_H

#include <QtWidgets>

class InLineEdit : public QLineEdit {
  Q_OBJECT
 public:
  explicit InLineEdit(int addr, bool isDevice = true);
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void setText(const QString& text);
  QString text();
 signals:
  void nameEdited(QString name, int addr);

 private slots:
  void finishedChanges();
  void checkTextLenght();

 private:
  QString m_name;
  int m_address;
  QPushButton* m_icon;
  QHBoxLayout* m_layout;
  bool m_isDevice = true;
};

#endif  // INLINEEDIT_H
