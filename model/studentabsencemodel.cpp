#include <QtCore>
#include <QtAlgorithms>

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
    return row < studentEntryList.size() && column < LAST;
}

QModelIndex StudentAbsenceModel::index(int row, int column, const QModelIndex &parent) const
{
    if(hasIndex(row, column, parent))
        return createIndex(row, column, (void*)(&studentEntryList.at(row)));
    return QModelIndex();
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

bool StudentAbsenceModel::setData(const QModelIndex &index_, const QVariant &value, int role)
{
    if(index_.isValid() && role == Qt::EditRole)
    {
        switch(index_.column())
        {
        case NAME:
            studentEntryList[index_.row()].name.setFullName(value.toString());
            break;
        case GROUP:
            studentEntryList[index_.row()].group.setValue(value.toString());
            break;
        case ILLNESS:
                studentEntryList[index_.row()].absence.setIllness(value.toInt());
            break;
        case ANOTHER:
                studentEntryList[index_.row()].absence.setAnother(value.toInt());
            break;
        case HOOKY:
                studentEntryList[index_.row()].absence.setHooky(value.toInt());;
            break;
        }
        emit dataChanged(index_, index_);
        return true;
    }
    return false;
}

bool StudentAbsenceModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);

    for(int i = 0; i < count; ++i)
    {
        studentEntryList.insert(row, StudentEntry());
    }

    endInsertRows();
    return true;
}

bool StudentAbsenceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(int i = 0; i < count; ++i)
    {
        studentEntryList.removeAt(row);
    }
    endRemoveRows();
    emit dataChanged(index(row, 0, QModelIndex()), index(row + count - 1, LAST, QModelIndex()));
    return true;
}

Qt::ItemFlags StudentAbsenceModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
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
