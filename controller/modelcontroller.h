#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QtCore>

#include "../model/studentabsencemodel.h"


class ModelController
{
public:
    ModelController(StudentAbsenceModel *controlModel = Q_NULLPTR);

    StudentAbsenceModel *getModel() const;
    void setModel(StudentAbsenceModel *value);

    void clearModel();

    void addEntry(qint64 row, const StudentEntry &entry);

private:
    StudentAbsenceModel *model;
};

#endif // MODELCONTROLLER_H
