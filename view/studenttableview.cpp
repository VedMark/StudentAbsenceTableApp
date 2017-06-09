#include "studenttableview.h"
#include "../controller/menucomponents.h"


StudentTableView::StudentTableView(QWidget* parent)
    : QTableView(parent)
{
    header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setHighlightSections(true);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    header->setDefaultSectionSize(31);
}
