#include "proxymodel.h"

#include <algorithm>
#include <iterator>

#include "../controller/menucomponents.h"

ProxyModel::ProxyModel(StudentAbsenceModel *source, QObject *parent)
    : QAbstractProxyModel(parent)
{
    sourceModel = source;
    proxy = new StudentAbsenceModel(this);

    entriesPerPage = 20;
    page = 0;

    connect(
        this, &ProxyModel::indexerChanged,
                [this] () { refreshPageEntries(); });
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel->headerData(section, orientation, role);
}

bool ProxyModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
    return sourceModel->hasIndex(row, column, parent);
}

QModelIndex ProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    return sourceModel->index(row, column, parent);
}

QVariant ProxyModel::data(const QModelIndex &index, int role) const
{
    return sourceModel->data(index, role);
}

QModelIndex ProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return sourceModel->index(sourceIndex.row() % entriesPerPage, sourceIndex.column(), QModelIndex());
}

QModelIndex ProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return sourceModel->index(proxyIndex.row() * (page + 1), proxyIndex.column(), QModelIndex());
}

QItemSelection ProxyModel::mapSelectionFromSource(const QItemSelection &selection) const
{
    return QItemSelection();
}

QItemSelection ProxyModel::mapSelectionToSource(const QItemSelection &selection) const
{
    return QItemSelection();
}

bool ProxyModel::showPrevPage()
{
    changePage(-1);
    return true;
}

bool ProxyModel::showNextPage()
{
    changePage(1);
    return true;
}

void ProxyModel::enablePrevPage()
{
    MenuComponents::instance().prevPage->setEnabled(page);
}

void ProxyModel::enableNextPage()
{
    MenuComponents::instance().nextPage->setEnabled( page < maxPage());
}

void ProxyModel::refreshPageEntries()
{
//    StudentAbsenceModel::students result;
//    StudentAbsenceModel::students::iterator beginIt =
//            sourceModel->getStudentEntryList().begin() + entriesPerPage * page;
//    StudentAbsenceModel::students::iterator endIt =
//            entriesPerPage * (page + 1) >= sourceModel->getStudentEntryList().size() ?
//            sourceModel->getStudentEntryList().end() :
//            sourceModel->getStudentEntryList().begin() + entriesPerPage * (page + 1);

//    std::copy(beginIt, endIt, std::back_inserter(result));

    //proxy->removeRows(0, entriesPerPage, QModelIndex());

    //proxy->setStudentEntryList(result);
}

void ProxyModel::changePage(qint8 value)
{
    qint64 nextPage = page + value;
    qint64 maxNumPages = maxPage();
    page = nextPage <= 0 ? 0 : nextPage > maxNumPages ? maxNumPages : nextPage;
    emit indexerChanged(page);
}

QModelIndex ProxyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex ProxyModel::proxyIndexToSource(const QModelIndex& proxyIndex) const
{
    return  sourceModel->index(proxyIndex.row() * (page + 1), proxyIndex.column(), QModelIndex());
}

qint64 ProxyModel::maxPage()
{
    return static_cast<int>(sourceModel->entriesSize() - 1 / entriesPerPage);
}
