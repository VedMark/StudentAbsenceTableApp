#include "studentabsenceserver.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTime>
#include <QTcpServer>
#include <QTextEdit>
#include <QThread>


StudentAbsenceServer::StudentAbsenceServer(int nPort, QWidget *parent)
    : QWidget(parent), port(nPort)
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

    setFixedSize(400, 400);
    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowTitle(tr("Сервер"));
}

StudentAbsenceServer::~StudentAbsenceServer()
{}

void StudentAbsenceServer::hanleNewConnection()
{
    auto newClientSocket = new ClientSocket(tcpServer->nextPendingConnection()->socketDescriptor());
    auto clientThread = new QThread();

    connect(newClientSocket, SIGNAL( disconnected() ), clientThread, SLOT( quit() ) );
    connect(newClientSocket, SIGNAL( logInfo(QString) ), this, SLOT( appendLog(QString) ) );
    connect(clientThread, SIGNAL( finished() ), clientThread, SLOT( deleteLater() ) );

    newClientSocket->setConnection();
    newClientSocket->moveToThread(clientThread);
    clientThread->start();
}

void StudentAbsenceServer::startServer()
{
    if(!tcpServer->listen(QHostAddress::LocalHost, port))
    {
        QMessageBox::critical(0,
                              QStringLiteral("Server Error"),
                              QStringLiteral("Unable to start the server:")
                              + tcpServer->errorString());
        tcpServer->close();
        return;
    }
    logEdt->append(QTime::currentTime().toString() + QString(" server launched - address %1:%2")
                   .arg(tcpServer->serverAddress().toString())
                   .arg(QString::number(tcpServer->serverPort())));

    connect(tcpServer, SIGNAL( newConnection() ), this, SLOT( hanleNewConnection() ) );
}

void StudentAbsenceServer::stopServer()
{
    tcpServer->close();
    logEdt->append(QTime::currentTime().toString() + QStringLiteral(" server stopped"));
}

void StudentAbsenceServer::appendLog(QString text)
{
    logEdt->append(text);
}
