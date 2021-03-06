#pragma once

#include "../model/studentabsencemodel.h"
#include "../view/HierarchicalHeaderView.h"
#include "../view/proxymodel.h"
#include "../controller/adddialog.h"
#include "../controller/finddialog.h"
#include "../controller/modelcontroller.h"
#include "../controller/removedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableView>
#include <QWidget>


class StudentAbsenceTableApp : public QMainWindow
{
    Q_OBJECT

public:
    StudentAbsenceTableApp(QWidget *parent = 0);
    ~StudentAbsenceTableApp();

    inline ModelController *getController() const
    { return controller; }
    inline void setController(ModelController *value)
    { controller = value; }

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool agreedToContinue();
    void setCurrentFileName(const QString &fileName);

protected slots:
    bool newFile();
    bool open();
    bool save();
    bool saveAs();

    void addEntry();
    void findEntry();
    void removeEntry();

private:
    void createMainWidget();
    void createToolBar();
    void createMenu();
    void createContextMenu();
    void setConnections();

    StudentAbsenceModel *model;
    ProxyModel *proxyModel;
    QTableView *view;
    HierarchicalHeaderView *header;
    ModelController *controller;

    QWidget *mainWidget;
    QMenuBar *menuBar;
    QMenu *contextMenu;
    QToolBar *toolBar;

    QString currentFileName;

    AddDialog *addDialog;
    FindDialog *findDialog;
    RemoveDialog *removeDialog;

    bool documentModified;

    QPushButton *prevPageBtn;
    QPushButton *nextPageBtn;
    QPushButton *firstPageBtn;
    QPushButton *lastPageBtn;
    QPushButton *goToPageBtn;
    QLineEdit *goToPageEdt;
    QLabel *pageLbl;
    QLabel *currentPageLbl;
    QPushButton *entriesPerPageBtn;
    QLineEdit *entriesPerPageEdt;
    QLabel *entriesPerPageLbl;
    QLabel *entriesLbl;
};
