#include "studentabsenceserver.h"
#include "model/studentabsencemodel.h"
#include "controller/modelcontroller.h"

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


StudentAbsenceServer::StudentAbsenceServer(int nPort, QWidget *parent)
    : QWidget(parent), port(nPort), nextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    model = new StudentAbsenceModel(this);
    controller = new ModelController(model);
    dataDir = QDir(QStringLiteral("/media/bsuir/sem_4/PPvIS/server_data"), QStringLiteral("*.xml"));
    dataDir.setFilter(QDir::Files);

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

    logEdt->append(QStringLiteral("Client connected - ") + clientSocket->localAddress().toString());
    sendResponce(clientSocket, QStringLiteral("Server response: Connected"));
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


        qint16 requestType = 0;
        QString requestString = "";
        QString surname = "";
        QString name = "";
        QString patronymic = "";
        QString group = "";
        qint16 illness = 0;
        qint16 another = 0;
        qint16 hooky = 0;
        QString fileName = "";
        in >> requestType;

        switch((Request)requestType)
        {
        case ADD:{
            in >> surname >> name >> patronymic >> group
               >> illness >> another >> hooky;
            requestString = QStringLiteral("add entry");
            controller->addEntry(model->rowCount(), StudentEntry(RussianFullName(surname, name, patronymic),
                                                                 Group(group),
                                                                 Absence(illness, another, hooky)));
            break;
        }
        case SEARCH:{
            QString searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("find entries");
            controller->findEntries((SearchPattern)searchType.toInt(), list);
            break;
        }
        case REMOVE:{
            QString searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("remove entries");
            controller->removeEntries((SearchPattern)searchType.toInt(), list);
            break;
        }
        case NEW:
            requestString = QStringLiteral("new table");
            controller->clearModel();
            break;
        case CHOOSE_FILE:
            requestString = QStringLiteral("choose file");
            sendStringList(clientSocket, dataDir.entryList());
            break;
        case SAVE:
            in >> fileName;
            requestString = QStringLiteral("save table");
            controller->saveModel(dataDir.absolutePath() + QStringLiteral("/") + fileName);
        case OPEN:
            in >> fileName;
            requestString = QStringLiteral("open table");
            controller->loadModel(dataDir.absolutePath() + QStringLiteral("/") + fileName);
            sendModel(clientSocket);
            break;
        }

        logEdt->append(QTime::currentTime().toString()
                       + QStringLiteral(" Client has sent request: ")
                       + requestString);
        nextBlockSize = 0;
        sendResponce(clientSocket, QStringLiteral("Server response: Received request"));
    }
}

void StudentAbsenceServer::sendModel(QTcpSocket *socket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(MODEL);
    foreach (const StudentEntry& entry, model->getStudentEntryList()) {
        out << entry.name.getSurname() << entry.name.getName() << entry.name.getPatronymic()
            << entry.group.getValue() << entry.absence.getIllness()
            << entry.absence.getAnother() << entry.absence.getHooky();
    }

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendResponce(QTcpSocket *socket, const QString &text)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(RESPONSE) << QTime::currentTime() << text;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendStringList(QTcpSocket *socket, const QStringList &list)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(FILES);
    foreach (const QString& text, list) {
        out << text;
    }

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
