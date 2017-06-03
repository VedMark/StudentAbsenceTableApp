#ifndef MENUCOMPONENTS_H
#define MENUCOMPONENTS_H

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

    QAction *addEntries;
    QAction *findEntries;
    QAction *removeEntries;

private:
    static MenuComponents *p_instance;
    MenuComponents();
    ~MenuComponents();

    MenuComponents(const MenuComponents*) = delete;
    MenuComponents& operator=(const MenuComponents*) = delete;
};


#endif // MENUCOMPONENTS_H
