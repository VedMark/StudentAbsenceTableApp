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

StudentAbsenceClient::StudentAbsenceClient(StudentAbsenceModel *model_, QWidget *parent)
    : QWidget(parent), nextBlockSize(0), model(model_)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, [this] {
        emit connectedToServer();
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" connected to server: ") + tcpSocket->localAddress().toString());
    });
    connect(tcpSocket, &QTcpSocket::disconnected, [this] {
        emit disconnectedFromServer();
        logEdt->append(QTime::currentTime().toString() + QStringLiteral(" disconnected from server: ") + tcpSocket->localAddress().toString());
    });

    logEdt = new QTextEdit();

    setCursor(Qt::ArrowCursor);
    logEdt->setReadOnly(true);

    hostNameEdt = new QLineEdit(this);
    portEdt = new QLineEdit(this);
    hostNameEdt->setMaximumWidth(100);
    hostNameEdt->setText("localhost");
    portEdt->setMaximumWidth(100);
    portEdt->setText("2323");
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
    out << qint16(0) << qint16(ADD);

    out << qint16(0) << qint16(REMOVE) << qint8(type) << searchList;

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
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

    out.device()->seek(0);
    out << qint16(arrBlock.size() - sizeof(qint16));
    tcpSocket->write(arrBlock);
}

void StudentAbsenceClient::sendSaveRequest()
{
    sendFilesRequest();
    connect(this, &StudentAbsenceClient::filesReady, [this] {
        auto networkDialog = new NetworkDialog(filesList, FOR_SAVE, this);
        connect(networkDialog, &NetworkDialog::fileChoosen, [this] (const QString& fileName) {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_2);
            out << qint16(0) << qint16(SAVE) << fileName;

            out.device()->seek(0);
            out << qint16(arrBlock.size() - sizeof(qint16));
            tcpSocket->write(arrBlock);

            disconnect(sender());
        });
        disconnect(this, &StudentAbsenceClient::filesReady, this, Q_NULLPTR);
    });

}

void StudentAbsenceClient::sendOpenRequest()
{
    sendFilesRequest();
    connect(this, &StudentAbsenceClient::filesReady, [this] {
        auto networkDialog = new NetworkDialog(filesList, FOR_OPEN, this);
        networkDialog->show();
        connect(networkDialog, &NetworkDialog::fileChoosen, [this] (const QString& fileName) {
            QByteArray arrBlock;
            QDataStream out(&arrBlock, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_2);
            out << qint16(0) << qint16(OPEN) << fileName;

            out.device()->seek(0);
            out << qint16(arrBlock.size() - sizeof(qint16));
            tcpSocket->write(arrBlock);

            disconnect(sender());
        });
        disconnect(this, &StudentAbsenceClient::filesReady, this, Q_NULLPTR);
    });
}

void StudentAbsenceClient::closeEvent(QCloseEvent *)
{
    hide();
}

void StudentAbsenceClient::connectToServer()
{
    this->hide();
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
    hide();
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
            StudentAbsenceModel::students studentList;
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
        case FILES:{
            filesList = QStringList();
            QString element;
            while(!in.atEnd()){
                in >> element;
                filesList.append(element);
            }
            emit filesReady();
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
