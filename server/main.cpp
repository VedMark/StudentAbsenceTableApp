#include "studentabsenceserver.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    StudentAbsenceServer server(2323);
    server.show();

    return app.exec();
}
