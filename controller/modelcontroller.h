#pragma once

#include "../model/studentabsencemodel.h"


class ModelController
{
public:
    ModelController(StudentAbsenceModel *controlModel = Q_NULLPTR);
    ~ModelController();

    StudentAbsenceModel *getModel() const;
    void setModel(StudentAbsenceModel *value);

    void clearModel();

    void addEntry(qint64 row, const StudentEntry &entry);

private:
    StudentAbsenceModel *model;
};
