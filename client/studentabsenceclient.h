#pragma once

#include <QTcpSocket>
#include <QWidget>

class QLineEdit;
class QTextEdit;

class StudentAbsenceClient : public QWidget
{
    Q_OBJECT

public:
    StudentAbsenceClient(const QString &strHost, int nPort, QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceClient();

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void readyRead();
    void handleError(QAbstractSocket::SocketError);
    void sendToServer();
    void handleConnected();

private:
    QTcpSocket *tcpSocket;
    QTextEdit *logEdt;
    QLineEdit *inputEdt;
    qint64 nextBlockSize;
};
