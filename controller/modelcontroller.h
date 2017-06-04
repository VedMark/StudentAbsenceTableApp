#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QtCore>

class StudentAbsenceModel;


class ModelController
{
public:
    ModelController(StudentAbsenceModel *controlModel = Q_NULLPTR);

    StudentAbsenceModel *getModel() const;
    void setModel(StudentAbsenceModel *value);

    void clearModel();

private:
    StudentAbsenceModel *model;
};

#endif // MODELCONTROLLER_H
