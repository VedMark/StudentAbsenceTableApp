#pragma once

#include <QWidget>
#include <QDir>

class StudentAbsenceModel;
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
    OPEN
};
enum Answer{
    RESPONSE,
    MODEL,
    FILES,
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
    void sendModel(QTcpSocket *socket);
    void sendResponce(QTcpSocket *socket, const QString &text);
    void sendStringList(QTcpSocket *socket, const QStringList &text);

private slots:
    void startServer();
    void stopServer();

private:
    QTcpServer *tcpServer;
    StudentAbsenceModel *model;
    ModelController *controller;
    QDir dataDir;
    QTextEdit *logEdt;
    qint64 port;
    qint16 nextBlockSize;
};
