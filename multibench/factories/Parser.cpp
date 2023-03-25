#include "Parser.h"

Parser::Parser(const QByteArray& dataArray, QObject* parent)
    : QObject(parent), m_data(dataArray) {
  m_data = m_data.simplified();
}

Parser::~Parser() { m_data.clear(); }

TreeItem* Parser::data() { return m_tree; }

void Parser::stop() { m_stop = true; }

QString Parser::errorString() { return m_errorString; }

// protected methods
void Parser::makeError(const QString& msg) {
  if (!m_errorString.isEmpty()) m_errorString.append("; ");
  m_errorString.append("[XmlParser] " + msg);
}

/*
 * debug only
 *  void Parser::readData(TreeItem* item, int level) {
    if(item != nullptr) {
        QString space = "";
        for(int i = 0; i < level; i ++) space += "=";
        space += item->name();

        qDebug() << space;
        for(int i = 0; i < item->childCount(); i++) {
            readData(item->child(i), level+1);
        }

    }
}*/
