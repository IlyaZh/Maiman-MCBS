#include "groupwidget.h"

#include <QPointer>

#include "ui_groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GroupWidget) {
  ui->setupUi(this);
}

GroupWidget::~GroupWidget() { delete ui; }

void GroupWidget::addGroupMember(QSharedPointer<GroupInterface> member) {
  if (member == this) return;
}

void GroupWidget::removeGroupMember(QSharedPointer<GroupInterface> member) {
  if (member == this) return;
}
