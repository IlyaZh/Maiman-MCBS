#ifndef PARSERWORKER_H
#define PARSERWORKER_H

#include <QObject>
#include <QPointer>
#include "Parser.h"
#include "xmlparser.h"

enum class ParserType {
    None,
    XmlParser
};

class ParserWorker : public QObject
{
    Q_OBJECT
public:
    explicit ParserWorker(QString fileName, ParserType type, QObject *parent = nullptr);
    ~ParserWorker();
    TreeItem* data();

public slots:
    void process();
    void stop();

private slots:
    void getData();

signals:
    void finished();
    void errorOccured(QString);

private:
    TreeItem* m_data = nullptr;
    QScopedPointer<Parser> m_parser;
    QString m_fileName;
    ParserType m_type;

    Parser* createParser(QByteArray dataArray);

};

#endif // PARSERWORKER_H
