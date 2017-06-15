#include "studentabsencedialogapp.h"
#include "../controller/xmlparser.h"
#include "../controller/menucomponents.h"
#include "../studentabsenceclient.h"

#include <QContextMenuEvent>
#include <QFileDialog>
#include <QGraphicsColorizeEffect>
#include <QLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QStatusBar>
#include <QToolBar>


StudentAbsenceTableApp::StudentAbsenceTableApp(QWidget *parent)
    : QMainWindow(parent), addDialog(Q_NULLPTR), findDialog(Q_NULLPTR), removeDialog(Q_NULLPTR),
      client(Q_NULLPTR), workMode(LOCAL), documentModified(false)
{
    model = new StudentAbsenceModel(this);

    view = new QTableView(this);
    controller = new ModelController(model);
    proxyModel = new ProxyModel(this);
    proxyModel->setSourceModel(model);
    view->setModel(proxyModel);

    header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    header->setDefaultSectionSize(30);
    view->setHorizontalHeader(header);

    createMainWidget();
    setStatusBar(new QStatusBar(this));

    modeLbl = new QLabel("локальный режим", this);
    statusBar()->addPermanentWidget(modeLbl);

    createToolBar();
    createMenu();
    createContextMenu();

    setConnections();

    setMinimumSize(800, 600);
    setWindowIcon(QIcon(":images/spreadsheet.png"));
    setWindowTitle(tr("Таблица пропусков студентов"));
    showMaximized();
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

void StudentAbsenceTableApp::paintEvent(QPaintEvent *event)
{
    proxyModel->resetModel();
    QMainWindow::paintEvent(event);
}

void StudentAbsenceTableApp::resizeEvent(QResizeEvent *)
{
    view->setColumnWidth(0, width() / 3);
    view->setColumnWidth(1, width() / 6);
    view->setColumnWidth(2, width() / 8);
    view->setColumnWidth(3, width() / 8);
    view->setColumnWidth(4, width() / 6);
}

bool StudentAbsenceTableApp::newFile()
{
    if(!documentModified || agreedToContinue()){
        controller->clearModel();
        setCurrentFileName(tr(""));
        documentModified = false;
        return true;
    }
    if(workMode == NETWORK)
        client->sendNewRequest();
    return false;
}

bool StudentAbsenceTableApp::open()
{
    if(workMode == LOCAL){
        if(agreedToContinue()){
            auto openFileName = QFileDialog::getOpenFileName(this,
                                                    tr("Открыть файл"),
                                                    tr("/home/vedmark"),
                                                    tr("Файл данных (*.xml)"));
            if(!openFileName.isEmpty())
                return loadFile(openFileName);

        }
    }
    if(workMode == NETWORK){
        client->sendOpenRequest();
        return true;
    }
    return false;
}

bool StudentAbsenceTableApp::save()
{
    if(workMode == LOCAL){
        if(currentFileName.isEmpty())
            return saveAs();
        else
            return saveFile(currentFileName);
    }
    else{
        client->sendSaveRequest();
        return true;
    }
}

bool StudentAbsenceTableApp::saveAs()
{
    if(workMode == LOCAL){
        auto fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Сохранить файл"),
                                                    tr("/home/vedmark"),
                                                    tr("Файл данных (*.xml)"));
        if(fileName.isEmpty())
            return false;
        if(!fileName.endsWith(".xml"))
            fileName += ".xml";
        return saveFile(fileName);
    }
    else{
        client->sendSaveRequest();
        return true;
    }
}

bool StudentAbsenceTableApp::loadFile(const QString &fileName)
{
    auto xmlParser = XMLParser(model);
    try
    {
        xmlParser.read(fileName);
        setCurrentFileName(fileName);
        statusBar()->showMessage(tr("Файл загружен"), 2000);
        documentModified = false;
    }
    catch(FileOpenException)
    {
        QMessageBox::warning(this, tr("Ошибка!"),
                             tr("Ошибка чтения файла!"),
                             QMessageBox::Ok);
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }
    catch(FileReadException)
    {
        QMessageBox::warning(this, tr("Ошибка!"),
                             tr("Ошибка чтения файла!"),
                             QMessageBox::Ok);
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }

    proxyModel->showPage(1);
    return true;
}

bool StudentAbsenceTableApp::saveFile(const QString &fileName)
{
    auto xmlParser = XMLParser(model);
    try
    {
        xmlParser.write(fileName);

        setCurrentFileName(fileName);
        statusBar()->showMessage(tr("Файл сохранён"), 2000);
        documentModified = false;
    }
    catch(FileOpenException)
    {
        QMessageBox::warning(this, tr("Ошибка!"),
                             tr("Ошибка открытия файла!"),
                             QMessageBox::Ok);
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }
    catch(FileReadException)
    {
        QMessageBox::warning(this, tr("Ошибка!"),
                             tr("Ошибка открытия файла!"),
                             QMessageBox::Ok);
        statusBar()->showMessage(tr("Загрузка отменена"), 2000);
        return false;
    }

    return true;
}

bool StudentAbsenceTableApp::agreedToContinue()
{
    if (!documentModified)
         return true;
    auto answer = QMessageBox::warning(
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
    if(workMode == LOCAL)
        addDialog = new AddDialog(model, Q_NULLPTR, this);
    else
        addDialog = new AddDialog(model, client, this);
    addDialog->show();
    addDialog->raise();
    addDialog->activateWindow();
}

void StudentAbsenceTableApp::findEntry()
{
    if(findDialog != Q_NULLPTR)
        delete findDialog;
    if(workMode == LOCAL)
        findDialog = new FindDialog(model, Q_NULLPTR, this);
    else
        findDialog = new FindDialog(model, client, this);

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void StudentAbsenceTableApp::removeEntry()
{
    if(removeDialog != Q_NULLPTR)
        delete removeDialog;
    if(workMode == LOCAL)
        removeDialog = new RemoveDialog(model, Q_NULLPTR, this);
    else
        removeDialog = new RemoveDialog(model, client, this);

    removeDialog->show();
    removeDialog->raise();
    removeDialog->activateWindow();
}

void StudentAbsenceTableApp::showConnectWindow()
{
    if(client == Q_NULLPTR)
        client = new StudentAbsenceClient(model);
    client->show();

    connect(client, &StudentAbsenceClient::connectedToServer, [this] {
        workMode = NETWORK;
        modeLbl->setText("подключено к серверу");
    });
    connect(client, &StudentAbsenceClient::disconnectedFromServer, [this] {
        workMode = LOCAL;
        modeLbl->setText("локальный режим");
    });
}

void StudentAbsenceTableApp::createMainWidget()
{
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    prevPageBtn = new QPushButton(QIcon(":/images/prevPage.png"), "", this);
    prevPageBtn->addAction(MenuComponents::instance().prevPage);
    prevPageBtn->setToolTip(tr("Перейти на предыдущую страницу"));
    prevPageBtn->setEnabled(false);

    nextPageBtn = new QPushButton(QIcon(":/images/nextPage.png"), "", this);
    nextPageBtn->addAction(MenuComponents::instance().nextPage);
    nextPageBtn->setToolTip(tr("Перейти на следующую страницу"));
    nextPageBtn->setEnabled(false);

    firstPageBtn = new QPushButton(QIcon(":/images/firstPage.png"), "", this);
    firstPageBtn->addAction(MenuComponents::instance().firstPage);
    firstPageBtn->setToolTip(tr("Перейти на первую страницу"));
    firstPageBtn->setEnabled(false);

    lastPageBtn = new QPushButton(QIcon(":/images/lastPage.png"), "", this);
    lastPageBtn->addAction(MenuComponents::instance().lastPage);
    lastPageBtn->setToolTip(tr("Перейти на последнюю страницу"));
    lastPageBtn->setEnabled(false);

    goToPageBtn = new QPushButton(QIcon(":/images/goToPage.png"), "Перейти", this);
    goToPageBtn->addAction(MenuComponents::instance().goToPage);
    goToPageBtn->setToolTip(tr("Перейти на страницу"));
    goToPageBtn->setEnabled(true);
    goToPageEdt = new QLineEdit(this);
    goToPageEdt->setMaximumWidth(50);

    pageLbl = new QLabel(QString::number(proxyModel->maxPage()), this);
    currentPageLbl = new QLabel(QString::number(proxyModel->getPage()), this);

    entriesPerPageBtn = new QPushButton(QIcon(":/images/entriesPerPage.png"), "Изменить", this);
    entriesPerPageBtn->addAction(MenuComponents::instance().entriesPerPage);
    entriesPerPageBtn->setToolTip(tr("Изменить количество записей на странице"));
    entriesPerPageBtn->setEnabled(true);
    entriesPerPageEdt = new QLineEdit(this);
    entriesPerPageEdt->setMaximumWidth(50);
    entriesPerPageLbl = new QLabel(QString::number(proxyModel->getEntriesPerPage()), this);

    entriesLbl = new QLabel(QString::number(model->entriesSize()), this);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    buttonsLayout->addWidget(firstPageBtn);
    buttonsLayout->addWidget(prevPageBtn);
    buttonsLayout->addWidget(nextPageBtn);
    buttonsLayout->addWidget(lastPageBtn);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(new QLabel(tr("Страниц:"), this));
    buttonsLayout->addWidget(pageLbl);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(new QLabel(tr("Текущая:"), this));
    buttonsLayout->addWidget(currentPageLbl);
    buttonsLayout->addWidget(goToPageEdt);
    buttonsLayout->addWidget(goToPageBtn);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(new QLabel(tr("Записей:"), this));
    buttonsLayout->addWidget(entriesLbl);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(new QLabel(tr("Записей на странице:"), this));
    buttonsLayout->addWidget(entriesPerPageLbl);
    buttonsLayout->addWidget(entriesPerPageEdt);
    buttonsLayout->addWidget(entriesPerPageBtn);
    buttonsLayout->addStretch(20);

    setTabOrder(goToPageEdt, goToPageBtn);
    setTabOrder(goToPageBtn, entriesPerPageEdt);
    setTabOrder(entriesPerPageEdt, entriesPerPageBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(view);
    mainLayout->addLayout(buttonsLayout);

    centralWidget()->setLayout(mainLayout);
}

void StudentAbsenceTableApp::createToolBar()
{
    toolBar = new QToolBar(this);

    toolBar->addAction(MenuComponents::instance().newTable);
    toolBar->addAction(MenuComponents::instance().openTable);
    toolBar->addAction(MenuComponents::instance().saveTable);
    toolBar->addSeparator();
    toolBar->addAction(MenuComponents::instance().addEntries);
    toolBar->addAction(MenuComponents::instance().findEntries);
    toolBar->addAction(MenuComponents::instance().removeEntries);

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
    menuBar->addMenu(editMenu);

    QMenu *watchMenu = new QMenu(tr("&Просмотр"), menuBar);
    watchMenu->addAction(MenuComponents::instance().firstPage);
    watchMenu->addAction(MenuComponents::instance().prevPage);
    watchMenu->addAction(MenuComponents::instance().nextPage);
    watchMenu->addAction(MenuComponents::instance().lastPage);
    menuBar->addMenu(watchMenu);

    QMenu *serverMenu = new QMenu(tr("&Сервер"), menuBar);
    serverMenu->addAction(MenuComponents::instance().connectToServer);
    menuBar->addMenu(serverMenu);

    setMenuBar(menuBar);
}

void StudentAbsenceTableApp::createContextMenu()
{
    contextMenu = new QMenu(this);

    contextMenu->addAction(MenuComponents::instance().addEntries);
    contextMenu->addAction(MenuComponents::instance().findEntries);
    contextMenu->addAction(MenuComponents::instance().removeEntries);

    contextMenu->addSeparator();

    contextMenu->addAction(MenuComponents::instance().firstPage);
    contextMenu->addAction(MenuComponents::instance().prevPage);
    contextMenu->addAction(MenuComponents::instance().nextPage);
    contextMenu->addAction(MenuComponents::instance().lastPage);
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

    connect(MenuComponents::instance().prevPage, &QAction::triggered, [this] () {
        proxyModel->showPage(proxyModel->getPage() - 1);
        MenuComponents::instance().enableFirstPage(proxyModel->getPage() - 1 != 1);
        MenuComponents::instance().enablePrevPage(proxyModel->getPage() - 1 != 1);
        view->setFocus();
    });
    connect(MenuComponents::instance().nextPage, &QAction::triggered, [this] () {
        proxyModel->showPage(proxyModel->getPage() + 1);
        MenuComponents::instance().enableNextPage(proxyModel->getPage() + 1 < proxyModel->maxPage());
        MenuComponents::instance().enableLastPage(proxyModel->getPage() + 1 < proxyModel->maxPage());
        view->setFocus();
    });
    connect(MenuComponents::instance().firstPage, &QAction::triggered, [this] () {
        proxyModel->showPage(1);
        MenuComponents::instance().enableFirstPage(false);
        MenuComponents::instance().enablePrevPage(false);
        view->setFocus();
    });
    connect(MenuComponents::instance().lastPage, &QAction::triggered, [this] () {
        proxyModel->showPage(proxyModel->maxPage());
        MenuComponents::instance().enableNextPage(false);
        MenuComponents::instance().enableLastPage(false);
        view->setFocus();
    });
    connect(prevPageBtn, &QPushButton::clicked, [this] () {
        proxyModel->showPage(proxyModel->getPage() - 1);
        MenuComponents::instance().enableFirstPage(proxyModel->getPage() - 1 != 1);
        MenuComponents::instance().enablePrevPage(proxyModel->getPage() - 1 != 1);
        view->setFocus();
    });
    connect(nextPageBtn, &QPushButton::clicked, [this] () {
        proxyModel->showPage(proxyModel->getPage() + 1);
        MenuComponents::instance().enableNextPage(proxyModel->getPage() < proxyModel->maxPage());
        MenuComponents::instance().enableLastPage(proxyModel->getPage() < proxyModel->maxPage());
        view->setFocus();
    });
    connect(firstPageBtn, &QPushButton::clicked, [this] () {
        proxyModel->showPage(1);
        MenuComponents::instance().enableFirstPage(false);
        MenuComponents::instance().enablePrevPage(false);
        view->setFocus();
    });
    connect(lastPageBtn, &QPushButton::clicked, [this] () {
        proxyModel->showPage(proxyModel->maxPage());
        MenuComponents::instance().enableNextPage(false);
        MenuComponents::instance().enableLastPage(false);
        view->setFocus();
    });

    connect(MenuComponents::instance().connectToServer, SIGNAL( triggered(bool) ), SLOT( showConnectWindow() ) );

    std::function<void ()> enableButtons = [&] {
        if(proxyModel->getPage() >= proxyModel->maxPage()){
            proxyModel->showPage(proxyModel->maxPage());
            firstPageBtn->setEnabled(proxyModel->getPage() != 1);
            prevPageBtn->setEnabled(proxyModel->getPage() != 1);
            nextPageBtn->setEnabled(false);
            lastPageBtn->setEnabled(false);
        }
        else{
            firstPageBtn->setEnabled(proxyModel->getPage() != 1);
            prevPageBtn->setEnabled(proxyModel->getPage() != 1);
            nextPageBtn->setEnabled(true);
            lastPageBtn->setEnabled(true);
        }
        view->setFocus();
    };
    connect(model, &StudentAbsenceModel::rowsInserted, enableButtons);
    connect(model, &StudentAbsenceModel::rowsRemoved, enableButtons);

    connect(model, &StudentAbsenceModel::rowsInserted, [&] {
        entriesLbl->setText(QString::number(model->entriesSize()));
        pageLbl->setText(QString::number(proxyModel->maxPage()));
        currentPageLbl->setText(QString::number(proxyModel->getPage()));
    });
    connect(model, &StudentAbsenceModel::rowsRemoved, [&] {
        entriesLbl->setText(QString::number(model->entriesSize()));
        pageLbl->setText(QString::number(proxyModel->maxPage()));
        currentPageLbl->setText(QString::number(proxyModel->getPage()));
    });

    connect(proxyModel, &ProxyModel::entriesPerPageChanged, [&] (qint64 value) {
        entriesPerPageLbl->setText(QString::number(value));
        pageLbl->setText(QString::number(proxyModel->maxPage()));
    });

    connect(proxyModel, &ProxyModel::pageChanged, [&] (qint64 page) {
        firstPageBtn->setEnabled(page != 1);
        prevPageBtn->setEnabled(page != 1);
        nextPageBtn->setEnabled(page < proxyModel->maxPage());
        lastPageBtn->setEnabled(page < proxyModel->maxPage());
        currentPageLbl->setText(QString::number(page));
    });

    connect(goToPageBtn, &QPushButton::clicked, [&] {
        auto ok = true;
        auto num = goToPageEdt->text().toLongLong(&ok);
        if(!(ok) || num < 1 || num > proxyModel->maxPage()){
            auto effect = new QGraphicsColorizeEffect(goToPageBtn);
            effect->setColor(QColor(250, 0, 0));
            goToPageEdt->setGraphicsEffect(effect);
            return;
        }
        goToPageEdt->setText("");
        proxyModel->showPage(goToPageEdt->text().toLongLong());
        view->update(proxyModel->index(proxyModel->getEntriesPerPage(), model->LAST));
        view->setFocus();
    });
    connect(goToPageEdt, &QLineEdit::textChanged, [&] () {
        goToPageEdt->setGraphicsEffect(Q_NULLPTR);
    });

    connect(entriesPerPageBtn, &QPushButton::clicked, [&] {
        auto ok = true;
        qint64 num = entriesPerPageEdt->text().toLongLong(&ok);
        if(!(ok) || num < 1){
            auto effect = new QGraphicsColorizeEffect(entriesPerPageBtn);
            effect->setColor(QColor(250, 0, 0));
            entriesPerPageEdt->setGraphicsEffect(effect);
            return;
        }
        entriesPerPageEdt->setText("");
        auto clueEntryInd = proxyModel->getEntriesPerPage() * (proxyModel->getPage() - 1) + 1;
        proxyModel->setEntriesPerPage(entriesPerPageEdt->text().toLongLong());
        auto newPage = clueEntryInd / proxyModel->getEntriesPerPage() + 1;
        proxyModel->showPage(newPage);
        view->update(proxyModel->index(proxyModel->getEntriesPerPage(), model->LAST));
        view->setFocus();
    });
    connect(entriesPerPageEdt, &QLineEdit::textChanged, [&] () {
        entriesPerPageEdt->setGraphicsEffect(Q_NULLPTR);
    });

    connect(
        model, &StudentAbsenceModel::dataChanged,
        [this] () { documentModified = true; }
    );
}
