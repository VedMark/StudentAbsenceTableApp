#pragma once

#include <QString>
#include <QWidget>

#include "model/studentabsencemodel.h"

class ModelController;

class QTcpServer;
class QTcpSocket;
class QTextEdit;

enum Request{
    ADD,
    SEARCH,
    REMOVE,
    NEW,
    CHOOSE_FILE,
    SAVE,
    OPEN,
    ENTRIES
};
enum Answer{
    RESPONSE,
    MODEL,
    MODEL_SIZE,
    FILES,
    SEARCH_RES,
    REMOVE_RES
};

class StudentAbsenceServer : public QWidget
{
    Q_OBJECT

public:
    StudentAbsenceServer(int nPort, QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceServer();

public slots:
    virtual void hanleNewConnection();
    void readClient();

private:
    void sendResponce(QTcpSocket *socket, const QString &text);
    void sendEntries(QTcpSocket *socket, qint64 begin, qint64 count);
    void sendModelSize(QTcpSocket *socket);
    void sendStringList(QTcpSocket *socket);
    void sendSearchResult(QTcpSocket *socket, const Students list);
    void sendRemoveResult(QTcpSocket * socket, qint64 count);

private slots:
    void startServer();
    void stopServer();

private:
    static const QString DATA_DIR_PATH;

    QTcpServer *tcpServer;
    Students model;
    ModelController *controller;
    QTextEdit *logEdt;
    qint64 port;
    qint16 nextBlockSize;
};
