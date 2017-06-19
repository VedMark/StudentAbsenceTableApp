#pragma once

#include <QTcpServer>

class Server: public QTcpServer
{
    Q_OBJECT

public:
    Server(int nPort, QObject *parent);

    Q_SLOT void startServer();
    Q_SLOT void stopServer();


protected:
    void incomingConnection(qintptr);

    Q_SIGNAL void reqLog(QString);

private:
    int port;
};
