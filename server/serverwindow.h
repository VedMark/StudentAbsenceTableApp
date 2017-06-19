#pragma once

#include <QString>
#include <QWidget>

class Server;
class QTextEdit;


class ServerWindow : public QWidget
{
    Q_OBJECT

public:
    ServerWindow(int nPort, QWidget *parent = Q_NULLPTR);

private slots:
    void appendLog(QString);

private:
    Server *tcpServer;
    QTextEdit *logEdt;
};
