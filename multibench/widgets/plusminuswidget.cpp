#include "plusminuswidget.h"

#include "device/commandsettings.h"
#include "model/device/devicewidget.h"
#include "ui_calibrationandlimitswidget.h"

const QString PlusMinusWidget::styleSheetOK{
    "\
    QLineEdit {\
        font: 16pt Share Tech Mono;\
        color: rgb(16, 33, 40);\
        background: rgb(255, 255, 255);\
        border-radius: 5px;\
        padding: 5px 0;\
    }\
    QLineEdit::disabled {\
        background: rgb(255, 255, 255);\
        color: rgb(76, 93, 100);\
        border-radius: 5px;\
        padding: 5px 0;\
    }"};

const QString PlusMinusWidget::styleSheetERROR{
    "\
    QLineEdit {\
        font: 16pt Share Tech Mono;\
        color: rgb(255, 0, 0);\
        background: rgb(255, 255, 255);\
        border-radius: 5px;\
        padding: 5px 0;\
}\
    QLineEdit::disabled {\
        background: rgb(255, 255, 255);\
        color: rgb(255, 0, 0);\
        border-radius: 5px;\
        padding: 5px 0;\
}"};

PlusMinusWidget::PlusMinusWidget(const CalibrationKoef &calibration,
                                 QSharedPointer<DevCommand> command,
                                 QWidget *parent)
    : QDialog(parent),
      m_command(command),
      ui(new Ui::CalibrationAndLimitsWidget),
      m_validator(new QDoubleValidator(this)) {
  ui->setupUi(this);
  ui->nameParameter->setText(calibration.name);
  ui->value->setText(m_command->valueStr());
  m_validator->setDecimals(m_command->tolerance());

  setMax(calibration.max);
  setMin(calibration.min);

  delta = 1.0 / m_command->divider();

  connect(ui->downValue, &QPushButton::clicked, this,
          &PlusMinusWidget::decrement);
  connect(ui->upValue, &QPushButton::clicked, this,
          &PlusMinusWidget::increment);

  connect(ui->value, &QLineEdit::textChanged, this,
          &PlusMinusWidget::textChanged);
  connect(ui->value, &QLineEdit::inputRejected, this,
          &PlusMinusWidget::rejectedEdit);
  connect(ui->value, &QLineEdit::editingFinished, this,
          &PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::PlusMinusWidget(const CalibrationKoef &calibration,
                                 QSharedPointer<CommandConverter> converter,
                                 QWidget *parent)
    : QDialog(parent),
      m_converter(converter),
      ui(new Ui::CalibrationAndLimitsWidget),
      m_validator(new QDoubleValidator(this)) {
  ui->setupUi(this);
  ui->nameParameter->setText(calibration.name);
  ui->value->setText(m_converter->valueStr());
  m_validator->setDecimals(m_converter->tolerance());

  setMax(calibration.max);
  setMin(calibration.min);

  delta = 1.0 / m_converter->divider();

  connect(ui->downValue, &QPushButton::clicked, this,
          &PlusMinusWidget::decrement);
  connect(ui->upValue, &QPushButton::clicked, this,
          &PlusMinusWidget::increment);

  connect(ui->value, &QLineEdit::textChanged, this,
          &PlusMinusWidget::textChanged);
  connect(ui->value, &QLineEdit::inputRejected, this,
          &PlusMinusWidget::rejectedEdit);
  connect(ui->value, &QLineEdit::editingFinished, this,
          &PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::PlusMinusWidget(const Limit &limit,
                                 QSharedPointer<DevCommand> command,
                                 QSharedPointer<DevCommand> maxCommand,
                                 QSharedPointer<DevCommand> minCommand,
                                 QWidget *parent)
    : QDialog(parent),
      m_command(command),
      ui(new Ui::CalibrationAndLimitsWidget),
      m_validator(new QDoubleValidator(this)) {
  ui->setupUi(this);
  ui->nameParameter->setText(limit.name);
  ui->value->setText(m_command->valueStr());
  m_validator->setDecimals(m_command->tolerance());

  setMax((maxCommand.isNull()) ? limit.maxValue : maxCommand->valueDouble());
  setMin((minCommand.isNull()) ? limit.minValue : minCommand->valueDouble());

  delta = 1.0 / m_command->divider();

  connect(ui->downValue, &QPushButton::clicked, this,
          &PlusMinusWidget::decrement);
  connect(ui->upValue, &QPushButton::clicked, this,
          &PlusMinusWidget::increment);

  connect(ui->value, &QLineEdit::textChanged, this,
          &PlusMinusWidget::textChanged);
  connect(ui->value, &QLineEdit::inputRejected, this,
          &PlusMinusWidget::rejectedEdit);
  connect(ui->value, &QLineEdit::editingFinished, this,
          &PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::PlusMinusWidget(const Limit &limit,
                                 QSharedPointer<CommandConverter> converter,
                                 QSharedPointer<CommandConverter> converterMax,
                                 QSharedPointer<CommandConverter> converterMin,
                                 QWidget *parent)
    : QDialog(parent),
      m_converter(converter),
      m_converterMax(converterMax),
      m_converterMin(converterMin),
      ui(new Ui::CalibrationAndLimitsWidget),
      m_validator(new QDoubleValidator(this)) {
  ui->setupUi(this);
  ui->nameParameter->setText(limit.name);
  ui->value->setText(m_converter->valueStr());
  m_validator->setDecimals(m_converter->tolerance());

  setMax((m_converterMax.isNull()) ? limit.maxValue
                                   : m_converterMax->valueDouble());
  setMin((m_converterMin.isNull()) ? limit.minValue
                                   : m_converterMin->valueDouble());

  delta = 1.0 / m_converter->divider();

  connect(ui->downValue, &QPushButton::clicked, this,
          &PlusMinusWidget::decrement);
  connect(ui->upValue, &QPushButton::clicked, this,
          &PlusMinusWidget::increment);

  connect(ui->value, &QLineEdit::textChanged, this,
          &PlusMinusWidget::textChanged);
  connect(ui->value, &QLineEdit::inputRejected, this,
          &PlusMinusWidget::rejectedEdit);
  connect(ui->value, &QLineEdit::editingFinished, this,
          &PlusMinusWidget::inputCompleted);
}

PlusMinusWidget::~PlusMinusWidget() { delete ui; }

void PlusMinusWidget::increment() {
  double value = ui->value->text().toDouble();
  value += delta;
  value = qBound(minValue, value, maxValue);
  ui->value->setText(QString::number(value, 'f', m_converter->tolerance()));
}

void PlusMinusWidget::decrement() {
  double value = ui->value->text().toDouble();
  value -= delta;
  value = qBound(minValue, value, maxValue);
  ui->value->setText(QString::number(value, 'f', m_converter->tolerance()));
}

void PlusMinusWidget::sendValue() {
  if (!m_converter.isNull() and
      (m_converter->valueStr() != ui->value->text())) {
    emit setDataFromWidget(
        m_converter->code(),
        m_converter->getRawFromValue(ui->value->text().toDouble()));
  }
  // m_command->setFromWidget(ui->value->text().toDouble());
}

double PlusMinusWidget::value() { return ui->value->text().toDouble(); }

void PlusMinusWidget::setMin(double min) {
  minValue = min;
  m_validator->setBottom(minValue);
  ui->minParameter->setText(QString("Min:%1").arg(minValue));
  validateValue();
}

void PlusMinusWidget::setMax(double max) {
  maxValue = max;
  m_validator->setTop(maxValue);
  ui->maxParameter->setText(QString("Max:%1").arg(maxValue));
  validateValue();
}

void PlusMinusWidget::textChanged() {
  m_textChanged = true;
  validateValue();
  emit lineEditTextChanged();
}

void PlusMinusWidget::validateValue() {
  QString v = ui->value->text();
  int pos = 0;
  if (m_validator->validate(v, pos) == QValidator::Acceptable) {
    ui->value->setStyleSheet(styleSheetOK);
    m_state = true;
  } else {
    ui->value->setStyleSheet(styleSheetERROR);
    m_state = false;
  }
}

void PlusMinusWidget::inputCompleted() {
  double value = ui->value->text().toDouble();
  ui->value->setText(QString::number(value, 'f', m_converter->tolerance()));
  ui->value->clearFocus();
}

void PlusMinusWidget::rejectedEdit() {
  ui->value->clear();
  ui->value->setText(m_converter->valueStr());
}

bool PlusMinusWidget::state() { return m_state; }

// private methods
void PlusMinusWidget::keyPressEvent(QKeyEvent *event) {
  if (event->matches(QKeySequence::Cancel)) {
    event->ignore();
    return;
  }
  QDialog::keyPressEvent(event);
}

QVector<quint16> PlusMinusWidget::Subscribe() {
  m_codes.append(m_converter->code());
  if (m_converterMax and m_converterMin) {
    m_codes.append(m_converterMax->code());
    m_codes.append(m_converterMin->code());
  }
  return m_codes;
}

void PlusMinusWidget::setData(quint16 code, quint16 data) {
  if (code == m_converter->code()) {
    m_converter->setValue(data);
    if (!m_textChanged) ui->value->setText(m_converter->valueStr());
  } else if (code == m_converterMax->code() and m_converterMax) {
    m_converterMax->setValue(data);
    setMax(m_converterMax->valueDouble());
  } else if (code == m_converterMin->code() and m_converterMin) {
    m_converterMin->setValue(data);
    setMin(m_converterMin->valueDouble());
  }
}
