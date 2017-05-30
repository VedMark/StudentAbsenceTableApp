#include "studentabsencedialogapp.h"

StudentAbsenceTableApp::StudentAbsenceTableApp(QWidget *parent)
    : QMainWindow(parent)
{
    view = new QTableView(this);

    view->setModel(source = new StudentAbsenceModel(this));

    header = new HierarchicalHeaderView(Qt::Horizontal, view);
    header->setHighlightSections(true);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    view->setHorizontalHeader(header);

    setCentralWidget(view);
    resize(maximumSize());
}

StudentAbsenceTableApp::~StudentAbsenceTableApp()
{

}

void StudentAbsenceTableApp::closeEvent(QCloseEvent *)
{
    close();
}

void StudentAbsenceTableApp::resizeEvent(QResizeEvent *)
{
    view->setColumnWidth(0, width() / 3);
    view->setColumnWidth(1, width() / 6);
    view->setColumnWidth(2, width() / 8);
    view->setColumnWidth(3, width() / 8);
    view->setColumnWidth(4, width() / 6);
    view->setColumnWidth(5, width() / 12);
}
