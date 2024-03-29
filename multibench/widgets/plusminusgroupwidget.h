#pragma once

#include <QObject>
#include <QtCore>
#include <QtWidgets>

class PlusMinusWidget;

class PlusMinusGroupWidget : public QWidget {
  Q_OBJECT
 public:
  explicit PlusMinusGroupWidget(PlusMinusWidget* min, PlusMinusWidget* max,
                                QWidget* parent = nullptr);

 signals:

 private:
  PlusMinusWidget* m_minWidget;
  PlusMinusWidget* m_maxWidget;
  QVBoxLayout* m_layout;
};
