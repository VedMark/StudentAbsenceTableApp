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
    ModelController(Students *controlModel = Q_NULLPTR);
    ~ModelController();

    Students *getModel() const;
    void setModel(Students *value);

    void addEntry(const StudentEntry &entry);
    Students findEntries(SearchPattern, const QStringList&);
    qint64 removeEntries(SearchPattern, const QStringList&);

    void clearModel();
    bool saveModel(QString fileName);
    bool loadModel(QString fileName);

private:
    std::function<bool (const StudentEntry &)> condition(SearchPattern pattern);
    Students *model;
    XMLParser *xmlParser;
    const QStringList *searchList;
};
