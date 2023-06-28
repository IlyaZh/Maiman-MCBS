#pragma once

#include <QScopedPointer>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

namespace Ui {
class HiddenWidget;
}  // namespace Ui

class HiddenWidget : public QWidget {
  Q_OBJECT

 public:
  explicit HiddenWidget(QWidget* parent = nullptr);
  ~HiddenWidget() override;
  void addWidget(QWidget* widget);
  bool isShown() const;
  void setShown(bool show);
  bool isPinned() const;
  void setPinned(bool pin);

 private:
  Ui::HiddenWidget* ui;
  QVector<QWidget*> m_widgets;
  bool m_isVisible{true};
  bool m_isPinned{false};
  QVBoxLayout* m_layout{nullptr};
};
