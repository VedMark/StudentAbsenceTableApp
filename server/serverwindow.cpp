#include "serverwindow.h"
#include "server.h"

#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>


ServerWindow::ServerWindow(int nPort, QWidget *parent)
    : QWidget(parent)
{
    tcpServer = new Server(nPort, this);
    connect(tcpServer, SIGNAL( reqLog(QString) ), this, SLOT( appendLog(QString) ) );

    logEdt = new QTextEdit(this);

    logEdt->setCursor(QCursor(Qt::ArrowCursor));
    logEdt->setReadOnly(true);

    auto startBtn = new QPushButton(tr("&Запустить сервер"));
    auto stopBtn = new QPushButton(tr("&Остановить сервер"));

    connect(startBtn, SIGNAL( clicked(bool) ), tcpServer, SLOT( startServer() ) );
    connect(stopBtn, SIGNAL( clicked(bool) ), tcpServer, SLOT( stopServer() ) );

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

void ServerWindow::appendLog(QString text)
{
    logEdt->append(text);
}
