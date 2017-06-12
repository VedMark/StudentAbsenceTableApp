#pragma once

#include <QAction>


class MenuComponents
{
public:
    static MenuComponents &instance();

    QAction *newTable;
    QAction *openTable;
    QAction *saveTable;
    QAction *saveAsTable;
    QAction *exitApp;

    QAction *prevPage;
    QAction *nextPage;
    QAction *firstPage;
    QAction *lastPage;
    QAction *goToPage;
    QAction *entriesPerPage;

    QAction *addEntries;
    QAction *findEntries;
    QAction *removeEntries;

public slots:
    void enablePrevPage(bool valid);
    void enableNextPage(bool valid);
    void enableFirstPage(bool valid);
    void enableLastPage(bool valid);

private:
    static MenuComponents *p_instance;
    MenuComponents();
    ~MenuComponents();

    MenuComponents(const MenuComponents*) = delete;
    MenuComponents& operator=(const MenuComponents*) = delete;
};
