#ifndef QUITDIALOG_H
#define QUITDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

#include "gui/guiinterface.h"
namespace Ui {
class QuitDialog;
}

class QuitDialog : public QDialog, public GuiWidgetInterface {
  Q_OBJECT

 public:
  explicit QuitDialog(QWidget *parent = nullptr);
  ~QuitDialog();
  static QuitDialog *createDialog(
      const QString &title, QString message,
      QDialogButtonBox::StandardButtons buttons = (QDialogButtonBox::Ok |
                                                   QDialogButtonBox::Cancel),
      int fontSize = 10, QWidget *parent = nullptr);
  void updateStyle() override;

 private:
  Ui::QuitDialog *ui;
};

#endif  // QUITDIALOG_H
