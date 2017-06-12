#include "studentabsenceserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StudentAbsenceServer server(2323);
    server.show();

    return app.exec();
}
