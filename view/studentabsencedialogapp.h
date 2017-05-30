#ifndef QTABLE_H
#define QTABLE_H

#include <QHeaderView>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QLineEdit>
#include <QLabel>

#include "HierarchicalHeaderView.h"
#include "../model/studentabsencemodel.h"

class StudentAbsenceTableApp : public QMainWindow
{
    Q_OBJECT

public:
    StudentAbsenceTableApp(QWidget *parent = 0);
    ~StudentAbsenceTableApp();

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

private slots:
//    void newFile();
//    void open();
//    bool save();
//    bool saveAs();

private:
    HierarchicalHeaderView *header;
    StudentAbsenceModel *source;
    QTableView *view;
};

#endif // QTABLE_H
