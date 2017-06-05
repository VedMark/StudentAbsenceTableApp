#ifndef STUDENTABSENCEMODEL_H
#define STUDENTABSENCEMODEL_H

#include <QAbstractTableModel>
#include <QStandardItemModel>

class RussianFullName;
class Group;
class Absence;
struct StudentEntry;

class StudentAbsenceModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column{
        NAME,
        GROUP,
        ILLNESS,
        ANOTHER,
        HOOKY,
        TOTAL,
        LAST
    };

    explicit StudentAbsenceModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool hasIndex(int row, int column, const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    inline int rowCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent)
        return studentEntryList.length();
    }
    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override{
        Q_UNUSED(parent);
        return LAST;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    inline qint64 entriesSize() const
    { return studentEntryList.size(); }

    inline QList<StudentEntry> &getStudentEntryList()
    { return studentEntryList; }
    inline void setStudentEntryList(const QList<StudentEntry> &value){
        beginInsertRows(QModelIndex(),0, value.size() - 1);
        studentEntryList = value;
        endInsertRows();
        emit dataChanged(
                    index(0, 0, QModelIndex()),
                    index(studentEntryList.size(), LAST, QModelIndex())
                    );
    }

private:
    void createHorizontalHeader();

    QList<StudentEntry> studentEntryList;
    QStandardItemModel *horizontalHeader;
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

#endif // STUDENTABSENCEMODEL_H
