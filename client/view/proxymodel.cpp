#include "proxymodel.h"
#include "../controller/menucomponents.h"


ProxyModel::ProxyModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
    entriesPerPage = 20;
    page = 1;
}

ProxyModel::~ProxyModel()
{}

int ProxyModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    if(!sourceModel())
        return 0;

    return entriesPerPage * page <= sourceModel()->rowCount()
            ? entriesPerPage
            : sourceModel()->rowCount() - entriesPerPage * (page - 1);
}

int ProxyModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return StudentAbsenceModel::LAST;
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
    if(page_ > maxPage() || page_ < 1)
        return false;
    page = page_;
    emit pageChanged(page_);
    return true;
}

void ProxyModel::resetModel()
{
    beginResetModel();
    endResetModel();
}

qint64 ProxyModel::maxPage()
{
    return static_cast<int>((sourceModel()->rowCount() - 1) / entriesPerPage + 1);
}
