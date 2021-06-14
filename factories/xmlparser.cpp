#include "xmlparser.h"
#include <QDebug>

XmlParser::XmlParser(QByteArray dataArray, QObject *parent) :
    Parser(dataArray, parent)
{
}

XmlParser::~XmlParser() {
}


bool XmlParser::start() {
        QXmlStreamReader* xml = new QXmlStreamReader(m_data);

        while(!xml->atEnd() && !xml->hasError()) {
            if(m_stop) {
                if(m_tree != nullptr) delete m_tree;
                return false;
            }

            QXmlStreamReader::TokenType token = xml->readNext();
            if(token == QXmlStreamReader::StartElement) {
                if(m_tree == nullptr) {
                    m_tree = parseTag(xml);
                } else {
                    delete m_tree;
                    m_tree = parseTag(xml);
                }
            }
        }

        if(xml->hasError()) {
            makeError(xml->errorString());
            xml->clear();
            delete xml;
            return false;
        }

//        readData(m_tree); // debug only

        xml->clear();
        delete xml;
        return true;
}

// private methods
TreeItem* XmlParser::parseTag(QXmlStreamReader* xml) {
    QVector<TreeItem*> *items = new QVector<TreeItem*>();
    QStringRef tagName = xml->name();
    TreeItem* item = new TreeItem(tagName.toString(), QVariant(xml->text().toString()), items);

    for(auto attr : xml->attributes()) {
        items->append(parseAttribs(attr.name().toString(), attr.value().toString()));
    }

    while(!xml->atEnd() && !xml->hasError()) {
        if(m_stop) {
            break;
        }

        QXmlStreamReader::TokenType token = xml->readNext();

        if(token == QXmlStreamReader::StartElement) {
//            qDebug() << tagName.toString() << xml->readElementText(/*QXmlStreamReader::SkipChildElements*/);
            if(xml->name() == tagName) {
                break;
            }
            items->append(parseTag(xml));
        } else
        if(token == QXmlStreamReader::EndElement) {
            if(xml->name() == tagName) {
                break;
            }
        } else if (token == QXmlStreamReader::Characters) {
            qDebug() << tagName.toString() << xml->text();
            QString st = xml->readElementText(QXmlStreamReader::SkipChildElements);
//            if(!st.isEmpty())
//            qDebug() << tagName.toString() << st;
        }
    }

    if(xml->hasError()) {
        makeError(xml->errorString());
    }

    return item;
}

TreeItem* XmlParser::parseAttribs(QString name, QVariant value) {
    return new TreeItem(name, value, new QVector<TreeItem*>(0));
}
