#ifndef QTABLE_H
#define QTABLE_H


#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableView>

#include "../model/studentabsencemodel.h"
#include "../view/studenttableview.h"
#include "../controller/adddialog.h"
#include "../controller/finddialog.h"
#include "../controller/removedialog.h"
#include "../controller/modelcontroller.h"

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
    void createToolBar();
    void createMenu();
    void createContextMenu();
    void setConnections();

    ModelController *controller;
    StudentAbsenceModel *model;
    StudentTableView *view;

    QMenuBar *menuBar;
    QMenu *contextMenu;
    QToolBar *toolBar;

    QString currentFileName;


    AddDialog *addDialog;
    FindDialog *findDialog;
    RemoveDialog *removeDialog;
};

#endif // QTABLE_H
