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

    addEntries = new QAction(QObject::tr("&Добавить"), Q_NULLPTR);
    addEntries->setIcon(QIcon(":/images/addEntries.png"));

    findEntries = new QAction(QObject::tr("&Найти"), Q_NULLPTR);
    findEntries->setIcon(QIcon(":/images/findEntries.png"));

    removeEntries = new QAction(QObject::tr("&Удалить"), Q_NULLPTR);
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
