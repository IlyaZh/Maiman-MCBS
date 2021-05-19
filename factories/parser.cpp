#include "parser.h"
#include "parseitem.h"
#include <QtXml>
#include <QDebug>

Parser::Parser(QString filePath, QObject *parent )
    : QAbstractItemModel(parent)
      //domDocument(document),
      //rootItem(new item(domDocument, 0))
{
    this->filePath = filePath;
}

Parser::~Parser()
{
    clear();
}

void Parser::clear()
{
    delete rootItem;
}

void Parser::process()
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QDomDocument document;
        QString errorMsg;
        int errorLine = 0;
        int errorCol = 0;
        if (document.setContent(&file, &errorMsg, &errorLine, &errorCol)) {
            domDocument = document;
            rootItem = new ParseItem(domDocument,0);
        } else {
            emit errorOccured(QString("File %1, line = %2, col=%3 error: %4").arg(filePath).arg(errorLine).arg(errorCol).arg(errorMsg));
            return;
        }
        file.close();
        emit finished();
    } else
        emit errorOccured(QString("File %1, error: %2").arg(filePath).arg(file.errorString()));
}


int Parser::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

Qt::ItemFlags Parser::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QModelIndex Parser::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ParseItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ParseItem*>(parent.internalPointer());

    ParseItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex Parser::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    ParseItem *childItem = static_cast<ParseItem*>(child.internalPointer());
    ParseItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
int Parser::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    ParseItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ParseItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

QVariant Parser::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const ParseItem *item = static_cast<class ParseItem*>(index.internalPointer());

    const QDomNode node = item->node();

    switch (index.column()) {
        case 0:
            return node.nodeName();
        case 1:
        {
            const QDomNamedNodeMap attributeMap = node.attributes();
            QStringList attributes;            
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + "="
                              + attribute.nodeValue();
            }
            return attributes.join(' ');
        }
        case 2:
            return node.nodeValue();
        case 3:
        {
            const QDomNamedNodeMap attributeMap = node.attributes();
            QString attribValue;
            for (int i = 0; i < attributeMap.count(); ++i) {
                QDomNode attribute = attributeMap.item(i);
                if (attribute.nodeName() == attrib)
                    attribValue = attribute.nodeValue();
            }
            return attribValue;
        }
        default:
            break;
    }
    return QVariant();
}

void Parser::attribFind(QString name)
{
    attrib = name;
}
