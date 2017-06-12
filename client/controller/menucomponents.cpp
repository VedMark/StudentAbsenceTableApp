#include "menucomponents.h"


MenuComponents *MenuComponents::p_instance = Q_NULLPTR;

MenuComponents &MenuComponents::instance()
{
    static MenuComponents m_instance;
    return m_instance;
}

MenuComponents::MenuComponents()
{
    newTable = new QAction(QObject::tr("&Новый"), Q_NULLPTR);
    newTable->setShortcut(QKeySequence::New);
    newTable->setIcon(QIcon(":/images/new.png"));

    openTable = new QAction(QObject::tr("&Открыть"), Q_NULLPTR);
    openTable->setShortcut(QKeySequence::Open);
    openTable->setIcon(QIcon(":/images/open.png"));

    saveTable = new QAction(QObject::tr("&Сохранить"), Q_NULLPTR);
    saveTable->setShortcut(QKeySequence::Save);
    saveTable->setIcon(QIcon(":/images/save.png"));

    saveAsTable = new QAction(QObject::tr("&Сохранить как"), Q_NULLPTR);
    saveAsTable->setShortcut(QKeySequence::SaveAs);

    exitApp = new QAction(QObject::tr("&Выйти"), Q_NULLPTR);
    exitApp->setShortcut(QKeySequence::Quit);
    exitApp->setIcon(QIcon(":/images/exit.png"));

    prevPage = new QAction(QObject::tr("&Предыдущая страница"), Q_NULLPTR);
    prevPage->setShortcut(QKeySequence::Back);
    prevPage->setIcon(QIcon(":/images/prevPage.png"));

    nextPage = new QAction(QObject::tr("&Следущая страница"), Q_NULLPTR);
    nextPage->setShortcut(QKeySequence::Forward);
    nextPage->setIcon(QIcon(":/images/nextPage.png"));

    firstPage = new QAction(QObject::tr("&Первая страница"), Q_NULLPTR);
    firstPage->setIcon(QIcon(":/images/firstPage.png"));

    lastPage = new QAction(QObject::tr("&Последняя страница"), Q_NULLPTR);
    lastPage->setIcon(QIcon(":/images/lastPage.png"));

    goToPage = new QAction(QObject::tr("&Перейти к странице"), Q_NULLPTR);

    entriesPerPage = new QAction(QObject::tr("&Изменить количество записей на странице"), Q_NULLPTR);

    addEntries = new QAction(QObject::tr("&Добавить"), Q_NULLPTR);
    addEntries->setIcon(QIcon(":/images/addEntries.png"));

    findEntries = new QAction(QObject::tr("&Найти"), Q_NULLPTR);
    findEntries->setIcon(QIcon(":/images/findEntries.png"));

    removeEntries = new QAction(QObject::tr("&Удалить"), Q_NULLPTR);
    removeEntries->setIcon(QIcon(":/images/removeEntries.png"));

    connectToServer = new QAction(QObject::tr("&Соединить"), Q_NULLPTR);
    connectToServer->setIcon(QIcon(":/images/connectToServer.png"));
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
    delete firstPage;
    delete lastPage;
    delete goToPage;
    delete entriesPerPage;

    delete addEntries;
    delete findEntries;
    delete removeEntries;

    delete connectToServer;
}

void MenuComponents::enablePrevPage(bool valid)
{
    prevPage->setEnabled(valid);
}

void MenuComponents::enableNextPage(bool valid)
{
    nextPage->setEnabled(valid);
}

void MenuComponents::enableFirstPage(bool valid)
{
    firstPage->setEnabled(valid);
}

void MenuComponents::enableLastPage(bool valid)
{
    lastPage->setEnabled(valid);
}
