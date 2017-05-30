#include "view/studentabsencedialogapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StudentAbsenceTableApp w;
    w.show();

    return a.exec();
}
