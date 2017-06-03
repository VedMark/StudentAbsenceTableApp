#include "menucomponents.h"

MenuComponents *MenuComponents::p_instance = Q_NULLPTR;

MenuComponents &MenuComponents::instance()
{
    static MenuComponents m_instance;
    return m_instance;
    if(MenuComponents::p_instance == Q_NULLPTR)
        MenuComponents::p_instance = new MenuComponents();
    //return MenuComponents::p_instance;
}

MenuComponents::MenuComponents()
{
    newTable = new QAction(QObject::tr("&New"), Q_NULLPTR);
    newTable->setStatusTip(QObject::tr("Create New Table"));
    newTable->setShortcut(QKeySequence::New);
    newTable->setIcon(QIcon(":/images/new.png"));

    openTable = new QAction(QObject::tr("&Open"), Q_NULLPTR);
    openTable->setShortcut(QKeySequence::Open);
    openTable->setIcon(QIcon(":/images/open.png"));

    saveTable = new QAction(QObject::tr("&Save"), Q_NULLPTR);
    saveTable->setShortcut(QKeySequence::Save);
    saveTable->setIcon(QIcon(":/images/save.png"));

    saveAsTable = new QAction(QObject::tr("&Save as"), Q_NULLPTR);
    saveAsTable->setShortcut(QKeySequence::SaveAs);

    exitApp = new QAction(QObject::tr("&Exit"), Q_NULLPTR);
    exitApp->setShortcut(QKeySequence::Quit);
    exitApp->setIcon(QIcon(":/images/exit.png"));

    prevPage = new QAction(QObject::tr("&Previous Page"), Q_NULLPTR);
    prevPage->setShortcut(QKeySequence::Back);
    prevPage->setIcon(QIcon(":/images/prevPage.png"));

    nextPage = new QAction(QObject::tr("&Next Page"), Q_NULLPTR);
    nextPage->setShortcut(QKeySequence::Forward);
    nextPage->setIcon(QIcon(":/images/nextPage.png"));

    addEntries = new QAction(QObject::tr("&Add Entries"), Q_NULLPTR);
    addEntries->setIcon(QIcon(":/images/addEntries.png"));

    findEntries = new QAction(QObject::tr("&Find Entries"), Q_NULLPTR);
    findEntries->setIcon(QIcon(":/images/findEntries.png"));

    removeEntries = new QAction(QObject::tr("&Remove Entries"), Q_NULLPTR);
    removeEntries->setIcon(QIcon(":/images/removeEntries.png"));
}

MenuComponents::~MenuComponents()
{
    delete newTable;
    delete openTable;
    delete saveTable;
    delete saveAsTable;
    delete exitApp;

    delete prevPage;
    delete nextPage;

    delete findEntries;
    delete removeEntries;
}
