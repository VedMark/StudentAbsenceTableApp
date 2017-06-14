#include "studentabsenceclient.h"

#include <QApplication>
#include <QCursor>
#include <QDataStream>
#include <QDesktopWidget>
#include <QGraphicsColorizeEffect>
#include <QHostAddress>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTime>
#include <QTcpSocket>

StudentAbsenceClient::StudentAbsenceClient(QWidget *parent)
    : QWidget(parent), nextBlockSize(0)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, [this] {
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" connected to server: ") + tcpSocket->localAddress().toString());
    });
    connect(tcpSocket, &QTcpSocket::disconnected, [this] {
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" disconnected from server: ") + tcpSocket->localAddress().toString());
    });

    logEdt = new QTextEdit();

    setCursor(Qt::ArrowCursor);
    logEdt->setReadOnly(true);

    hostNameEdt = new QLineEdit(this);
    portEdt = new QLineEdit(this);
    hostNameEdt->setMaximumWidth(100);
    portEdt->setMaximumWidth(100);
    connect(hostNameEdt, &QLineEdit::textChanged, [&] () {
        hostNameEdt->setGraphicsEffect(Q_NULLPTR);
    });
    connect(portEdt, &QLineEdit::textChanged, [&] () {
        portEdt->setGraphicsEffect(Q_NULLPTR);
    });

    locateWidgets();

    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowIcon(QIcon(":/images/connectToServer.png"));
    setWindowTitle(tr("Клиент"));
}

StudentAbsenceClient::~StudentAbsenceClient()
{}

void StudentAbsenceClient::closeEvent(QCloseEvent *)
{
    this->~StudentAbsenceClient();
}

void StudentAbsenceClient::connectToServer()
{
    auto ok = true;
    qint64 num = portEdt->text().toShort(&ok);
    if(!(ok) || num < 1){
        auto effect = new QGraphicsColorizeEffect(portEdt);
        effect->setColor(QColor(250, 0, 0));
        portEdt->setGraphicsEffect(effect);
        return;
    }

    tcpSocket->connectToHost(hostNameEdt->text(), portEdt->text().toShort());

    connect(tcpSocket, SIGNAL( readyRead() ), this, SLOT( slotReadyRead() ) );
    connect(tcpSocket,  SIGNAL( error(QAbstractSocket::SocketError) ),
            this,       SLOT( handleError(QAbstractSocket::SocketError) ) );
}

void StudentAbsenceClient::disconnectFromServer()
{
    tcpSocket->disconnectFromHost();
}

void StudentAbsenceClient::slotReadyRead()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_2);
    forever{
        if(!nextBlockSize){
            if(tcpSocket->bytesAvailable() < sizeof(qint16))
                break;
            in >> nextBlockSize;
        }
        if(tcpSocket->bytesAvailable() < nextBlockSize)
            break;

        QTime time;
        QString text;
        in >> time >> text;

        logEdt->append(time.toString() + " " +  text);
        nextBlockSize = 0;
    }
}

void StudentAbsenceClient::handleError(QAbstractSocket::SocketError err)
{
    QString errString =
            QStringLiteral("Error: ") +
                         (err == QAbstractSocket::HostNotFoundError ?
                         QStringLiteral("The host not found.") :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         QStringLiteral("The remote host is closed.") :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         QStringLiteral("The connection was refused") :
                         tcpSocket->errorString());

    logEdt->append(errString);
}

void StudentAbsenceClient::sendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    //out << qint16(0) << QTime::currentTime() << inputEdt->text();

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::locateWidgets()
{
    auto connectBtn = new QPushButton(tr("&Подключиться"), this);
    connect(connectBtn, SIGNAL( clicked(bool) ), this, SLOT( connectToServer() ) );

    auto disconnectBtn = new QPushButton(tr("&Отключиться"), this);
    connect(disconnectBtn, SIGNAL( clicked(bool) ), this, SLOT( disconnectFromServer() ) );\

    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel(tr("Хост"), this));
    topLayout->addWidget(hostNameEdt);
    topLayout->addWidget(new QLabel(tr("Порт"), this));
    topLayout->addWidget(portEdt);

    auto topBtnLayout = new QHBoxLayout;
    topBtnLayout->addWidget(connectBtn);
    topBtnLayout->addWidget(disconnectBtn);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("<H1>Клиент</H1>"), this));
    mainLayout->addSpacing(20);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(topBtnLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(logEdt);
    setLayout(mainLayout);
}
