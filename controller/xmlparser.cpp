#include "xmlparser.h"

#include <QFile>
#include <QIODevice>

const QString XMLParser::XML_TAG_STUDENT_DATA = "studentData";
const QString XMLParser::XML_TAG_ENTRY = "entry";
const QString XMLParser::XML_TAG_SURNAME = "surname";
const QString XMLParser::XML_TAG_NAME = "name";
const QString XMLParser::XML_TAG_PATROMYMIC = "patronymic";
const QString XMLParser::XML_TAG_GROUP = "group";
const QString XMLParser::XML_TAG_ILLNESS = "illness";
const QString XMLParser::XML_TAG_ANOTHER = "another";
const QString XMLParser::XML_TAG_HOOKY = "hooky";

XMLParser::XMLParser(StudentAbsenceModel *model_)
    : model(model_)
{}

XMLParser::~XMLParser()
{}

bool XMLParser::read(const QString& file)
{
    QFile inFile(file);

    if(!(inFile.open(QIODevice::ReadOnly | QIODevice::Text) && file.endsWith(".xml")))
        throw FileOpenException();

    QXmlStreamReader *reader = new QXmlStreamReader(&inFile);

    StudentAbsenceModel::students newDataList;
    StudentEntry *entry;

    while(!reader->atEnd())
    {
        QXmlStreamReader::TokenType tokenEntry = reader->readNext();
        if(reader->hasError())
            throw FileReadException();

        if(tokenEntry == QXmlStreamReader::StartElement)
        {
            if(reader->name() == XML_TAG_ENTRY)
                entry = new StudentEntry();
            else if(reader->name() == XML_TAG_SURNAME)
                entry->name.setSurname(reader->readElementText());
            else if(reader->name() == XML_TAG_NAME)
                entry->name.setName(reader->readElementText());
            else if(reader->name() == XML_TAG_PATROMYMIC)
                entry->name.setPatronymic(reader->readElementText());
            else if(reader->name() == XML_TAG_GROUP)
                entry->group.setValue(reader->readElementText());
            else if(reader->name() == XML_TAG_ILLNESS)
                entry->absence.setIllness(reader->readElementText().toInt());
            else if(reader->name() == XML_TAG_ANOTHER)
                entry->absence.setAnother(reader->readElementText().toInt());
            else if(reader->name() == XML_TAG_HOOKY)
                entry->absence.setHooky(reader->readElementText().toInt());
        }
        else if(tokenEntry == QXmlStreamReader::EndElement && reader->name() == XML_TAG_ENTRY)
            newDataList << *entry;
    }

    model->removeRows(0, model->entriesSize(), QModelIndex());
    model->setStudentEntryList(newDataList);

    inFile.close();
    delete reader;
    return true;
}

bool XMLParser::write(const QString& file)
{
    QXmlStreamWriter *writer = new QXmlStreamWriter();
    QFile inFile(file);

    writer->setDevice(&inFile);
    if(!(inFile.open(QIODevice::WriteOnly) && file.endsWith(".xml")))
        throw FileOpenException();

    writer->writeStartDocument();
    writer->writeStartElement(XML_TAG_STUDENT_DATA);

    foreach (const StudentEntry& entry, model->getStudentEntryList()) {
        writer->writeStartElement(XML_TAG_ENTRY);
        writeEntry(writer, entry);
        writer->writeEndElement();
    }

    writer->writeEndElement();
    writer->writeEndDocument();
    QFile outFile(file);

    outFile.close();
    return true;
}

void XMLParser::writeEntry(QXmlStreamWriter * writer, const StudentEntry &entry)
{
    writer->writeStartElement(XML_TAG_SURNAME);
    writer->writeCharacters(entry.name.getSurname());
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_NAME);
    writer->writeCharacters(entry.name.getName());
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_PATROMYMIC);
    writer->writeCharacters(entry.name.getPatronymic());
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_GROUP);
    writer->writeCharacters(entry.group.getValue());
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_ILLNESS);
    writer->writeCharacters(QString::number(entry.absence.getIllness()));
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_ANOTHER);
    writer->writeCharacters(QString::number(entry.absence.getAnother()));
    writer->writeEndElement();

    writer->writeStartElement(XML_TAG_HOOKY);
    writer->writeCharacters(QString::number(entry.absence.getHooky()));
    writer->writeEndElement();
}
