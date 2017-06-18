#pragma once

#include <QDialog>
#include <QTcpSocket>
#include <QWidget>

#include "view/proxymodel.h"

class QLineEdit;
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
class StudentAbsenceClient : public QDialog
{
    Q_OBJECT

public:
    StudentAbsenceClient(ProxyModel *model_, QWidget *parent = Q_NULLPTR);
    ~StudentAbsenceClient();

    inline void setCurrentFileName(const QString &fileName)
    { currentFileName = fileName; }
    inline const QString& getCurrentFileName()
    { return currentFileName; }

    inline bool isConnected() const
    { return connected; }

public slots:
    void sendAddRequest(const StudentEntry& entry);
    void sendFindRequest(SearchPattern type, const QStringList& searchList);
    void sendRemoveRequest(SearchPattern type, const QStringList& searchList);
    void sendFilesRequest();
    void sendNewRequest();
    void sendSaveRequest();
    void sendSaveAsRequest();
    void sendOpenRequest(qint64 begin, qint64 count);
    void sendEntriesRequest();

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void searchResultReturned(ProxyModel::Students);
    void removeResultReturned(qint64);

protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void connectToServer();
    void disconnectFromServer();
    void slotReadyRead();
    void handleError(QAbstractSocket::SocketError);

private:
    void locateWidgets();

    Q_SIGNAL void filesReady();

private:
    QTcpSocket *tcpSocket;
    ProxyModel *model;
    QStringList filesList;
    QLineEdit *hostNameEdt;
    QLineEdit *portEdt;
    QTextEdit *logEdt;
    qint16 nextBlockSize;

    QString currentFileName;

    bool connected;
};
