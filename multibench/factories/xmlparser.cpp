#include "xmlparser.h"
#include <QDebug>
#include <utility>

XmlParser::XmlParser(const QByteArray& dataArray, QObject *parent) :
    Parser(dataArray, parent)
{
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
                 //xml->readElementText(QXmlStreamReader::SkipChildElements);
                if(m_tree == nullptr) {
                    m_tree = parseTag(xml);
                } else {
                    delete m_tree;
                    m_tree = parseTag(xml);
                }
            }
        }

        if(xml->hasError()) {
            makeError(xml->errorString() + "at row=" + QString::number(xml->lineNumber()) + " col="+ QString::number(xml->columnNumber()));
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
    QVariant* variant = new QVariant(0);
    TreeItem* item = new TreeItem(tagName.toString(), variant, items);

    for(const auto &attr : xml->attributes()) {
        items->append(parseAttribs(attr.name().toString(), attr.value().toString()));
    }

    while(!xml->atEnd() && !xml->hasError()) {
        if(m_stop) {
            break;
        }

        QXmlStreamReader::TokenType token = xml->readNext();

        if(token == QXmlStreamReader::StartElement) {
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
            QString tagText = xml->text().toString();
            if(tagText.compare(" ") != 0) {
                variant->setValue(tagText);
            }
//            items->append(new TreeItem("value", QVariant(xml)));
//            QString st = xml->readElementText(QXmlStreamReader::IncludeChildElements);
//            if(!st.isEmpty())
//            qDebug() << tagName.toString() << st;
        }
    }

    if(xml->hasError()) {
        makeError(xml->errorString() + "at row=" + QString::number(xml->lineNumber()) + " col="+ QString::number(xml->columnNumber()));
    }

    return item;
}

TreeItem* XmlParser::parseAttribs(QString name, QVariant value) {
    return new TreeItem(name, new QVariant(value), new QVector<TreeItem*>(0));
}
