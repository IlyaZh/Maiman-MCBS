#include "xmlparser.h"
#include <QDebug>

XmlParser::XmlParser(QString filename, QObject *parent) :
    Parser(filename, parent)
{

}

XmlParser::~XmlParser() {}


// public clots

void XmlParser::process() {
    if(m_file->open(QIODevice::ReadOnly)) {
        QByteArray dataBytes = m_file->readAll();
        QXmlStreamReader* xml = new QXmlStreamReader(dataBytes);

        while(!xml->atEnd() && !xml->hasError()) {
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
        }

//        readData(m_tree); // debug only

        xml->clear();
        m_file->close();
        delete xml;
    } else {
        makeError(QString("File %1 can't be open!").arg(m_file->fileName()));
    }


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
