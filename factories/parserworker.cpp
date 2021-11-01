#include "parserworker.h"
#include <QDebug>
#include <QScopedPointer>

ParserWorker::ParserWorker(const QString& fileName, ParserType type, QObject *parent) :
    QObject(parent),
    m_fileName(fileName),
    m_type(type)
{
}

ParserWorker::~ParserWorker() {

}

TreeItem* ParserWorker::data() {
    return m_data;
}

void ParserWorker::process() {
    QScopedPointer<QFile> file(new QFile(m_fileName));
    if(file->exists()) {
        if(file->open(QIODevice::ReadOnly)) {

            QByteArray dataArray = file->readAll();

            switch(m_type) {
                case ParserType::XmlParser:
                m_parser.reset(new XmlParser(dataArray));
                break;
            default:
                break;
            }

            if(m_parser) {
                if(m_parser->start()) {
                    m_data = m_parser->data();
                    m_parser.reset();
                    emit finished();
                } else {
                    QString errorString = QString("Parsing erorr in file %1! \"%2\"").arg(file->fileName(), m_parser->errorString());
                    emit errorOccured(errorString);
                }
            }

        } else {
            QString errorString = QString("File %1 can't be open!").arg(file->fileName());
            emit errorOccured(errorString);
        }
    } else {
       QString errorString = QString("File %1 is not exists!").arg(m_fileName);
       emit errorOccured(errorString);
    }
}

void ParserWorker::stop() {
    if(m_parser) {
        m_parser->stop();
    }
}

// private slots

void ParserWorker::getData() {
    if(m_parser) m_data = m_parser->data();
}

// private methods

//Parser* ParserWorker::createParser(QByteArray dataArray) {
//    Parser* parser = nullptr;


//    return parser;
//}
