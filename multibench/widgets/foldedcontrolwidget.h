#ifndef FOLDEDCONTROLWIDGET_H
#define FOLDEDCONTROLWIDGET_H

#include <QWidget>

#include "gui/guiinterface.h"

namespace Ui {
class FoldedControlWidget;
}

class FoldedControlWidget : public GuiWidgetBase, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit FoldedControlWidget(QStringView name,
                               QSharedPointer<CommandConverter> Value,
                               QSharedPointer<CommandConverter> Real,
                               QWidget *parent = nullptr);
  ~FoldedControlWidget();
  void setData(quint16 code, quint16 data) override;
  QVector<quint16> Subscribe() override;
  void updateStyle() override;
  QString getName();
  bool isPinned();
  void setShown();
 public slots:
  void setPinned(bool pin);

 private:
  Ui::FoldedControlWidget *ui;
  QSharedPointer<CommandConverter> m_RealConv;
  QSharedPointer<CommandConverter> m_ValueConv;
  QVector<quint16> m_codes;
  QString m_name;
  bool m_isPinned = false;
};

#endif  // FOLDEDCONTROLWIDGET_H
