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

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool hasIndex(int row, int column, const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    qint64 entriesSize() const;

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
    RussianFullName &operator=(const RussianFullName&);

    QString getFullName() const;
    bool setFullName(QString fullName);

    QString getSurname() const;
    void setSurname(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    QString getPatronimic() const;
    void setPatronimic(const QString &value);
};



class Group{
    QString value;
    static qint8 numLetters;

public:
    Group(QString val = QString("000000"));
    Group &operator=(const Group&);

    QString getValue() const;
    void setValue(const QString &value);

    static qint8 getNumLetters();
    static void setNumLetters(const qint8 &value);

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
    Absence &operator=(const Absence&);

    qint16 getIllness() const;
    void setIllness(qint16 value);

    qint16 getAnother() const;
    void setAnother(qint16 value);

    qint16 getHooky() const;
    void setHooky(qint16 value);

    qint16 getTotal() const;
};



struct StudentEntry{
    RussianFullName name;
    Group group;
    Absence absence;

    StudentEntry();
    StudentEntry(const RussianFullName&, const Group &group, const Absence&);
};

#endif // STUDENTABSENCEMODEL_H
