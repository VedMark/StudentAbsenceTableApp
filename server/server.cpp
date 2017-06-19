#include "server.h"
#include "clientsocket.h"

#include <QMessageBox>
#include <QNetworkInterface>
#include <QTime>
#include <QThread>

Server::Server(int nPort, QObject *parent)
    :QTcpServer(parent), port(nPort)
{}

void Server::startServer()
{
    auto ipAddressList = QNetworkInterface::allAddresses();
    foreach (QHostAddress ipAddress, ipAddressList) {
        if(ipAddress != QHostAddress::LocalHost && ipAddress.toIPv4Address()){
            if(!listen(ipAddress, port))
            {
                QMessageBox::critical(0,
                                      QStringLiteral("Server Error"),
                                      QStringLiteral("Unable to start the server:")
                                      + errorString());
                close();
                return;
            }
            emit reqLog(QTime::currentTime().toString() + QString(" server launched - address %1:%2")
                           .arg(serverAddress().toString())
                           .arg(QString::number(serverPort())));
        }
    }
}

void Server::stopServer()
{
    close();
    emit reqLog(QTime::currentTime().toString() + QStringLiteral(" server stopped"));
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    auto newClientSocket = new ClientSocket(socketDescriptor);
    auto clientThread = new QThread();
    newClientSocket->moveToThread(clientThread);

    connect(newClientSocket, SIGNAL( disconnected() ), newClientSocket, SLOT( deleteLater() ) );
    connect(newClientSocket, SIGNAL( readyRead() ), newClientSocket, SLOT( readClient() ) );
    connect(newClientSocket, &ClientSocket::reqLog, [this] (QString text) -> void {
        emit reqLog(text);
    });
    connect(newClientSocket, SIGNAL( disconnected() ), clientThread, SLOT( quit() ) );
    connect(clientThread, SIGNAL( finished() ), clientThread, SLOT( deleteLater() ) );
    connect(clientThread, SIGNAL( started() ), newClientSocket, SLOT( setConnection() ) );

    clientThread->start();
}
