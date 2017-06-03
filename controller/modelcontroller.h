#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QtCore>

#include <QException>

class StudentAbsenceModel;



class InputError: public QException
{
public:
    void raise() const { throw *this; }
    InputError *clone() const { return new InputError(); }
};



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
