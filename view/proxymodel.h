#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include "QDebug"

#include <QIdentityProxyModel>

#include "../model/studentabsencemodel.h"
#include "../controller/modelcontroller.h"

class ProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    ProxyModel(QObject *parent);

    inline qint64 getEntriesPerPage() const
    { return entriesPerPage; }
    inline void setEntriesPerPage(qint64 value)
    { entriesPerPage = value; emit entriesPerPageChanged(value); }

    inline qint64 getPage() const
    { return page; }
    inline void setPage(const qint64 &value)
    { page = value; emit pageChanged(value); }

    qint64 maxPage();

    inline int rowCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent)
        if(!sourceModel())
            return 0;

        return entriesPerPage * page <= sourceModel()->rowCount()
                ? entriesPerPage
                : sourceModel()->rowCount() - entriesPerPage * (page - 1);
    }

    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent);
        return StudentAbsenceModel::LAST;
    }

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

public slots:
    bool showPage(qint64 page);

signals:
    void entriesPerPageChanged(qint64);
    void pageChanged(qint64);

private:
    qint64 entriesPerPage;
    qint64 page;
};

#endif // PROXYMODEL_H
