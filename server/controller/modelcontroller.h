#pragma once

#include <QtGlobal>
#include <QString>
#include <functional>

#include "model/studentabsencemodel.h"

class XMLParser;

enum SearchPattern{
    FIRST,
    SECOND,
    THIRD
};

class ModelController
{
public:
    ModelController(StudentAbsenceModel *controlModel = Q_NULLPTR);
    ~ModelController();

    StudentAbsenceModel *getModel() const;
    void setModel(StudentAbsenceModel *value);

    void addEntry(qint64 row, const StudentEntry &entry);
    StudentAbsenceModel::students findEntries(SearchPattern, const QStringList&);
    void removeEntries(SearchPattern, const QStringList&);

    void clearModel();
    bool saveModel(QString fileName);
    bool loadModel(QString fileName);

private:
    std::function<bool (const StudentEntry &)> condition(SearchPattern pattern);
    StudentAbsenceModel *model;
    XMLParser *xmlParser;
    const QStringList *searchList;
};
