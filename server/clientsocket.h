#pragma once

#include "model/studentabsencemodel.h"

#include <QHostAddress>
#include <QTcpSocket>

class ModelController;

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

class ClientSocket: public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket(qintptr socketDescriptor, QObject *parent = Q_NULLPTR);

    Q_SLOT void setConnection();

public slots:
    void readClient();

signals:
    void errorSignal(QTcpSocket::SocketError socketError);
    void reqLog(QString);

private:
    void sendResponce(const QString &text);
    void sendEntries(qint64 begin, qint64 count);
    void sendModelSize();
    void sendStringList();
    void sendSearchResult(const Students list);
    void sendRemoveResult(qint64 count);


private:
    static const QString DATA_DIR_PATH;

    qintptr socketDescriptor;
    Students model;
    ModelController *controller;
    qint16 nextBlockSize;

    QHostAddress localAddress_;
    qint16 localPort_;
};
