#include "modelcontroller.h"
#include "../model/studentabsencemodel.h"

ModelController::ModelController(StudentAbsenceModel *controlModel)
{
    model = controlModel;
}

ModelController::~ModelController()
{}

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

void ModelController::addEntry(qint64 row, const StudentEntry& entry)
{
    model->insertRow(row);

    model->setData(
                model->index(row, StudentAbsenceModel::NAME, QModelIndex()),
                QVariant(entry.name.getFullName()), Qt::EditRole);
    model->setData(
                model->index(row, StudentAbsenceModel::GROUP, QModelIndex()),
                QVariant(entry.group.getValue()), Qt::EditRole);
    model->setData(
                model->index(row, StudentAbsenceModel::ILLNESS, QModelIndex()),
                QVariant(QString::number(entry.absence.getIllness())), Qt::EditRole);
    model->setData(
                model->index(row, StudentAbsenceModel::ANOTHER, QModelIndex()),
                QVariant(QString::number(entry.absence.getAnother())), Qt::EditRole);
    model->setData(
                model->index(row, StudentAbsenceModel::HOOKY, QModelIndex()),
                QVariant(QString::number(entry.absence.getHooky())), Qt::EditRole);
}
