#include "studentabsenceserver.h"
#include "model/studentabsencemodel.h"
#include "controller/modelcontroller.h"

#include <QApplication>
#include <QByteArray>
#include <QCursor>
#include <QDataStream>
#include <QDesktopWidget>
#include <QDir>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>

const QString StudentAbsenceServer::DATA_DIR_PATH = QStringLiteral("/media/bsuir/sem_4/PPvIS/server_data");

StudentAbsenceServer::StudentAbsenceServer(int nPort, QWidget *parent)
    : QWidget(parent), port(nPort), nextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    controller = new ModelController(&model);

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
            controller->addEntry(StudentEntry(RussianFullName(surname, name, patronymic),
                                              Group(group),
                                              Absence(illness, another, hooky)));
            sendModelSize(clientSocket);
            break;
        }
        case SEARCH:{
            qint8 searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("find entries");
            auto result = controller->findEntries((SearchPattern)searchType, list);
            sendSearchResult(clientSocket, result);
            break;
        }
        case REMOVE:{
            qint8 searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("remove entries");
            auto result = controller->removeEntries((SearchPattern)searchType, list);
            sendRemoveResult(clientSocket, result);
            sendModelSize(clientSocket);
            break;
        }
        case NEW:
            requestString = QStringLiteral("new table");
            controller->clearModel();
            sendModelSize(clientSocket);
            break;
        case CHOOSE_FILE:
            requestString = QStringLiteral("choose file");
            sendStringList(clientSocket);
            break;
        case SAVE:
            in >> fileName;
            requestString = QStringLiteral("save table");
            controller->saveModel(DATA_DIR_PATH + QStringLiteral("/") + fileName);
            break;
        case OPEN:{
            in >> fileName;
            requestString = QStringLiteral("open table");
            controller->loadModel(DATA_DIR_PATH + QStringLiteral("/") + fileName);
            qint64 beginInd = 0, count = 0;
            in >> beginInd >> count;
            sendModelSize(clientSocket);
            sendEntries(clientSocket, beginInd, count);
            break;
        }
        case ENTRIES:{
            qint64 beginInd = 0, count = 0;
            in >> beginInd >> count;
            requestString = QStringLiteral("send entries");
            sendEntries(clientSocket, beginInd, count);
            break;
        }
        }
        logEdt->append(QTime::currentTime().toString()
                       + QStringLiteral(" Client has sent request: ")
                       + requestString);
        nextBlockSize = 0;
        sendResponce(clientSocket, QStringLiteral("Server response: Received request"));
    }
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

void StudentAbsenceServer::sendEntries(QTcpSocket *socket, qint64 begin, qint64 count)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(MODEL);
    for(qint64 i = begin; count > 0 && i < model.size(); --count, ++i){
        out << model.at(i).name.getSurname() << model.at(i).name.getName()
            << model.at(i).name.getPatronymic() << model.at(i).group.getValue()
            << model.at(i).absence.getIllness() << model.at(i).absence.getAnother()
            << model.at(i).absence.getHooky();
    }

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendModelSize(QTcpSocket *socket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(MODEL_SIZE) << qint64(model.size());

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendStringList(QTcpSocket *socket)
{
    QDir dataDir = QDir(DATA_DIR_PATH, QStringLiteral("*.xml"));
    dataDir.setFilter(QDir::Files);
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(FILES);
    foreach (const QString& text, dataDir.entryList()) {
        out << text;
    }

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendSearchResult(QTcpSocket *socket, const Students list)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(SEARCH_RES);
    foreach (const StudentEntry& entry, list) {
        out << entry.name.getSurname() << entry.name.getName()
            << entry.name.getPatronymic() << entry.group.getValue()
            << entry.absence.getIllness() << entry.absence.getAnother()
            << entry.absence.getHooky();
    }

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    socket->write(arrBlock);
}

void StudentAbsenceServer::sendRemoveResult(QTcpSocket *socket, qint64 count)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(REMOVE_RES);
        out << count;

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
