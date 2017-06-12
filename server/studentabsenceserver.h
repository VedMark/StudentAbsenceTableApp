#pragma once

#include <QWidget>

class QTcpServer;
class QTcpSocket;
class QTextEdit;


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

private:
    QTcpServer *tcpServer;
    QTextEdit *logEdt;
    qint64 nextBlockSize;
};
