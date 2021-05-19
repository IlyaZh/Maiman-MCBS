#include "parseitem.h"

ParseItem::ParseItem(const QDomNode &node, int row, ParseItem *parent)
    :domNode(node),parentItem(parent),rowNumber(row)
{

}
ParseItem::~ParseItem()
{
    qDeleteAll(childItems);
}

QDomNode ParseItem::node() const
{
    return domNode;
}

ParseItem *ParseItem::parent()
{
    return parentItem;
}

ParseItem *ParseItem::child(int i)
{
    ParseItem *childItem = childItems.value(i);

    if(childItem)
        return childItem;

    if(i>=0 && i< domNode.childNodes().count())
    {
        QDomNode childNode = domNode.childNodes().item(i);
        if ((childNode.nodeType() == QDomNode::CommentNode)||(childNode.nodeType() == QDomNode::ProcessingInstructionNode))
            return childItem;
        childItem = new ParseItem(childNode,i,this);
        childItems[i] = childItem;
    }
    return childItem;
}

int ParseItem::row() const
{
    return rowNumber;
}
