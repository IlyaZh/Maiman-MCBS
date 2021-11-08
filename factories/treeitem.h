#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QString>
#include <QVector>

class TreeItem;

class TreeItem
{
public:
    TreeItem(const QString& name, QVariant* value, QVector<TreeItem*>* pChilds);
    ~TreeItem();

    const QVariant& value() const;
    const QString& name() const;
    bool hasChilds() const;
    int childCount() const;
    const TreeItem& child(int num) const;

private:
    QString m_name;
    QVariant* m_value;

    QVector<TreeItem*>* m_childs;

};

#endif // TREEITEM_H
