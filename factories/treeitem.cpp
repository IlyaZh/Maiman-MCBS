#include "treeitem.h"

TreeItem::TreeItem(QString name, QVariant value, QVector<TreeItem*>* pChilds) : m_name(name), m_value(value), m_childs(pChilds) {

}

TreeItem::~TreeItem() {
    qDeleteAll(*m_childs);
}

QVariant TreeItem::value() {
    return m_value;
}

QString TreeItem::name() {
    return m_name;
}

bool TreeItem::hasChilds() {
    return (!m_childs->isEmpty());
}

int TreeItem::childCount() {
    return m_childs->count();
}

TreeItem* TreeItem::child(int num) {
    return (num < m_childs->count()) ? m_childs->at(num) : nullptr;
}
