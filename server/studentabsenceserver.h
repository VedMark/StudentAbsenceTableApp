#pragma once

#include <QWidget>

#include "model/studentabsencemodel.h"

class QTcpServer;
class QTcpSocket;
class QTextEdit;

enum request{
    READ,
    ADD,
    SEARCH,
    REMOVE,
    NEW,
    SAVE,
    OPEN
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
    void sendToClient(QTcpSocket *socket, const QString &text);

private slots:
    void startServer();
    void stopServer();

private:
    QTcpServer *tcpServer;
    StudentAbsenceModel *model;
    QTextEdit *logEdt;
    qint64 port;
    qint16 nextBlockSize;
};
