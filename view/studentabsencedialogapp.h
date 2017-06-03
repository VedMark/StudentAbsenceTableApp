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

    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    bool agreedToContinue();
    void setCurrentFileName(const QString &fileName);

signals:
    void indexerChanged();

protected slots:
    bool newFile();
    bool open();
    bool save();
    bool saveAs();

    bool addEntry();
    bool findEntry();
    bool removeEntry();

    bool showPrevPage();
    bool showNextPage();
    void enablePrevPage();
    void enableNextPage();
    void changePageIndexer(qint8);

private:
    void createToolBar();
    void createMenu();
    void createContextMenu();
    void setConnections();

    static qint8 ENTRIES_PER_PAGE;

    ModelController *controller;
    StudentAbsenceModel *model;
    QTableView *view;
    HierarchicalHeaderView *header;

    QMenuBar *menuBar;
    QMenu *contextMenu;
    QToolBar *toolBar;

    QString currentFileName;


    AddDialog *addDialog;

    qint64 pageIndexer;
};

#endif // QTABLE_H
