#include "modelcontroller.h"
#include "../controller/xmlparser.h"

#include <algorithm>

ModelController::ModelController(Students *controlModel)
    :searchList(Q_NULLPTR)
{
    model = controlModel;
    xmlParser = new XMLParser(model);
}

ModelController::~ModelController()
{
    delete xmlParser;
}

Students *ModelController::getModel() const
{
    return model;
}

void ModelController::setModel(Students *value)
{
    model = value;
}

void ModelController::addEntry(const StudentEntry& entry)
{
    model->push_back(entry);
}

Students ModelController::findEntries(SearchPattern pattern, const QStringList &list)
{
    std::function<bool (const StudentEntry &)> cond = condition(pattern);
    searchList = &list;

    Students searchResult;
    std::copy_if(model->begin(),
                 model->end(),
                 std::back_inserter(searchResult),
                 cond);
    return searchResult;
}

qint64 ModelController::removeEntries(SearchPattern pattern, const QStringList &list)
{
    std::function<bool (const StudentEntry &)> cond = condition(pattern);
    searchList = &list;
    qint64 size_before = model->size();
    model->erase(
                std::remove_if(model->begin(),model->end(), cond),
                model->end());
    return qint64(size_before - model->size());
}

void ModelController::clearModel()
{
    *model = Students();
}

bool ModelController::saveModel(QString fileName)
{
    try{
        if(!fileName.endsWith(QStringLiteral(".xml")))
            fileName += QStringLiteral(".xml");
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
                    switch ((Column)searchList->at(1).toInt() + 2) {
                    case ILLNESS:
                        return entry.absence.getIllness() == searchList->at(2).toInt();
                    case ANOTHER:
                        return entry.absence.getAnother() ==searchList->at(2).toInt();
                    case HOOKY:
                        return entry.absence.getHooky() == searchList->at(2).toInt();
                    default:
                        break;
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
