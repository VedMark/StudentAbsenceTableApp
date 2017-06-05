#include "modelcontroller.h"
#include "../model/studentabsencemodel.h"

ModelController::ModelController(StudentAbsenceModel *controlModel)
{
    model = controlModel;
}

StudentAbsenceModel *ModelController::getModel() const
{
    return model;
}

void ModelController::setModel(StudentAbsenceModel *value)
{
    model = value;
}

void ModelController::clearModel()
{
    model->removeRows(0, model->entriesSize(), QModelIndex());
}
