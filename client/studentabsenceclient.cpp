#include "studentabsenceclient.h"
#include "controller/networkdialog.h"

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
#include <QStringList>
#include <QTextEdit>
#include <QTcpSocket>
#include <QTime>

StudentAbsenceClient::StudentAbsenceClient(ProxyModel *model_, QWidget *parent)
    : QDialog(parent), nextBlockSize(0), model(model_)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, [this] {
        connected = true;
        emit connectedToServer();
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" connected to server: ")
                       + tcpSocket->localAddress().toString());
        this->hide();
    });
    connect(tcpSocket, &QTcpSocket::disconnected, [this] {
        connected = false;
        emit disconnectedFromServer();
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" disconnected from server: ")
                       + tcpSocket->localAddress().toString());
    });
    connect(tcpSocket, SIGNAL( readyRead() ), this, SLOT( slotReadyRead() ) );
    connect(tcpSocket,  SIGNAL( error(QAbstractSocket::SocketError) ),
            this,       SLOT( handleError(QAbstractSocket::SocketError) ) );

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

    setMaximumSize(sizeHint());
    setMinimumSize(sizeHint());
    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
    setWindowIcon(QIcon(":/images/connectToServer.png"));
    setWindowTitle(tr("Клиент"));
}

StudentAbsenceClient::~StudentAbsenceClient()
{
    disconnect(this, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR);
}

void StudentAbsenceClient::sendAddRequest(const StudentEntry& entry)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << qint16(0) << qint16(ADD);

    out << entry.name.getSurname() << entry.name.getName() << entry.name.getPatronymic()
        << entry.group.getValue() << entry.absence.getIllness()
        << entry.absence.getAnother() << entry.absence.getHooky();

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
    sendEntriesRequest();
}

void StudentAbsenceClient::sendFindRequest(SearchPattern type, const QStringList& searchList)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint16(0) << qint16(SEARCH) << qint8(type) << searchList;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::sendRemoveRequest(SearchPattern type, const QStringList &searchList)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint16(0) << qint16(REMOVE) << qint8(type) << searchList;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
    sendEntriesRequest();
}

void StudentAbsenceClient::sendFilesRequest()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint16(0) << qint16(CHOOSE_FILE);

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));

    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::sendNewRequest()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint16(0) << qint16(NEW);

    setCurrentFileName("");
    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::sendSaveRequest()
{
    if(getCurrentFileName() != ""){
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_2);

        out << qint16(0) << qint16(SAVE) << getCurrentFileName();

        out.device()->seek(0);
        out << qint16(arrBlock.size() - sizeof(qint16));
        tcpSocket->write(arrBlock);
    }
    else
        sendSaveAsRequest();
}

void StudentAbsenceClient::sendSaveAsRequest()
{
    sendFilesRequest();
    connect(this, &StudentAbsenceClient::filesReady, [this] {
        auto networkDialog = new NetworkDialog(filesList, FOR_SAVE, this);
        networkDialog->show();
        connect(networkDialog, &NetworkDialog::fileChoosen, [this, networkDialog] (const QString& fileName) {
            setCurrentFileName(fileName);
            sendSaveRequest();

            networkDialog->deleteLater();
        });
        disconnect(this, &StudentAbsenceClient::filesReady, this, Q_NULLPTR);
    });
}

void StudentAbsenceClient::sendOpenRequest(qint64 begin, qint64 count)
{
    sendFilesRequest();
    connect(this, &StudentAbsenceClient::filesReady, [this, begin, count] {
        auto networkDialog = new NetworkDialog(filesList, FOR_OPEN, this);
        networkDialog->show();
        connect(networkDialog, &NetworkDialog::fileChoosen, [this, begin, count, networkDialog] (const QString& fileName) {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_2);

            out << qint16(0) << qint16(OPEN) << fileName << begin << count;

            setCurrentFileName(fileName);
            out.device()->seek(0);
            out << qint16(arrBlock.size() - sizeof(qint16));
            tcpSocket->write(arrBlock);

            networkDialog->deleteLater();
        });
        disconnect(this, &StudentAbsenceClient::filesReady, this, Q_NULLPTR);
    });
}

void StudentAbsenceClient::sendEntriesRequest()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);

    out << qint16(0) << qint16(ENTRIES)
        << qint64((model->getPage() - 1) * model->getEntriesPerPage())
        << qint64(model->getEntriesPerPage());

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::closeEvent(QCloseEvent *)
{
    hide();
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

    tcpSocket->connectToHost(hostNameEdt->text(), num);
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

        qint16 answerType;
        in >> answerType;
        switch ((Answer)answerType) {
        case RESPONSE:{
            QTime time;
            QString text;
            in >> time >> text;

            logEdt->append(time.toString() + " " + text);
            break;
        }
        case MODEL:{
            ProxyModel::Students studentList;
            QString surname = "";
            QString name = "";
            QString patronymic = "";
            QString group = "";
            qint16 illness = 0;
            qint16 another = 0;
            qint16 hooky = 0;
            forever{
                in >> surname >> name >> patronymic >> group
                   >> illness >> another >> hooky;
                if(in.atEnd()) break;
                studentList.append(StudentEntry(RussianFullName(surname, name, patronymic),
                                                Group(group),
                                                Absence(illness, another, hooky)));
            }
            model->setStudentEntryList(studentList);
            break;
        }
        case MODEL_SIZE:{
            qint64 size;
            in >> size;
            model->setSize(size);
            break;
        }
        case FILES:{
            filesList = QStringList();
            QString element;
            forever{
                in >> element;
                if(in.atEnd()) break;
                filesList.append(element);
            }
            emit filesReady();
            break;
        }
        case SEARCH_RES:{
            ProxyModel::Students studentList;
            QString surname = "";
            QString name = "";
            QString patronymic = "";
            QString group = "";
            qint16 illness = 0;
            qint16 another = 0;
            qint16 hooky = 0;
            forever{
                in >> surname >> name >> patronymic >> group
                   >> illness >> another >> hooky;
                if(in.atEnd()) break;
                studentList.append(StudentEntry(RussianFullName(surname, name, patronymic),
                                                Group(group),
                                                Absence(illness, another, hooky)));
            }
            emit searchResultReturned(studentList);
            break;
        }
        case REMOVE_RES:{
            qint64 count = 0;
            in >> count;
            emit removeResultReturned(count);
            break;
        }
        }

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

void StudentAbsenceClient::locateWidgets()
{
    auto connectBtn = new QPushButton(tr("&Подключиться"), this);
    connect(connectBtn, SIGNAL( clicked(bool) ), this, SLOT( connectToServer() ) );

    auto disconnectBtn = new QPushButton(tr("&Отключиться"), this);
    connect(disconnectBtn, SIGNAL( clicked(bool) ), this, SLOT( disconnectFromServer() ) );

    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(new QLabel(tr("Хост"), this));
    topLayout->addWidget(hostNameEdt);
    topLayout->addStretch();
    topLayout->addWidget(new QLabel(tr("Порт"), this));
    topLayout->addWidget(portEdt);

    auto topBtnLayout = new QHBoxLayout;
    topBtnLayout->addWidget(connectBtn);
    topBtnLayout->addWidget(disconnectBtn);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("<H1>Установка соединения</H1>"), this));
    mainLayout->addSpacing(20);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(topBtnLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(logEdt);
    setLayout(mainLayout);
}
