#pragma once

#include "clientsocket.h"

#include <QString>
#include <QWidget>

class QTcpServer;
class QTextEdit;


class StudentAbsenceServer : public QWidget
{
    Q_OBJECT

public:
    StudentAbsenceServer(int nPort, QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceServer();

public slots:
    virtual void hanleNewConnection();

private slots:
    void startServer();
    void stopServer();
    void appendLog(QString);

private:
    QTcpServer *tcpServer;
    QTextEdit *logEdt;
    qint64 port;
};
