#include "clientsocket.h"
#include "controller/modelcontroller.h"

#include <QByteArray>
#include <QDataStream>
#include <QDir>
#include <QString>
#include <QTime>

const QString ClientSocket::DATA_DIR_PATH = QStringLiteral("/media/bsuir/sem_4/PPvIS/server_data");

ClientSocket::ClientSocket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket(parent), nextBlockSize(0), socketDescriptor(socketDescriptor)
{
    controller = new ModelController(&model);
}

void ClientSocket::setConnection()
{
    if (!setSocketDescriptor(socketDescriptor)) {
        emit errorSignal(error());
        return;
    }
    localAddress_ = localAddress();
    localPort_ = localPort();
    sendResponce(QStringLiteral("Server response: Connected"));

    connect(this, &ClientSocket::disconnected, [this] {
        logInfo(QTime::currentTime().toString() + QString(" client (%1:%2) disconnected")
                .arg(localAddress_.toString())
                .arg(QString::number(localPort_)));
        deleteLater();
    });
    connect(this, SIGNAL( readyRead() ), this, SLOT( readClient() ) );

    emit logInfo(QTime::currentTime().toString() + QString(" client (%1:%2) connected")
                 .arg(localAddress().toString())
                 .arg(QString::number(localPort())));
}

void ClientSocket::readClient()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_2);

    forever{
        if(!nextBlockSize){
            if((quint64)bytesAvailable() < sizeof(qint16))
                break;
            in >> nextBlockSize;
        }
        if(bytesAvailable() < nextBlockSize)
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
            sendModelSize();
            break;
        }
        case SEARCH:{
            qint8 searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("find entries");
            auto result = controller->findEntries((SearchPattern)searchType, list);
            sendSearchResult(result);
            break;
        }
        case REMOVE:{
            qint8 searchType;
            QStringList list;
            in >> searchType >> list;
            requestString = QStringLiteral("remove entries");
            auto result = controller->removeEntries((SearchPattern)searchType, list);
            sendRemoveResult(result);
            sendModelSize();
            break;
        }
        case NEW:
            requestString = QStringLiteral("new table");
            controller->clearModel();
            sendModelSize();
            break;
        case CHOOSE_FILE:
            requestString = QStringLiteral("choose file");
            sendStringList();
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
            sendModelSize();
            sendEntries(beginInd, count);
            break;
        }
        case ENTRIES:{
            qint64 beginInd = 0, count = 0;
            in >> beginInd >> count;
            requestString = QStringLiteral("send entries");
            sendEntries(beginInd, count);
            break;
        }
        }
        emit logInfo(QTime::currentTime().toString()
                     + QString(" Client (%1:%2) has sent request: ")
                     .arg(localAddress_.toString())
                     .arg(QString::number(localPort_))
                     + requestString);
        nextBlockSize = 0;
        sendResponce(QStringLiteral("Server response: Received request"));
    }
}

void ClientSocket::sendResponce(const QString &text)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(RESPONSE) << QTime::currentTime() << text;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    write(arrBlock);
}

void ClientSocket::sendEntries(qint64 begin, qint64 count)
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

    write(arrBlock);
}

void ClientSocket::sendModelSize()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(MODEL_SIZE) << qint64(model.size());

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    write(arrBlock);
}

void ClientSocket::sendStringList()
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

    write(arrBlock);
}

void ClientSocket::sendSearchResult(const Students list)
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

    write(arrBlock);
}

void ClientSocket::sendRemoveResult(qint64 count)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(REMOVE_RES);
        out << count;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    write(arrBlock);
}
