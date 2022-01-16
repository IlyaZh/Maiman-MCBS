#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QXmlStreamReader>
#include "Parser.h"

class XmlParser : public Parser
{
    Q_OBJECT
public:
    explicit XmlParser(const QByteArray& dataArray, QObject *parent = nullptr);
    virtual bool start() override;

private:
    TreeItem* parseTag(QXmlStreamReader* xml);
    TreeItem* parseAttribs(QString name, QVariant value);

};

#endif // XMLPARSER_H
