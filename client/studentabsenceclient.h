#pragma once

#include <QTcpSocket>
#include <QWidget>

#include "model/studentabsencemodel.h"

class QLineEdit;
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

class StudentAbsenceClient : public QWidget
{
    Q_OBJECT

public:
    StudentAbsenceClient(StudentAbsenceModel *model_, QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceClient();

public slots:
    void sendAddRequest(const StudentEntry& entry);
    void sendFindRequest(SearchPattern type, const QStringList& searchList);
    void sendRemoveRequest(SearchPattern type, const QStringList& searchList);
    void sendFilesRequest();
    void sendNewRequest();
    void sendSaveRequest();
    void sendOpenRequest();

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void filesReady();

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void connectToServer();
    void disconnectFromServer();
    void slotReadyRead();
    void handleError(QAbstractSocket::SocketError);

private:
    void locateWidgets();

private:
    QTcpSocket *tcpSocket;
    StudentAbsenceModel *model;
    QStringList filesList;
    QLineEdit *hostNameEdt;
    QLineEdit *portEdt;
    QTextEdit *logEdt;
    qint16 nextBlockSize;
};
