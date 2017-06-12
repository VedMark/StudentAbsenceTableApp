#include "proxymodel.h"

#include <algorithm>
#include <iterator>

#include "../controller/menucomponents.h"

ProxyModel::ProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    entriesPerPage = 5;
    page = 1;
}

QModelIndex ProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if(!sourceModel() && !sourceIndex.isValid())
        return QModelIndex();

    return sourceIndex.isValid()
            ? createIndex(sourceIndex.row() % entriesPerPage, sourceIndex.column(), sourceIndex.internalPointer())
            : QModelIndex();
}

QModelIndex ProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if(!sourceModel() && !proxyIndex.isValid())
        return QModelIndex();

    QModelIndex remapped = createIndex(proxyIndex.row() + entriesPerPage * (page - 1),
                                       proxyIndex.column(),
                                       proxyIndex.internalPointer());
    return QIdentityProxyModel::mapToSource(remapped);
}

bool ProxyModel::showPage(qint64 page_)
{
    if(page > maxPage() || page < 1)
        return false;
    page = page_;
    emit pageChanged(page_);
    return true;
}

qint64 ProxyModel::maxPage()
{
    return static_cast<int>((sourceModel()->rowCount() - 1) / entriesPerPage + 1);
}
