#include "studentabsencedialogapp.h"
#include "../controller/xmlparser.h"
#include "../controller/menucomponents.h"
#include "../studentabsenceclient.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsColorizeEffect>
#include <QLayout>
#include <QMessageBox>
#include <QScrollBar>
#include <QStatusBar>
#include <QToolBar>


StudentAbsenceTableApp::StudentAbsenceTableApp(QWidget *parent)
    : QMainWindow(parent), addDialog(Q_NULLPTR), findDialog(Q_NULLPTR), removeDialog(Q_NULLPTR),
      client(Q_NULLPTR), documentModified(false), documentAvailable(false)
{
    model = new ProxyModel(this);

    view = new QTableView(this);

    header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setStretchLastSection(true);
    header->setDefaultSectionSize(30);
    view->setHorizontalHeader(header);
    view->horizontalHeader()->hide();

    view->setModel(model);

    createMainWidget();
    setStatusBar(new QStatusBar(this));

    modeLbl = new QLabel("отключено от сервера", this);
    statusBar()->addPermanentWidget(modeLbl);


    client = new StudentAbsenceClient(model, this);
    showConnectWindow();

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
{}

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
    setColumnsSize();
}

bool StudentAbsenceTableApp::newFile()
{
    if(client->isConnected()){
        if(agreedToContinue()){
            client->sendNewRequest();
            documentModified = false;
        }
        model->reset();
        enableTableView();
        return true;
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
    return false;
}

bool StudentAbsenceTableApp::open()
{
    if(client->isConnected()){
        if(agreedToContinue()){
            model->setPage(0);
            client->sendOpenRequest((model->getPage() - 1) * model->getEntriesPerPage(),
                                    model->getEntriesPerPage());
            documentModified = false;
        }
        return true;
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
    return false;
}

bool StudentAbsenceTableApp::save()
{
    if(client->isConnected()){
        if(documentAvailable){
            client->sendSaveRequest();
            documentModified = false;
        }
        else
            QMessageBox::information(this, tr("Внимание"),
                                     tr("Для совершения действия необходим открытый документ"),
                                     QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
    return true;
}

bool StudentAbsenceTableApp::saveAs()
{
    if(client->isConnected()){
        if(documentAvailable){
            client->sendSaveAsRequest();
            documentModified = false;
        }
        else
            QMessageBox::information(this, tr("Внимание"),
                                     tr("Для совершения действия необходим открытый документ"),
                                     QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
    return true;
}

void StudentAbsenceTableApp::showStartScreen()
{
    if(client->isConnected()){
        if(agreedToContinue())
            disableTableView();
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
}

void StudentAbsenceTableApp::disableTableView()
{
    model->reset();
    view->horizontalHeader()->hide();
    buttonsFrame->hide();
    documentModified = false;
    documentAvailable = false;
}

void StudentAbsenceTableApp::enableTableView()
{
    view->horizontalHeader()->show();
    setColumnsSize();
    buttonsFrame->show();
    documentAvailable = true;
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

void StudentAbsenceTableApp::addEntry()
{
    if(client->isConnected()){
        if(documentAvailable){
            addDialog = new AddDialog(model, client, this);
            addDialog->show();
            addDialog->raise();
            addDialog->activateWindow();

            connect(addDialog, &AddDialog::notNoneResult, [this] {
                 documentModified = true;
            });
        }
        else
            QMessageBox::information(this, tr("Внимание"),
                                     tr("Для совершения действия необходим открытый документ"),
                                     QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
}

void StudentAbsenceTableApp::findEntry()
{
    if(client->isConnected()){
        if(documentAvailable){
            findDialog = new FindDialog(model, client, this);
            findDialog->show();
            findDialog->raise();
            findDialog->activateWindow();
            connect(client, SIGNAL( searchResultReturned(ProxyModel::Students) ),
                    findDialog, SLOT( displayResult(ProxyModel::Students) ) );
        }
        else
            QMessageBox::information(this, tr("Внимание"),
                                     tr("Для совершения действия необходим открытый документ"),
                                     QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
}

void StudentAbsenceTableApp::removeEntry()
{
    if(client->isConnected()){
        if(documentAvailable){
            removeDialog = new RemoveDialog(model, client, this);
            removeDialog->show();
            removeDialog->raise();
            removeDialog->activateWindow();
            connect(client, SIGNAL( removeResultReturned(qint64) ),
                    removeDialog, SLOT( displayResult(qint64) ) );
            connect(removeDialog, &RemoveDialog::notNoneResult, [this] {
                 documentModified = true;
            });
        }
        else
            QMessageBox::information(this, tr("Внимание"),
                                     tr("Для совершения действия необходим открытый документ"),
                                     QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание"),
                                 tr("Невозможно совершить дейсвие:\n отсутствует соединение с сервером!"),
                                 QMessageBox::Ok);
}

void StudentAbsenceTableApp::showConnectWindow()
{
    client->show();
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

    pageLbl = new QLabel(QString::number(model->maxPage()), this);
    currentPageLbl = new QLabel(QString::number(model->getPage()), this);

    entriesPerPageBtn = new QPushButton(QIcon(":/images/entriesPerPage.png"), "Изменить", this);
    entriesPerPageBtn->addAction(MenuComponents::instance().entriesPerPage);
    entriesPerPageBtn->setToolTip(tr("Изменить количество записей на странице"));
    entriesPerPageBtn->setEnabled(true);
    entriesPerPageEdt = new QLineEdit(this);
    entriesPerPageEdt->setMaximumWidth(50);
    entriesPerPageLbl = new QLabel(QString::number(model->getEntriesPerPage()), this);

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

    buttonsFrame = new QFrame(this);
    buttonsFrame->setLayout(buttonsLayout);
    buttonsFrame->setFrameStyle(QFrame::NoFrame);
    buttonsFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    buttonsFrame->hide();

    setTabOrder(goToPageEdt, goToPageBtn);
    setTabOrder(goToPageBtn, entriesPerPageEdt);
    setTabOrder(entriesPerPageEdt, entriesPerPageBtn);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonsFrame);

    centralWidget()->setLayout(mainLayout);
}

void StudentAbsenceTableApp::createToolBar()
{
    toolBar = new QToolBar(this);

    toolBar->addAction(MenuComponents::instance().newTable);
    toolBar->addAction(MenuComponents::instance().openTable);
    toolBar->addAction(MenuComponents::instance().saveTable);
    toolBar->addAction(MenuComponents::instance().closeTable);
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
    fileMenu->addAction(MenuComponents::instance().closeTable);
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
    contextMenu->addAction(MenuComponents::instance().closeTable);

    contextMenu->addSeparator();

    contextMenu->addAction(MenuComponents::instance().firstPage);
    contextMenu->addAction(MenuComponents::instance().prevPage);
    contextMenu->addAction(MenuComponents::instance().nextPage);
    contextMenu->addAction(MenuComponents::instance().lastPage);
}

void StudentAbsenceTableApp::setColumnsSize()
{
    view->setColumnWidth(0, width() / 3);
    view->setColumnWidth(1, width() / 6);
    view->setColumnWidth(2, width() / 8);
    view->setColumnWidth(3, width() / 8);
    view->setColumnWidth(4, width() / 6);
}

void StudentAbsenceTableApp::setConnections()
{
    connect(MenuComponents::instance().newTable, SIGNAL( triggered(bool) ), SLOT( newFile() ) );
    connect(MenuComponents::instance().openTable, SIGNAL( triggered(bool) ), SLOT( open() ) );
    connect(MenuComponents::instance().saveTable, SIGNAL( triggered(bool) ), SLOT( save() ) );
    connect(MenuComponents::instance().saveAsTable, SIGNAL( triggered(bool) ), SLOT( saveAs() ) );
    connect(MenuComponents::instance().closeTable, SIGNAL( triggered(bool) ), SLOT( showStartScreen() ) );
    connect(MenuComponents::instance().exitApp, SIGNAL( triggered(bool) ), SLOT( close() ) );

    connect(MenuComponents::instance().addEntries, SIGNAL( triggered(bool) ), SLOT( addEntry() ) );
    connect(MenuComponents::instance().findEntries, SIGNAL( triggered(bool) ), SLOT( findEntry() ) );
    connect(MenuComponents::instance().removeEntries, SIGNAL( triggered(bool) ), SLOT( removeEntry() ) );

    connect(MenuComponents::instance().prevPage, &QAction::triggered, [this] () {
        model->setPage(model->getPage() - 1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(MenuComponents::instance().nextPage, &QAction::triggered, [this] () {
        model->setPage(model->getPage() + 1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(MenuComponents::instance().firstPage, &QAction::triggered, [this] () {
        model->setPage(1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(MenuComponents::instance().lastPage, &QAction::triggered, [this] () {
        model->setPage(model->maxPage());
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(prevPageBtn, &QPushButton::clicked, [this] () {
        model->setPage(model->getPage() - 1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(nextPageBtn, &QPushButton::clicked, [this] () {
        model->setPage(model->getPage() + 1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(firstPageBtn, &QPushButton::clicked, [this] () {
        model->setPage(1);
        client->sendEntriesRequest();
        view->setFocus();
    });
    connect(lastPageBtn, &QPushButton::clicked, [this] () {
        model->setPage(model->maxPage());
        client->sendEntriesRequest();
        view->setFocus();
    });

    connect(MenuComponents::instance().connectToServer, SIGNAL( triggered(bool) ), SLOT( showConnectWindow() ) );

    connect(model, &ProxyModel::entriesPerPageChanged, [&] (qint64 value) {
        entriesPerPageLbl->setText(QString::number(value));
        pageLbl->setText(QString::number(model->maxPage()));
        client->sendEntriesRequest();
    });
    connect(model, &ProxyModel::pageChanged, [&] (qint64 page) {
        firstPageBtn->setEnabled(page != 1);
        prevPageBtn->setEnabled(page != 1);
        nextPageBtn->setEnabled(page < model->maxPage());
        lastPageBtn->setEnabled(page < model->maxPage());
        currentPageLbl->setText(QString::number(page));
    });
    connect(model, &ProxyModel::sizeChanged, [this] (qint64 size) {
        entriesLbl->setText(QString::number(size));
        pageLbl->setText(QString::number(model->maxPage()));
        qint64 page = currentPageLbl->text().toLongLong();
        firstPageBtn->setEnabled(page != 1);
        prevPageBtn->setEnabled(page != 1);
        nextPageBtn->setEnabled(page < model->maxPage());
        lastPageBtn->setEnabled(page < model->maxPage());
    });
    connect(model, SIGNAL( dataChanged(QModelIndex,QModelIndex,QVector<int>) ), SLOT( enableTableView() ) );

    connect(goToPageBtn, &QPushButton::clicked, [&] {
        auto ok = true;
        auto num = goToPageEdt->text().toLongLong(&ok);
        if(!(ok) || num < 1 || num > model->maxPage()){
            auto effect = new QGraphicsColorizeEffect(goToPageBtn);
            effect->setColor(QColor(250, 0, 0));
            goToPageEdt->setGraphicsEffect(effect);
            return;
        }
        model->setPage(goToPageEdt->text().toLongLong());
        client->sendEntriesRequest();
        goToPageEdt->setText("");
        view->setFocus();
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
        auto clueEntryInd = model->getEntriesPerPage() * (model->getPage() - 1) + 1;
        model->setEntriesPerPage(entriesPerPageEdt->text().toLongLong());
        auto newPage = clueEntryInd / model->getEntriesPerPage() + 1;
        model->setPage(newPage);
        client->sendEntriesRequest();
        entriesPerPageEdt->setText("");
        view->setFocus();
    });
    connect(goToPageEdt, &QLineEdit::textChanged, [&] () {
        goToPageEdt->setGraphicsEffect(Q_NULLPTR);
    });
    connect(entriesPerPageEdt, &QLineEdit::textChanged, [&] () {
        entriesPerPageEdt->setGraphicsEffect(Q_NULLPTR);
    });

    connect(client, &StudentAbsenceClient::connectedToServer, [this] {
        modeLbl->setText("подключено к серверу");
    });
    connect(client, &StudentAbsenceClient::disconnectedFromServer, [this] {
        modeLbl->setText("отключено от сервера");
        disableTableView();
    });
}
