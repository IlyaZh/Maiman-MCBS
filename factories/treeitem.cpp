#include "treeitem.h"

TreeItem::TreeItem(QString name, QVariant* value, QVector<TreeItem*>* pChilds) : m_name(name), m_value(value), m_childs(pChilds) {
}

TreeItem::~TreeItem() {
    qDeleteAll(*m_childs);
    delete m_value;
}

const QVariant& TreeItem::value() const {
    return (*m_value);
}

const QString& TreeItem::name() const {
    return m_name;
}

bool TreeItem::hasChilds() const {
    return (!m_childs->isEmpty());
}

int TreeItem::childCount() const {
    return m_childs->count();
}

const TreeItem& TreeItem::child(int num) const {
    Q_ASSERT(num < m_childs->size());
    return *(m_childs->at(num));
}
