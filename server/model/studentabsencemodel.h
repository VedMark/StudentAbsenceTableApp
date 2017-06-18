#pragma once

#include <QList>
#include <QString>
#include <QStringList>


class RussianFullName;
class Group;
class Absence;
struct StudentEntry;

using Students = QList<StudentEntry>;

enum Column{
    NAME,
    GROUP,
    ILLNESS,
    ANOTHER,
    HOOKY,
    TOTAL,
    LAST
};

class RussianFullName{
private:
    QString surname;
    QString name;
    QString patronymic;

public:
    explicit RussianFullName();
    explicit RussianFullName(QString surname_, QString name_, QString patronimic_);
    RussianFullName(const RussianFullName&);
    RussianFullName &operator=(const RussianFullName&);
    ~RussianFullName();

    inline QString getFullName() const
    { return surname + " " + name + " " + patronymic; }
    inline bool setFullName(QString fullName) {
        QStringList list = fullName.split(" ");
        if(list.length() != 3) return false;
        surname = list[0]; name = list[1]; patronymic = list[2];
        return true;
    }

    inline QString getSurname() const
    { return surname; }
    inline void setSurname(const QString &value)
    { surname = value; }

    QString getName() const
    { return name; }
    void setName(const QString &value)
    { name = value; }

    QString getPatronymic() const
    { return patronymic; }
    void setPatronymic(const QString &value)
    { patronymic = value; }
};



class Group{
    QString value;
    static qint8 numLetters;

public:
    explicit Group(QString val = QString("000000"));
    Group(const Group&);
    Group &operator=(const Group&);
    ~Group();

    inline QString getValue() const
    { return value; }
    inline void setValue(const QString &value)
    { this->value = value; }

    inline static qint8 getNumLetters()
    { return numLetters; }
    inline static void setNumLetters(const qint8 &value)
    { numLetters = value; }

private:
    bool isNumber(const QString & source) const;
};



class Absence{
private:
    qint16 illness;
    qint16 another;
    qint16 hooky;

public:
    explicit Absence();
    explicit Absence(qint16 illness_, qint16 another_, qint16 hooky_);
    Absence(const Absence&);
    Absence &operator=(const Absence&);
    ~Absence();

    inline qint16 getIllness() const
    { return illness; }
    inline void setIllness(qint16 value)
    { illness = value; }

    inline qint16 getAnother() const
    { return another; }
    inline void setAnother(qint16 value)
    { another = value; }

    inline qint16 getHooky() const
    { return hooky; }
    inline void setHooky(qint16 value)
    { hooky = value; }

    inline qint16 getTotal() const
    { return illness + another + hooky; }
};



struct StudentEntry{
    RussianFullName name;
    Group group;
    Absence absence;

    explicit StudentEntry();
    explicit StudentEntry(const RussianFullName&, const Group &group, const Absence&);
    StudentEntry(const StudentEntry&);
};
