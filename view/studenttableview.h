#ifndef STUDENTTABLEVIEW_H
#define STUDENTTABLEVIEW_H

#include <QTableWidget>

#include "../model/studentabsencemodel.h"
#include "../view/HierarchicalHeaderView.h"

class StudentTableView : public QTableView
{
    Q_OBJECT
public:

    StudentTableView(QWidget *parent = Q_NULLPTR);

private:
    HierarchicalHeaderView *header;
};

#endif // STUDENTTABLEVIEW_H
