#include <QContextMenuEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QStatusBar>
#include <QToolBar>
#include <iterator>

#include "studentabsencedialogapp.h"
#include "../controller/xmlparser.h"
#include "../controller/menucomponents.h"

StudentAbsenceTableApp::StudentAbsenceTableApp(QWidget *parent)
    : QMainWindow(parent)
{
    model = new StudentAbsenceModel(this);

    view = new StudentTableView(this);

    view->setEntriesPerPage(20);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setModel(model);
    setCentralWidget(view);

    controller = new ModelController(model);

    addDialog = Q_NULLPTR;
    findDialog = Q_NULLPTR;
    removeDialog = Q_NULLPTR;

    documentModified = false;

    createToolBar();
    createMenu();
    createContextMenu();

    setStatusBar(new QStatusBar(this));

    setConnections();

    setMinimumSize(800, 600);
    resize(maximumSize());

    setWindowTitle("Таблица пропусков студентов");
}

StudentAbsenceTableApp::~StudentAbsenceTableApp()
{
    delete controller;
}

void StudentAbsenceTableApp::contextMenuEvent(QContextMenuEvent *mouse_pointer)
{
    contextMenu->exec(mouse_pointer->globalPos());
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

    view->verticalHeader()->setDefaultSectionSize(31);
}

bool StudentAbsenceTableApp::newFile()
{
    if(agreedToContinue()){
        controller->clearModel();
        setCurrentFileName("");
        return true;
    }
    return false;
}

bool StudentAbsenceTableApp::open()
{
    if(agreedToContinue()){
        QString openFileName = QFileDialog::getOpenFileName(this,
                                                tr("Открыть файл"), "/home/vedmark",
                                                tr("Файл данных (*.xml)"));
        if(!openFileName.isEmpty())
            return loadFile(openFileName);

    }
    return false;
}

bool StudentAbsenceTableApp::save()
{
    if(currentFileName.isEmpty())
        return saveAs();
    else
        return saveFile(currentFileName);
}

bool StudentAbsenceTableApp::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                tr("Сохранить файл"), "/home/vedmark",
                                                tr("Файл данных (*.xml)"));
    if(fileName.isEmpty())
        return false;
    if(!fileName.endsWith(".xml"))
        fileName += ".xml";
    return saveFile(fileName);
}

bool StudentAbsenceTableApp::loadFile(const QString &fileName)
{
    XMLParser *xmlParser = new XMLParser(model);
    try
    {
        xmlParser->read(fileName);
        setCurrentFileName(fileName);
        statusBar()->showMessage(tr("Файл загружен"), 2000);
        documentModified = false;
    }
    catch(FileOpenException)
    {
        QMessageBox::warning(this, "Ошибка!", "Ошибка чтения файла!", QMessageBox::Ok);
        delete xmlParser;
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }
    catch(FileReadException)
    {
        QMessageBox::warning(this, "Ошибка!", "Ошибка чтения файла!", QMessageBox::Ok);
        delete xmlParser;
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }

    return true;
}

bool StudentAbsenceTableApp::saveFile(const QString &fileName)
{
    XMLParser *xmlParser = new XMLParser(model);
    try
    {       
        xmlParser->write(fileName);

        setCurrentFileName(fileName);
        statusBar()->showMessage(tr("Файл сохранён"), 2000);
        documentModified = false;
    }
    catch(FileOpenException)
    {
        QMessageBox::warning(this, "Ошибка!", "Ошибка открытия файла!", QMessageBox::Ok);
        delete xmlParser;
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }
    catch(FileReadException)
    {
        QMessageBox::warning(this, "Ошибка!", "Ошибка открытия файла!", QMessageBox::Ok);
        delete xmlParser;
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }

    delete xmlParser;
    return true;
}

bool StudentAbsenceTableApp::agreedToContinue()
{
    if (!documentModified)
         return true;
    QMessageBox::StandardButton answer = QMessageBox::warning(
                this,
                tr("Документ был изменён"),
                tr("Сохранить изменения?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (answer == QMessageBox::Save)
        return save();
    else if (answer == QMessageBox::Cancel)
        return false;
    return true;
}

void StudentAbsenceTableApp::setCurrentFileName(const QString &fileName)
{
    currentFileName = fileName;
}

void StudentAbsenceTableApp::addEntry()
{
    if(addDialog != Q_NULLPTR)
        delete addDialog;
    addDialog = new AddDialog(model, this);

    addDialog->show();
    addDialog->raise();
    addDialog->activateWindow();
}

void StudentAbsenceTableApp::findEntry()
{
    if(findDialog != Q_NULLPTR)
        delete findDialog;
    findDialog = new FindDialog(model, this);

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void StudentAbsenceTableApp::removeEntry()
{
    if(removeDialog != Q_NULLPTR)
        delete removeDialog;
    removeDialog = new RemoveDialog(model, this);

    removeDialog->show();
    removeDialog->raise();
    removeDialog->activateWindow();
}

void StudentAbsenceTableApp::createToolBar()
{
    toolBar = new QToolBar(this);

    toolBar->addAction(MenuComponents::instance().addEntries);
    toolBar->addAction(MenuComponents::instance().findEntries);
    toolBar->addAction(MenuComponents::instance().removeEntries);

    toolBar->addSeparator();

    toolBar->addAction(MenuComponents::instance().prevPage);
    toolBar->addAction(MenuComponents::instance().nextPage);
    MenuComponents::instance().prevPage->setDisabled(true);
    MenuComponents::instance().nextPage->setDisabled(true);

    addToolBar(toolBar);
}

void StudentAbsenceTableApp::createMenu()
{
    menuBar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu(tr("&Файл"), menuBar);
    fileMenu->addAction(MenuComponents::instance().newTable);
    fileMenu->addAction(MenuComponents::instance().openTable);
    fileMenu->addAction(MenuComponents::instance().saveTable);
    fileMenu->addAction(MenuComponents::instance().saveAsTable);
    fileMenu->addSeparator();
    fileMenu->addAction(MenuComponents::instance().exitApp);
    menuBar->addMenu(fileMenu);

    QMenu *editMenu = new QMenu(tr("&Правка"), menuBar);
    editMenu->addAction(MenuComponents::instance().addEntries);
    editMenu->addAction(MenuComponents::instance().findEntries);
    editMenu->addAction(MenuComponents::instance().removeEntries);
    editMenu->addSeparator();
    editMenu->addAction(MenuComponents::instance().prevPage);
    editMenu->addAction(MenuComponents::instance().nextPage);

    menuBar->addMenu(editMenu);

    setMenuBar(menuBar);
}

void StudentAbsenceTableApp::createContextMenu()
{
    contextMenu = new QMenu(this);

    contextMenu->addAction(MenuComponents::instance().addEntries);
    contextMenu->addAction(MenuComponents::instance().findEntries);
    contextMenu->addAction(MenuComponents::instance().removeEntries);

    contextMenu->addSeparator();

    contextMenu->addAction(MenuComponents::instance().prevPage);
    contextMenu->addAction(MenuComponents::instance().nextPage);
}

void StudentAbsenceTableApp::setConnections()
{
    connect(MenuComponents::instance().newTable, SIGNAL( triggered(bool) ), SLOT( newFile() ) );
    connect(MenuComponents::instance().openTable, SIGNAL( triggered(bool) ), SLOT( open() ) );
    connect(MenuComponents::instance().saveTable, SIGNAL( triggered(bool) ), SLOT( save() ) );
    connect(MenuComponents::instance().saveAsTable, SIGNAL( triggered(bool) ), SLOT( saveAs() ) );
    connect(MenuComponents::instance().exitApp, SIGNAL( triggered(bool) ), SLOT( close() ) );

    connect(MenuComponents::instance().addEntries, SIGNAL( triggered(bool) ), SLOT( addEntry() ) );
    connect(MenuComponents::instance().findEntries, SIGNAL( triggered(bool) ), SLOT( findEntry() ) );
    connect(MenuComponents::instance().removeEntries, SIGNAL( triggered(bool) ), SLOT( removeEntry() ) );

    connect(MenuComponents::instance().prevPage, SIGNAL( triggered(bool) ), view, SLOT( showPrevPage() ) );
    connect(MenuComponents::instance().nextPage, SIGNAL( triggered(bool) ), view, SLOT( showNextPage() ) );
    connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex, QVector<int>) ), view, SLOT( enableNextPage() ) );
    connect(
        model, &StudentAbsenceModel::dataChanged,
        [this] () { documentModified = true; }
    );
}
