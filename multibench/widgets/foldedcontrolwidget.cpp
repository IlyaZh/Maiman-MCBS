#include "foldedcontrolwidget.h"

#include "ui_foldedcontrolwidget.h"

FoldedControlWidget::FoldedControlWidget(QStringView name,
                                         QSharedPointer<CommandConverter> Value,
                                         QSharedPointer<CommandConverter> Real,
                                         QWidget *parent)
    : GuiWidgetBase(parent),
      ui(new Ui::FoldedControlWidget),
      m_RealConv(Real),
      m_ValueConv(Value) {
  ui->setupUi(this);
  m_name = name.toString();
  ui->nameLabel->setText(QString("%1:").arg(name.toString()));
  QFont font10("Poppins", 10);
  font10.setLetterSpacing(QFont::PercentageSpacing, 105);
  font10.setPixelSize(14);
  ui->nameLabel->setFont(font10);
  ui->realLabel->setFont(font10);
  ui->setLabel->setFont(font10);
  if (m_RealConv.isNull()) {
    ui->realLabel->setVisible(false);
  } else {
    ui->realLabel->setVisible(true);
  }
}

FoldedControlWidget::~FoldedControlWidget() { delete ui; }

QString FoldedControlWidget::getName() { return m_name; }

void FoldedControlWidget::setData(quint16 code, quint16 data) {
  if (code == m_ValueConv->code()) {
    m_ValueConv->setValue(data);
    const QString value = m_ValueConv->valueStr();
    ui->setLabel->setText(
        QString("Set: %1 %2").arg(value, m_ValueConv->unit()));
    if (m_RealConv.isNull()) {
      ui->setLabel->setText(QString("%1 %2").arg(value, m_ValueConv->unit()));
    }
  } else if (code == m_RealConv->code()) {
    m_RealConv->setValue(data);
    if (m_RealConv)
      ui->realLabel->setText(
          QString("Real :%1 %2")
              .arg(m_RealConv->valueStr(), m_ValueConv->unit()));
  }
}

QVector<quint16> FoldedControlWidget::Subscribe() {
  m_codes.append(m_ValueConv->code());
  if (m_RealConv) m_codes.append(m_RealConv->code());
  return m_codes;
}

void FoldedControlWidget::updateStyle() { this->update(); }
