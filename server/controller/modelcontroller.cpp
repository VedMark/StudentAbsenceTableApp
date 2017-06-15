#include "modelcontroller.h"
#include "../controller/xmlparser.h"

#include <algorithm>

ModelController::ModelController(StudentAbsenceModel *controlModel)
    :searchList(Q_NULLPTR)
{
    model = controlModel;
    xmlParser = new XMLParser(model);
}

ModelController::~ModelController()
{
    delete xmlParser;
}

StudentAbsenceModel *ModelController::getModel() const
{
    return model;
}

void ModelController::setModel(StudentAbsenceModel *value)
{
    model = value;
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

StudentAbsenceModel::students ModelController::findEntries(SearchPattern pattern, const QStringList &list)
{
    std::function<bool (const StudentEntry &)> cond = condition(pattern);
    searchList = &list;

    StudentAbsenceModel::students searchResult;
    std::copy_if(model->getStudentEntryList().begin(),
                 model->getStudentEntryList().end(),
                 std::back_inserter(searchResult),
                 cond);
    return searchResult;
}

void ModelController::removeEntries(SearchPattern pattern, const QStringList &list)
{
    StudentAbsenceModel::students &studentList = model->getStudentEntryList();
    auto length_before = model->getStudentEntryList().length();
    std::function<bool (const StudentEntry &)> cond = condition(pattern);
    searchList = &list;

    studentList.erase(
                std::remove_if(studentList.begin(),studentList.end(), cond),
                studentList.end());

    auto length_after = model->getStudentEntryList().length();
    model->removeRows(length_after, length_before - length_after, QModelIndex());
}

void ModelController::clearModel()
{
    model->removeRows(0, model->entriesSize(), QModelIndex());
}

bool ModelController::saveModel(QString fileName)
{
    try{
        xmlParser->write(fileName);
    }
    catch(...){
        return false;
    }
    return true;
}

bool ModelController::loadModel(QString fileName)
{
    try{
        xmlParser->read(fileName);
    }
    catch(...){
        return false;
    }
    return true;
}

std::function<bool (const StudentEntry &)> ModelController::condition(SearchPattern pattern)
{
    std::function<bool (const StudentEntry &)> condition;

    switch (pattern) {
    case FIRST:{
        condition = [this] (const StudentEntry &entry) -> bool {
                    return entry.name.getSurname() == searchList->at(0) &&
                           entry.group.getValue() == searchList->at(1);
                    };
        break;
    }
    case SECOND:{
        std::function<bool (const StudentEntry&)> equalAbsence =
                [&] (const StudentEntry& entry) -> bool {
                    switch (searchList->at(1).toInt()) {
                    case StudentAbsenceModel::ILLNESS:
                        return entry.absence.getIllness() == searchList->at(2).toInt();
                    case StudentAbsenceModel::ANOTHER:
                        return entry.absence.getAnother() ==searchList->at(2).toInt();
                    case StudentAbsenceModel::HOOKY:
                        return entry.absence.getHooky() == searchList->at(2).toInt();
                    }
                    return false;
                };

        condition =  [this, equalAbsence] (const StudentEntry &entry) -> bool {
                     return entry.name.getSurname() == searchList->at(0) &&
                            equalAbsence(entry);
                     };
        break;
    }
    case THIRD:{
        condition =  [this] (const StudentEntry &entry) -> bool {
                     return entry.name.getSurname() == searchList->at(0) &&
                            entry.absence.getTotal() >= searchList->at(1).toInt() &&
                            entry.absence.getTotal() <= searchList->at(2).toInt();
                     };
        break;
    }
    }
    return condition;
}
