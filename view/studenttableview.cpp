#include "studenttableview.h"
#include "../controller/menucomponents.h"

#include <QBrush>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>


StudentTableView::StudentTableView(QWidget* parent)
    : QTableView(parent)
{
    page = 0;

    header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setHighlightSections(true);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    setHorizontalHeader(header);

    verticalHeader()->setDefaultSectionSize(30);

    connect(
        this, &StudentTableView::indexerChanged,
        [this] () { enablePrevPage(); }
    );

    connect(
        this, &StudentTableView::indexerChanged,
        [this] () { enableNextPage(); }
    );
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
                        page < static_cast<int>(getModel()->entriesSize() / entriesPerPage));
}

void StudentTableView::changePage(qint8 value)
{
    qint64 nextPage = page + value;
    qint64 maxNumPages = static_cast<int>(getModel()->entriesSize() / entriesPerPage);
    page = nextPage <= 0 ? 0 : nextPage > maxNumPages ? maxNumPages : nextPage;
    emit indexerChanged();
}

void StudentTableView::paintEvent(QPaintEvent *event)
{
//    Q_UNUSED(event);
//    int height = viewport()->height() / ENTRIES_PER_PAGE;

//    QPainter painter(viewport());
//    painter.setPen(QColor(200, 200, 200));

//    for(int i = 0; i < ENTRIES_PER_PAGE; ++i)
//        painter.drawLine(0, i * height - 1, viewport()->width(), i * height - 1);

//    QPainter painter(viewport());
//    painter.setPen(QColor(200, 200, 200));
//    int h = viewport()->height() / ENTRIES_PER_PAGE;

//    for(int r = 0; r < ENTRIES_PER_PAGE; ++r)
//        for(int c = 0; c < getModel()->LAST; ++c)
//        {
//            int x = columnViewportPosition(c);
//            int y = rowViewportPosition(r);
//            int w = columnWidth(c);
//            painter.drawRect(x, y, w, h);
//            painter.drawText(QPointF(x,y), getModel()->data(getModel()->index(r, c, QModelIndex())).toString());

//        }

    QTableView::paintEvent(event);
}
