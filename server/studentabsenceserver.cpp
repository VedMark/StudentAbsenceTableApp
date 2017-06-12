#include "studentabsenceserver.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>

StudentAbsenceServer::StudentAbsenceServer(int nPort, QWidget *parent)
    : QWidget(parent), nextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any, nPort))
    {
        QMessageBox::critical(0,
                              QStringLiteral("Server Error"),
                              QStringLiteral("Unable to start the server:")
                              + tcpServer->errorString());
        tcpServer->close();
        return;
    }

    connect(tcpServer, SIGNAL( newConnection() ), this, SLOT( hanleNewConnection() ) );

    logEdt = new QTextEdit(this);

    logEdt->setCursor(QCursor(Qt::ArrowCursor));
    logEdt->setReadOnly(true);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(QStringLiteral("<H1>Сервер</H1>")));
    mainLayout->addWidget(logEdt);
    setLayout(mainLayout);

    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowTitle("Сервер");
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

}

void StudentAbsenceServer::sendToClient(QTcpSocket *socket, const QString &text)
{

}
