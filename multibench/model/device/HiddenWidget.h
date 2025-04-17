#pragma once

#include <QScopedPointer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "gui/guiinterface.h"

enum WidgetInside { None = 0, Control = 1, Binary = 2, Read = 3 };

namespace Ui {
class HiddenWidget;
}  // namespace Ui

class HiddenWidget : public QWidget, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit HiddenWidget(QWidget* parent = nullptr);
  ~HiddenWidget() override;
  void addWidget(QWidget* widget);
  bool isShown() const;
  void setShown(bool show);
  bool isPinned() const;
  void setPinned(bool pin);
  void setMargins(int left, int top, int right, int bottom);
  void setSpacerSize(int w, int h,
                     QSizePolicy::Policy hData = QSizePolicy::Minimum,
                     QSizePolicy::Policy vData = QSizePolicy::Minimum);
  void defineWidget(WidgetInside inside);
  WidgetInside whichWWidgetInside();
  void updateStyle() override;
 signals:
  void pinned(bool pin);

 private:
  Ui::HiddenWidget* ui;
  QVector<QWidget*> m_widgets;
  bool m_isVisible{true};
  bool m_isPinned{false};
  QVBoxLayout* m_layout{nullptr};
  QMargins m_margin;
  WidgetInside m_inside = None;
};
