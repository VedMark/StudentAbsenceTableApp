#pragma once

#include "../model/studentabsencemodel.h"

#include <QException>
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class FileOpenException: public QException
{
public:
    void raise() const { throw *this; }
    FileOpenException *clone() const { return new FileOpenException(*this); }
};

class FileReadException: public QException
{
public:
    void raise() const { throw *this; }
    FileReadException *clone() const { return new FileReadException(*this); }
};

class XMLParser
{
public:
    XMLParser(StudentAbsenceModel *model);
    ~XMLParser();

    bool read(const QString& file);
    bool write(const QString& file);

private:
    static const QString XML_TAG_STUDENT_DATA;
    static const QString XML_TAG_ENTRY;
    static const QString XML_TAG_SURNAME;
    static const QString XML_TAG_NAME;
    static const QString XML_TAG_PATROMYMIC;
    static const QString XML_TAG_GROUP;
    static const QString XML_TAG_ILLNESS;
    static const QString XML_TAG_ANOTHER;
    static const QString XML_TAG_HOOKY;

    void writeEntry(QXmlStreamWriter &writer, const StudentEntry &entry);

    StudentAbsenceModel *model;
};
