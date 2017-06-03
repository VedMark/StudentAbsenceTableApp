#ifndef STUDENTTABLEVIEW_H
#define STUDENTTABLEVIEW_H

#include <QTableView>

#include "../model/studentabsencemodel.h"
#include "../view/HierarchicalHeaderView.h"

class StudentTableView : public QTableView
{
    Q_OBJECT
public:
    static qint8 ENTRIES_PER_PAGE;

    StudentTableView(QWidget *parent = Q_NULLPTR);

    qint64 getPage() const;
    void setPage(const qint64 &value);

    StudentAbsenceModel *getModel() const;
    void setModel(StudentAbsenceModel *value);

public slots:
    bool showPrevPage();
    bool showNextPage();
    void enablePrevPage();
    void enableNextPage();

protected slots:
    void changePage(qint8);

protected:
    //void paintEvent(QPaintEvent *);

signals:
    void indexerChanged();

private:
    HierarchicalHeaderView *header;
    qint64 page;
};

#endif // STUDENTTABLEVIEW_H
