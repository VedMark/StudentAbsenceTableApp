#include "studentabsencemodel.h"

StudentAbsenceModel::StudentAbsenceModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    horizontalHeader = new QStandardItemModel(this);
    createHorizontalHeader();
}

QVariant StudentAbsenceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role);
    if(orientation == Qt::Vertical)
        return section;

    return QVariant();
}

bool StudentAbsenceModel::hasIndex(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(row < 0 || column < 0)
        return false;
    return row < studentEntryList.length() && column < LAST;
}

QModelIndex StudentAbsenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent))
        return createIndex(row, column, (void*)&studentEntryList.at(row));
    return QModelIndex();
}

int StudentAbsenceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return studentEntryList.length();
}

int StudentAbsenceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return LAST;
}

QVariant StudentAbsenceModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        if(index.isValid())
        {
            switch(index.column())
            {
            case NAME:
                return studentEntryList[index.row()].name.getFullName();
            case GROUP:
                return studentEntryList[index.row()].group.getValue();
            case ILLNESS:
                return QString::number(studentEntryList[index.row()].absence.getIllness());
            case ANOTHER:
                return QString::number(studentEntryList[index.row()].absence.getAnother());
            case HOOKY:
                return QString::number(studentEntryList[index.row()].absence.getHooky());
            case TOTAL:
                return QString::number(studentEntryList[index.row()].absence.getTotal());
            }
        }
        break;

    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter);

    case Qt::UserRole:
        QVariant horizontalHeaderData;
        horizontalHeaderData.setValue((QObject*)horizontalHeader);
        return horizontalHeaderData;
    }

    return QVariant();
}

bool StudentAbsenceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole)
    {
        try
        {
            switch(index.column())
            {
            case NAME:
                studentEntryList[index.row()].name.setFullName(value.toString());
                break;
            case GROUP:
                studentEntryList[index.row()].group.setValue(value.toString());
                break;
            case ILLNESS:
                studentEntryList[index.row()].absence.setIllness(value.toInt());
                break;
            case ANOTHER:
                studentEntryList[index.row()].absence.setAnother(value.toInt());
                break;
            case HOOKY:
                studentEntryList[index.row()].absence.setHooky(value.toInt());;
                break;
            }
        }
        catch(...)
        {
            return false;
        }
        return true;
    }
    return false;
}

bool StudentAbsenceModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count);

    for(int i = 0; i < count; ++i)
    {
        StudentEntry newStudentEntry = StudentEntry();
        studentEntryList.insert(row, newStudentEntry);
    }

    endInsertRows();
    return true;
}

bool StudentAbsenceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, count);

    for(int i = 0; i < count; ++i)
    {
        studentEntryList.removeAt(row);
    }

    endRemoveRows();
    return true;
}

Qt::ItemFlags StudentAbsenceModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void StudentAbsenceModel::createHorizontalHeader()
{
    horizontalHeader->setItem(0, 0, new QStandardItem(tr("Ф.И.О. студента")));
    horizontalHeader->setItem(0, 1, new QStandardItem(tr("группа")));

    QStandardItem *absence_root_item = new QStandardItem(tr("число пропусков занятий за год"));
    QList<QStandardItem*> absence_list = QList<QStandardItem*>();

    absence_list.append(new QStandardItem("по болезни"));
    absence_root_item->appendColumn(absence_list);
    absence_list.clear();

    absence_list.append(new QStandardItem("по другим причинам"));
    absence_root_item->appendColumn(absence_list);
    absence_list.clear();

    absence_list.append(new QStandardItem("без уважительной причины"));
    absence_root_item->appendColumn(absence_list);
    absence_list.clear();

    absence_list.append(new QStandardItem("итого"));
    absence_root_item->appendColumn(absence_list);
    absence_list.clear();

    horizontalHeader->setItem(0, 2, absence_root_item);
}

StudentAbsenceModel::RussianFullName::RussianFullName()
    : surname(""),
      name(""),
      patronimic(""){}

StudentAbsenceModel::RussianFullName::RussianFullName(QString s_, QString n_, QString p_)
    : surname(s_),
      name(n_),
      patronimic(p_){}

QString StudentAbsenceModel::RussianFullName::getFullName() const
{
    return surname + " " + name + " " + patronimic;
}

bool StudentAbsenceModel::RussianFullName::setFullName(QString fullName)
{
    QStringList list = fullName.split(" ");

    if(list.length() != 3)
        return false;

    surname = list[0];
    name = list[1];
    patronimic = list[2];
    return true;
}

QString StudentAbsenceModel::RussianFullName::getSurname() const
{
    return surname;
}

void StudentAbsenceModel::RussianFullName::setSurname(const QString &value)
{
    surname = value;
}

QString StudentAbsenceModel::RussianFullName::getName() const
{
    return name;
}

void StudentAbsenceModel::RussianFullName::setName(const QString &value)
{
    name = value;
}

QString StudentAbsenceModel::RussianFullName::getPatronimic() const
{
    return patronimic;
}

void StudentAbsenceModel::RussianFullName::setPatronimic(const QString &value)
{
    patronimic = value;
}



qint8 StudentAbsenceModel::Group::numLetters = 6;

StudentAbsenceModel::Group::Group(QString val)
{
    if(numLetters != val.length())
        throw GroupNumLettersException();
    value = val;
}

QString StudentAbsenceModel::Group::getValue() const
{
    return value;
}

void StudentAbsenceModel::Group::setValue(const QString &val)
{
    if(numLetters != val.length() || !isNumber(val))
        throw GroupNumLettersException();
    value = val;
}

qint8 StudentAbsenceModel::Group::getNumLetters()
{
    return numLetters;
}

void StudentAbsenceModel::Group::setNumLetters(const qint8 &value)
{
    numLetters = value;
}

bool StudentAbsenceModel::Group::isNumber(const QString & source) const
{
    QRegExp numberExp("[0-9]{" + QString::number(numLetters) + "}");
    return numberExp.indexIn(source) != 1;
}



StudentAbsenceModel::Absence::Absence()
    : illness(0),
      another(0),
      hooky(0) {}

StudentAbsenceModel::Absence::Absence(qint16 illness_, qint16 another_, qint16 hooky_)
    : illness(illness_),
      another(another_),
      hooky(hooky_){}

qint16 StudentAbsenceModel::Absence::getIllness() const
{
    return illness;
}

void StudentAbsenceModel::Absence::setIllness(qint16 value)
{
    if(value < 0)
        throw AbsenceNumOutOfRangeExceprion();
    illness = value;
}

qint16 StudentAbsenceModel::Absence::getAnother() const
{
    return another;
}

void StudentAbsenceModel::Absence::setAnother(qint16 value)
{
    if(value < 0)
        throw AbsenceNumOutOfRangeExceprion();
    another = value;
}

qint16 StudentAbsenceModel::Absence::getHooky() const
{
    return hooky;
}

void StudentAbsenceModel::Absence::setHooky(qint16 value)
{
    if(value < 0)
        throw AbsenceNumOutOfRangeExceprion();
    hooky = value;
}

qint16 StudentAbsenceModel::Absence::getTotal() const
{
    return illness + another + hooky;
}



StudentAbsenceModel::StudentEntry::StudentEntry()
    : name(), group(), absence()
{}

StudentAbsenceModel::StudentEntry::StudentEntry(const StudentAbsenceModel::RussianFullName &n_,
        const QString &gr_,
        const StudentAbsenceModel::Absence &abs_)
    : name(n_), group(gr_), absence(abs_)
{}
