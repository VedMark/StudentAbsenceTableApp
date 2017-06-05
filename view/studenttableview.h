#ifndef STUDENTTABLEVIEW_H
#define STUDENTTABLEVIEW_H

#include <QTableView>

#include "../model/studentabsencemodel.h"
#include "../view/HierarchicalHeaderView.h"

class StudentTableView : public QTableView
{
    Q_OBJECT
public:

    StudentTableView(QWidget *parent = Q_NULLPTR);

    inline qint64 getPage() const
    { return page; }
    inline void setPage(const qint64 &value)
    { page = value; }

    inline StudentAbsenceModel *getModel() const
    { return static_cast<StudentAbsenceModel*>(model()); }
    inline void setModel(StudentAbsenceModel *value)
    { QTableView::setModel(value); }

    inline qint8 getEntriesPerPage() const
    { return entriesPerPage; }
    inline void setEntriesPerPage(const qint8 &value)
    { entriesPerPage = value; }

public slots:
    bool showPrevPage();
    bool showNextPage();
    void enablePrevPage();
    void enableNextPage();

protected slots:
    void changePage(qint8);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void indexerChanged();

private:
    qint8 entriesPerPage;

    HierarchicalHeaderView *header;
    qint64 page;
};

#endif // STUDENTTABLEVIEW_H
