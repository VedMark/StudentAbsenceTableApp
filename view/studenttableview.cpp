#include "studenttableview.h"
#include "../controller/menucomponents.h"

#include <QPaintEvent>

qint8 StudentTableView::ENTRIES_PER_PAGE = 20;

StudentTableView::StudentTableView(QWidget* parent)
    : QTableView(parent)
{
    page = 0;

    header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setHighlightSections(true);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    setHorizontalHeader(header);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(
        this, &StudentTableView::indexerChanged,
        [this] () { enablePrevPage(); }
    );

    connect(
        this, &StudentTableView::indexerChanged,
        [this] () { enableNextPage(); }
    );
}

qint64 StudentTableView::getPage() const
{
    return page;
}

void StudentTableView::setPage(const qint64 &value)
{
    page = value;
}

StudentAbsenceModel *StudentTableView::getModel() const
{
    return static_cast<StudentAbsenceModel*>(model());
}

void StudentTableView::setModel(StudentAbsenceModel *value)
{
    QTableView::setModel(value);
}

bool StudentTableView::showPrevPage()
{
    changePage(-1);

    return true;
}

bool StudentTableView::showNextPage()
{
    changePage(1);

    return true;
}

void StudentTableView::enablePrevPage()
{
    MenuComponents::instance().prevPage->setEnabled(page);
}

void StudentTableView::enableNextPage()
{
    MenuComponents::instance().nextPage->setEnabled(
                        page < static_cast<int>(getModel()->entriesSize() / ENTRIES_PER_PAGE));
}

void StudentTableView::changePage(qint8 value)
{
    qint64 nextPage = page + value;
    qint64 maxNumPages = static_cast<int>(getModel()->entriesSize() / ENTRIES_PER_PAGE);
    page = nextPage <= 0 ? 0 : nextPage > maxNumPages ? maxNumPages : nextPage;
    emit indexerChanged();
}

//void StudentTableView::paintEvent(QPaintEvent *)
//{

//}
