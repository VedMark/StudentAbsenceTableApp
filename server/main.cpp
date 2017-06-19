#include "serverwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    ServerWindow server(2323);
    server.show();

    return app.exec();
}
