#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QString>
#include <QVector>

class TreeItem;

class TreeItem
{
public:
    TreeItem(QString name, QVariant* value, QVector<TreeItem*>* pChilds);
    ~TreeItem();

    QVariant value();
    QString name();
    bool hasChilds();
    int childCount();
    TreeItem* child(int num);

private:
    QString m_name;
    QVariant* m_value;

    QVector<TreeItem*> *m_childs;

};

#endif // TREEITEM_H
