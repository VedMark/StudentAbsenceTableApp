#include "studentabsenceserver.h"

#include <QApplication>
#include <QByteArray>
#include <QCursor>
#include <QDataStream>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>

#include <QDebug>

StudentAbsenceServer::StudentAbsenceServer(int nPort, QWidget *parent)
    : QWidget(parent), port(nPort), nextBlockSize(0)
{
    tcpServer = new QTcpServer(this);

    logEdt = new QTextEdit(this);

    logEdt->setCursor(QCursor(Qt::ArrowCursor));
    logEdt->setReadOnly(true);

    auto startBtn = new QPushButton(tr("&Запустить сервер"));
    auto stopBtn = new QPushButton(tr("&Остановить сервер"));

    connect(startBtn, SIGNAL( clicked(bool) ), this, SLOT( startServer() ) );
    connect(stopBtn, SIGNAL( clicked(bool) ), this, SLOT( stopServer() ) );

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("<H1>Сервер</H1>")));
    mainLayout->addWidget(logEdt);
    mainLayout->addWidget(startBtn);
    mainLayout->addWidget(stopBtn);
    setLayout(mainLayout);

    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowTitle(tr("Сервер"));
}

StudentAbsenceServer::~StudentAbsenceServer()
{}

void StudentAbsenceServer::hanleNewConnection()
{
    auto clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, SIGNAL( disconnected() ), clientSocket, SLOT( deleteLater() ) );
    connect(clientSocket, SIGNAL( readyRead() ), this, SLOT( readClient() ) );

    sendToClient(clientSocket, QStringLiteral("Server response: Connected"));
}

void StudentAbsenceServer::readClient()
{
    auto clientSocket = static_cast<QTcpSocket*>(sender());
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_5_2);

    forever{
        if(!nextBlockSize){
            if(clientSocket->bytesAvailable() < sizeof(qint16))
                break;
            in >> nextBlockSize;
        }

        if(clientSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString text;

        in >> time >> text;

        logEdt->append(time.toString() + " " + "Client has sent request");

        nextBlockSize = 0;

        sendToClient(clientSocket, QStringLiteral("Server response: Received request"));
    }
}

void StudentAbsenceServer::sendToClient(QTcpSocket *socket, const QString &text)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << QTime::currentTime() << text;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::startServer()
{
    if(!tcpServer->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(0,
                              QStringLiteral("Server Error"),
                              QStringLiteral("Unable to start the server:")
                              + tcpServer->errorString());
        tcpServer->close();
        return;
    }
    logEdt->append(QTime::currentTime().toString() + QStringLiteral(" server launched"));
    connect(tcpServer, SIGNAL( newConnection() ), this, SLOT( hanleNewConnection() ) );
}

void StudentAbsenceServer::stopServer()
{
    tcpServer->close();
    logEdt->append(QTime::currentTime().toString() + QStringLiteral(" server stopped"));
}
