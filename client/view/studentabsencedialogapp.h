#pragma once

#include "../view/proxymodel.h"
#include "../view/HierarchicalHeaderView.h"
#include "../controller/adddialog.h"
#include "../controller/finddialog.h"
#include "../controller/removedialog.h"

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableView>
#include <QWidget>

class StudentAbsenceClient;


class StudentAbsenceTableApp : public QMainWindow
{
    Q_OBJECT

public:
    StudentAbsenceTableApp(QWidget *parent = 0);
    ~StudentAbsenceTableApp();

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);

    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool agreedToContinue();

protected slots:
    bool newFile();
    bool open();
    bool save();
    bool saveAs();

    void showStartScreen();

    void addEntry();
    void findEntry();
    void removeEntry();

    void showConnectWindow();

    void disableTableView();
    void enableTableView();

private:
    void createMainWidget();
    void createToolBar();
    void createMenu();
    void createContextMenu();
    void setColumnsSize();
    void setConnections();

    ProxyModel *model;
    QTableView *view;
    HierarchicalHeaderView *header;
    StudentAbsenceClient *client;

    QWidget *mainWidget;
    QMenuBar *menuBar;
    QMenu *contextMenu;
    QToolBar *toolBar;

    AddDialog *addDialog;
    FindDialog *findDialog;
    RemoveDialog *removeDialog;

    bool documentModified;
    bool documentAvailable;

    QFrame *buttonsFrame;
    QLabel *modeLbl;
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
