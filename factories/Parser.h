#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include "treeitem.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QString fileName, QObject *parent = nullptr);
    ~Parser();
    TreeItem* data();
public slots:
    virtual void process() {};

signals:
    void errorOccured(QString);
    void finished();

protected:
    QString m_fileName;
    TreeItem* m_tree = nullptr;
    QFile* m_file = nullptr;

    void makeError(QString msg);
//    void readData(TreeItem* item, int level = 0); // debug only

};

#endif // PARSER_H
