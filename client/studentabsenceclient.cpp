#include "studentabsenceclient.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTcpSocket>

StudentAbsenceClient::StudentAbsenceClient(const QString &strHost, int nPort, QWidget *parent)
    : QWidget(parent), nextBlockSize(0)
{
    tcpSocket = new QTcpSocket(this);

    tcpSocket->connectToHost(strHost, nPort);
    connect(tcpSocket, SIGNAL( connected() ), this, SLOT( handleConnected() ) );
    connect(tcpSocket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
    connect(tcpSocket,  SIGNAL( error(QAbstractSocket::SocketError) ),
            this,       SLOT( handleError(QAbstractSocket::SocketError) ) );

    logEdt = new QTextEdit();
    inputEdt = new QLineEdit(this);

    setCursor(Qt::ArrowCursor);
    logEdt->setReadOnly(true);

    auto sendBtn = new QPushButton(QStringLiteral("&Выслать"), this);
    connect(sendBtn, SIGNAL( clicked(bool) ), this, SLOT( sendToServer() ) );
    connect(inputEdt, SIGNAL( returnPressed() ), this, SLOT( sendToServer() ) );

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(QStringLiteral("<H1>Клиент</H1>"), this));
    mainLayout->addWidget(logEdt);
    mainLayout->addWidget(inputEdt);
    mainLayout->addWidget(sendBtn);
    setLayout(mainLayout);

    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowIcon(QIcon(":/images/connectToServer.png"));
    setWindowTitle(QStringLiteral("Клиент"));
}

StudentAbsenceClient::~StudentAbsenceClient()
{}

void StudentAbsenceClient::closeEvent(QCloseEvent *)
{
    this->~StudentAbsenceClient();
}

void StudentAbsenceClient::readyRead()
{

}

void StudentAbsenceClient::handleError(QAbstractSocket::SocketError)
{

}

void StudentAbsenceClient::sendToServer()
{

}

void StudentAbsenceClient::handleConnected()
{

}
