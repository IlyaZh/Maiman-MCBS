#include "guifactory.h"

GuiFactory::GuiFactory(QString fileName, AppSettings& settings, QObject *parent) :
    QObject(parent),
    m_settings(settings),
    m_fileName(fileName)
{

}

GuiFactory::~GuiFactory() {}

void GuiFactory::start() {
    if(!m_parseWorker.isNull()) m_parseWorker->deleteLater();
    m_parseWorker = new ParserWorker(m_fileName, ParserType::XmlParser);
    m_thread = new QThread();
    m_parseWorker->moveToThread(m_thread);

    connect(m_thread, SIGNAL(started()), m_parseWorker, SLOT(process()));
    connect(m_parseWorker, SIGNAL(finished()), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(finished()), this, SLOT(parsingFinished()));
    //    //connect(...,m_parser, SLOT(stop()));
    // проверь сигналы по статье
    //    connect(m_parseWorker, SIGNAL(finished()), m_parseWorker, SLOT(deleteLater()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), this, SLOT(threadError(QString)));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(quit()));
    connect(m_parseWorker, SIGNAL(errorOccured(QString)), m_thread, SLOT(deleteLater()));
    m_thread->start();
}

// private slots

void GuiFactory::parsingFinished() {
    TreeItem* parserTree = m_parseWorker->data();
    qDebug() << "m_parseWorker.reset();";
//    m_parseWorker->deleteLater();
    if(parseTree(*parserTree)) {
        qDebug() << "Parse tree is ok!";
    } else {
        qDebug() << "Can't parse tree";
    }
    m_parseWorker->deleteLater();
    delete parserTree;
}

void GuiFactory::threadError(const QString &msg) {
    qDebug() << "Error while parsing. " << msg;
}

// private methods

bool GuiFactory::parseTree(TreeItem& tree) {
    if(tree.name() == "GUI") {
        qDebug() << "YEAH!";
    }
}

