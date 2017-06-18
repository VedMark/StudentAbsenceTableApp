#include "studentabsencemodel.h"


RussianFullName::RussianFullName()
    : surname(""),
      name(""),
      patronymic(""){}

RussianFullName::RussianFullName(QString s_, QString n_, QString p_)
    : surname(s_),
      name(n_),
      patronymic(p_){}

RussianFullName::RussianFullName(const RussianFullName & name)
    : surname(name.surname),
      name(name.name),
      patronymic(name.patronymic)
{}

RussianFullName &RussianFullName::operator=(const RussianFullName & anotherName)
{
    surname = anotherName.surname;
    name = anotherName.name;
    patronymic = anotherName.patronymic;
    return *this;
}

RussianFullName::~RussianFullName()
{}


qint8 Group::numLetters = 6;

Group::Group(QString val)
    : value(val)
{}

Group::Group(const Group & group)
    : value(group.value)
{}

Group &Group::operator=(const Group &anotherGroup)
{
    value = anotherGroup.value;
    return *this;
}

Group::~Group()
{}

bool Group::isNumber(const QString & source) const
{
    QRegExp numberExp("[0-9]{" + QString::number(numLetters) + "}");
    return numberExp.indexIn(source) != 1;
}



Absence::Absence()
    : illness(0),
      another(0),
      hooky(0) {}

Absence::Absence(qint16 illness_, qint16 another_, qint16 hooky_)
    : illness(illness_),
      another(another_),
      hooky(hooky_){}

Absence::Absence(const Absence &abs)
    : illness(abs.illness),
      another(abs.another),
      hooky(abs.hooky)
{}

Absence &Absence::operator=(const Absence &anotherAbsence)
{
    illness = anotherAbsence.illness;
    another = anotherAbsence.another;
    hooky = anotherAbsence.hooky;
    return *this;
}

Absence::~Absence()
{}



StudentEntry::StudentEntry()
    : name(), group(), absence()
{}

StudentEntry::StudentEntry(const RussianFullName &n_,
        const Group &gr_,
        const Absence &abs_)
    : name(n_), group(gr_), absence(abs_)
{}

StudentEntry::StudentEntry(const StudentEntry &entry)
    : name(entry.name), group(entry.group), absence(entry.absence)
{}
