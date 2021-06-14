#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include "treeitem.h"

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QByteArray dataArray, QObject *parent = nullptr);
    ~Parser();
    TreeItem* data();
    virtual bool start() = 0;
    virtual void stop();
    virtual QString errorString();

protected:
    QByteArray m_data;
    TreeItem* m_tree = nullptr;
    bool m_stop = false;
    QString m_errorString;

    void makeError(QString msg);
//    void readData(TreeItem* item, int level = 0); // debug only

};

#endif // PARSER_H
