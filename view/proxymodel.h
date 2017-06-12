#pragma once

#include "../model/studentabsencemodel.h"
#include "../controller/modelcontroller.h"

#include <QIdentityProxyModel>


class ProxyModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    ProxyModel(QObject *parent);
    ~ProxyModel();

    inline qint64 getEntriesPerPage() const
    { return entriesPerPage; }
    inline void setEntriesPerPage(qint64 value)
    {
        entriesPerPage = value;
        emit entriesPerPageChanged(value);
    }

    inline qint64 getPage() const
    { return page; }
    inline void setPage(const qint64 &value)
    {
        page = value;
        emit pageChanged(value);
    }

    qint64 maxPage();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

public slots:
    bool showPage(qint64 page);
    void resetModel();

signals:
    void entriesPerPageChanged(qint64);
    void pageChanged(qint64);

private:
    qint64 entriesPerPage;
    qint64 page;
};
