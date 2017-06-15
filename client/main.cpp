#include "view/studentabsencedialogapp.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    StudentAbsenceTableApp tableApp;
    tableApp.show();

    return app.exec();
}
