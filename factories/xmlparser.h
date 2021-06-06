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
    explicit XmlParser(QString filename, QObject *parent = nullptr);
    ~XmlParser();

public slots:
    virtual void process() override;

private:
//    TreeItem* m_lastTree = nullptr;

    TreeItem* parseTag(QXmlStreamReader* xml);
    TreeItem* parseAttribs(QString name, QVariant value);

};

#endif // XMLPARSER_H
