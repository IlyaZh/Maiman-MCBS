#include "Parser.h"

Parser::Parser(QString fileName, QObject* parent) :
    QObject(parent),
    m_fileName(fileName)
{
    m_file = new QFile(m_fileName);
    if(!m_file->exists()) {
        makeError(QString("File %1 is not exists!").arg(m_fileName));
    }
}

Parser::~Parser() {
    m_file->deleteLater();
}

TreeItem* Parser::data() {
    return m_tree;
}


// protected methods
void Parser::makeError(QString msg) {
    emit errorOccured("[XmlParser] "+msg);
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
