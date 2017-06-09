#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QAbstractProxyModel>

#include "../model/studentabsencemodel.h"
#include "../controller/modelcontroller.h"

class ProxyModel : public QAbstractProxyModel
{
    Q_OBJECT

public:
    ProxyModel(StudentAbsenceModel *source, QObject *parent);

    inline int getEntriesPerPage() const
    { return entriesPerPage; }
    inline void setEntriesPerPage(int value)
    { entriesPerPage = value; }

    inline qint64 getPage() const
    { return page; }
    inline void setPage(const qint64 &value)
    { page = value; }

    inline void setSourceModel(StudentAbsenceModel *value)
    { sourceModel = value; }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool hasIndex(int row, int column, const QModelIndex &parent) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    inline int rowCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent)
        return sourceModel->entriesSize();
    }
    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent);
        return StudentAbsenceModel::LAST;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

    QItemSelection mapSelectionFromSource(const QItemSelection &selection) const override;
    QItemSelection mapSelectionToSource(const QItemSelection &selection) const override;

public slots:
    bool showPrevPage();
    bool showNextPage();
    void enablePrevPage();
    void enableNextPage();
    void refreshPageEntries();

protected slots:
    void changePage(qint8);

signals:
    void indexerChanged(int);

private:
    QModelIndex parent(const QModelIndex &child) const override;
    QModelIndex proxyIndexToSource(const QModelIndex &proxyIndex) const;

    qint64 maxPage();

    const StudentAbsenceModel *sourceModel;
    StudentAbsenceModel *proxy;

    int entriesPerPage;
    qint64 page;
};

#endif // PROXYMODEL_H
