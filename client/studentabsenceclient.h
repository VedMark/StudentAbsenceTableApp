#pragma once

#include <QTcpSocket>
#include <QWidget>

class QLineEdit;
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

class StudentAbsenceClient : public QWidget
{
    Q_OBJECT

public:
    StudentAbsenceClient(QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceClient();

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void connectToServer();
    void disconnectFromServer();
    void slotReadyRead();
    void handleError(QAbstractSocket::SocketError);
    void sendToServer();

private:
    void locateWidgets();

private:
    QTcpSocket *tcpSocket;
    QLineEdit *hostNameEdt;
    QLineEdit *portEdt;
    QTextEdit *logEdt;
    qint16 nextBlockSize;
};
