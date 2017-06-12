#include "view/studentabsencedialogapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StudentAbsenceTableApp tableApp;
    tableApp.show();

    return app.exec();
}
