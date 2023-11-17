#include "controlwidget.h"

#include "ui_controlwidget.h"

enum { ErrorTimeout = 1000 };

ControlWidget::ControlWidget(QStringView name, QSharedPointer<DevCommand> Value,
                             QSharedPointer<DevCommand> Max,
                             QSharedPointer<DevCommand> Min,
                             QSharedPointer<DevCommand> Real, QWidget *parent)
    : GuiWidgetBase(parent),
      ui(new Ui::CommandWidget),
      m_Real(Real),
      m_Value(Value),
      m_Max(Max),
      m_Min(Min) {
  ui->setupUi(this);

  ui->WidgetName->setText(name.toString());

  if (m_Value and m_Max and m_Min) {
    m_Validator = new QDoubleValidator(
        m_Min->valueDouble(), m_Max->valueDouble(), m_Value->tolerance());

    setMinValue();
    setMaxValue();
    setValue();
    if (m_Real.isNull()) {
      ui->Real->setText("Set");
      setRealValue();
    } else {
      ui->Real->setText("Real");
      setRealValue(/*m_Real->valueDouble(),m_Real->tolerance()*/);
    }
    setUnits(m_Value->unit());

    connect(m_Value.get(), &DevCommand::updatedValue, this,
            &ControlWidget::setValue);

    connect(m_Value.get(), &DevCommand::updatedUnit, this,
            &ControlWidget::setUnits);
    setUnits(m_Value->unit());
    connect(m_Min.get(), &DevCommand::updatedValue, this,
            &ControlWidget::setMinValue);
    connect(m_Max.get(), &DevCommand::updatedValue, this,
            &ControlWidget::setMaxValue);
  }

  if (m_Real) {
    connect(m_Real.get(), &DevCommand::updatedValue, this,
            &ControlWidget::setRealValue);
  }

  connect(ui->Value, &QLineEdit::returnPressed, this,
          &ControlWidget::userEnteredValue);
  connect(ui->Value, &QLineEdit::cursorPositionChanged, this,
          [this]() { isUserEdit = true; });
  adjust();
}

ControlWidget::ControlWidget(QStringView name,
                             QSharedPointer<CommandConverter> Value,
                             QSharedPointer<CommandConverter> Max,
                             QSharedPointer<CommandConverter> Min,
                             QSharedPointer<CommandConverter> Real,
                             QWidget *parent)
    : GuiWidgetBase(parent),
      ui(new Ui::CommandWidget),
      m_RealConv(Real),
      m_ValueConv(Value),
      m_MaxConv(Max),
      m_MinConv(Min) {
  ui->setupUi(this);

  ui->WidgetName->setText(name.toString());

  if (m_ValueConv and m_MaxConv and m_MinConv) {
    m_Validator =
        new QDoubleValidator(m_MinConv->valueDouble(), m_MaxConv->valueDouble(),
                             m_ValueConv->tolerance());
    if (m_RealConv.isNull()) {
      ui->Real->setText("Set");
    } else {
      ui->Real->setText("Real");
    }
    setUnits(m_ValueConv->unit());
  }

  connect(ui->Value, &QLineEdit::returnPressed, this,
          &ControlWidget::userEnteredData);
  connect(ui->Value, &QLineEdit::cursorPositionChanged, this,
          [this]() { isUserEdit = true; });
  adjust();
}

ControlWidget::~ControlWidget() { delete ui; }

void ControlWidget::userEnteredValue() {
  double valueFromLine = ui->Value->text().toDouble();
  ui->Value->setText(QString::number(valueFromLine, 'f', m_Value->tolerance()));
  ui->Value->clearFocus();

  if (valueFromLine > m_Max->valueDouble() or
      valueFromLine < m_Min->valueDouble()) {
    valueFromLine =
        qBound(m_Min->valueDouble(), valueFromLine, m_Max->valueDouble());
    setEditLineRed();
    QTimer::singleShot(ErrorTimeout, this, SLOT(setEditLineWhite()));
  }

  if (!m_Value.isNull()) {
    m_Value->setFromWidget(valueFromLine);
  }

  isUserEdit = false;
}

void ControlWidget::userEnteredData() {
  double valueFromLine = ui->Value->text().toDouble();
  ui->Value->setText(
      QString::number(valueFromLine, 'f', m_ValueConv->tolerance()));
  ui->Value->clearFocus();

  if (valueFromLine > m_MaxConv->valueDouble() or
      valueFromLine < m_MinConv->valueDouble()) {
    valueFromLine = qBound(m_MinConv->valueDouble(), valueFromLine,
                           m_MaxConv->valueDouble());
    setEditLineRed();
    QTimer::singleShot(ErrorTimeout, this, SLOT(setEditLineWhite()));
  }

  if (!m_ValueConv.isNull()) {
    emit setDataFromWidget(
        m_ValueConv->code(),
        m_ValueConv->getRawFromValue(static_cast<double>(valueFromLine)));
    // m_ValueConv->setFromWidget(valueFromLine);
  }

  isUserEdit = false;
}

void ControlWidget::setValue() {
  const QString value = m_Value->valueStr();
  if (!isUserEdit) {
    if (m_Value) {
      ui->Value->setText(value);
    }
  }
  if (m_Real.isNull()) {
    ui->RealValue->setText(value);
  }
}

void ControlWidget::setUnits(QStringView units) {
  ui->RealUnits->setText(units.toString());
  ui->MaxUnits->setText(units.toString());
  ui->MinUnits->setText(units.toString());
}

void ControlWidget::setMaxValue() {
  ui->MaxValue->setText(m_Max->valueStr());
  m_Validator->setTop(m_Max->valueDouble());
}

void ControlWidget::setMinValue() {
  ui->MinValue->setText(m_Min->valueStr());
  m_Validator->setBottom(m_Min->valueDouble());
}

void ControlWidget::setRealValue() {
  if (m_Real) ui->RealValue->setText(m_Real->valueStr());
}

// private method
void ControlWidget::adjust() {
  this->adjustSize();
  this->setMinimumSize(this->size());
}

void ControlWidget::setEditLineRed() {
  ui->Value->setStyleSheet(
      "  color: rgb(255, 255, 255);\n"
      "	background: rgb(230, 0, 0);\n"
      "	border-radius: 5px;\n");
}

void ControlWidget::setEditLineWhite() {
  ui->Value->setStyleSheet(
      "  color: rgb(0, 0, 0);\n"
      "	background: rgb(255, 255, 255);\n"
      "	border-radius: 5px;\n");
  const QString value = m_Value->valueStr();
  ui->Value->setText(value);
}

void ControlWidget::setData(quint16 code, quint16 data) {
  setUnits(m_ValueConv->unit());
  if (code == m_ValueConv->code()) {
    m_ValueConv->setValue(data);
    const QString value = m_ValueConv->valueStr();
    if (!isUserEdit) {
      if (m_ValueConv) {
        ui->Value->setText(value);
      }
    }
    if (m_RealConv.isNull()) {
      ui->RealValue->setText(value);
    }
  } else if (code == m_MaxConv->code()) {
    m_MaxConv->setValue(data);
    ui->MaxValue->setText(m_MaxConv->valueStr());
    m_Validator->setTop(m_MaxConv->valueDouble());
  } else if (code == m_MinConv->code()) {
    m_MinConv->setValue(data);
    ui->MinValue->setText(m_MinConv->valueStr());
    m_Validator->setBottom(m_MinConv->valueDouble());
  } else if (code == m_RealConv->code()) {
    m_RealConv->setValue(data);
    if (m_RealConv) ui->RealValue->setText(m_RealConv->valueStr());
  }
}

QVector<quint16> ControlWidget::Subscribe() {
  m_codes.append(m_ValueConv->code());
  m_codes.append(m_MaxConv->code());
  m_codes.append(m_MinConv->code());
  if (m_RealConv) m_codes.append(m_RealConv->code());
  return m_codes;
}
