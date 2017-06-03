#ifndef QTABLE_H
#define QTABLE_H


#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableView>

#include "HierarchicalHeaderView.h"
#include "../controller/adddialog.h"
#include "../controller/menucomponents.h"
#include "../controller/modelcontroller.h"
#include "../model/studentabsencemodel.h"

class FileException{};

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

private slots:
    bool newFile();
    bool open();
    bool save();
    bool saveAs();

    bool addEntry();
    bool findEntry();
    bool removeEntry();

    bool showPrevPage();
    bool showNextPage();

private:
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool agreedToContinue();
    void setCurrentFileName(const QString &fileName);

    void createToolBar();
    void createMenu();
    void createContextMenu();
    void setConnections();


    ModelController *controller;
    StudentAbsenceModel *model;
    QTableView *view;
    HierarchicalHeaderView *header;

    QMenuBar *menuBar;
    QMenu *contextMenu;
    QToolBar *toolBar;

    QString currentFileName;

    AddDialog *addDialog;
};

#endif // QTABLE_H
