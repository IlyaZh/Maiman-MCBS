#ifndef ITEM_H
#define ITEM_H

#include <QDomNode>
#include <QHash>

class ParseItem
{
public:
    ParseItem(const QDomNode &node, int row, ParseItem *parent = nullptr);
    ~ParseItem();
    ParseItem *child(int i);
    ParseItem *parent();
    QDomNode node() const;
    int row() const;

private:
    QDomNode domNode;
    QHash<int, ParseItem *> childItems;
    ParseItem *parentItem;
    int rowNumber;
};

#endif // ITEM_H
