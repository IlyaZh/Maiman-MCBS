#ifndef UPDATEWIDGET_H
#define UPDATEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "constants.h"
#include "UpdatesChecker.h"
#include "version.h"
#include <QtCore>

class UpdateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateWidget(QWidget *parent = nullptr);
private:
    static const QString m_Url;
    UpdatesChecker* m_updater;

signals:
    void updateApp(QCoreApplication* app);

private slots:
    void acceptError(const QString& errorMsg);
    void updateCheck(bool state);
};

#endif // UPDATEWIDGET_H
