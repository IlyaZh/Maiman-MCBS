#ifndef GUIFACTORY_H
#define GUIFACTORY_H

#include <QThread>
#include <QObject>
#include "factories/parserworker.h"
#include "appsettings.h"

class GuiFactory : public QObject
{
    Q_OBJECT
public:
    GuiFactory(QString fileName, AppSettings& settings, QObject *parent = nullptr);
    ~GuiFactory();
    void start();


private slots:
    void parsingFinished();
    void threadError(const QString&);

signals:

private:
    QThread* m_thread;
    QPointer<ParserWorker> m_parseWorker;
    AppSettings& m_settings;
    QString m_fileName;

    bool parseTree(TreeItem& tree);

};

#endif // GUIFACTORY_H
