#include <QGraphicsColorizeEffect>
#include <QLayout>
#include <QMessageBox>

#include "adddialog.h"
#include "../controller/modelcontroller.h"


AddDialog::AddDialog(StudentAbsenceModel *model_, StudentAbsenceClient *client_, QWidget *parent)
    :QDialog(parent), client(client_)
{
    model = model_;

    addBtn         = new QPushButton(tr("Добавить"), this);
    addBtn->setEnabled(false);
    closeBtn        = new QPushButton(tr("Закрыть"), this);

    surnameLbl      = new QLabel(tr("Фамилия"), this);
    nameLbl         = new QLabel(tr("Имя"), this);
    patronymicLbl   = new QLabel(tr("Отчество"), this);
    groupLbl        = new QLabel(tr("Группа"), this);
    absenceLbl      = new QLabel(tr("Число пропусков за год"), this);
    illnessLbl      = new QLabel(tr("Болезнь"), this);
    anotherLbl      = new QLabel(tr("Другие причины"), this);
    hookyLbl        = new QLabel(tr("Прогулы"), this);

    surnameEdt      = new QLineEdit(this);
    nameEdt         = new QLineEdit(this);

    patronymicEdt   = new QLineEdit(this);
    groupEdt        = new QLineEdit(this);
    illnessEdt      = new QLineEdit(this);
    anotherEdt      = new QLineEdit(this);
    hookyEdt        = new QLineEdit(this);

    surnameEdt->setFocus();

    listEdits << surnameEdt << nameEdt << patronymicEdt << groupEdt
              << illnessEdt << anotherEdt << hookyEdt;

    setWindowIcon(QIcon(":/images/addEntries.png"));
    setWindowTitle(tr("Добавить запись"));
    setMaximumSize(550, 200);
    setMinimumSize(550, 200);

    connectLineEdits();
    addLayouts();
}

AddDialog::~AddDialog()
{}

void AddDialog::createEntry()
{
    if(verifyEdits())
    {
        model->insertRow(model->entriesSize());

        model->setData(
                    model->index(model->entriesSize() - 1, StudentAbsenceModel::NAME, QModelIndex()),
                    QVariant(surnameEdt->text() + " " + nameEdt->text() + " " + patronymicEdt->text()), Qt::EditRole);
        model->setData(
                    model->index(model->entriesSize() - 1, StudentAbsenceModel::GROUP, QModelIndex()),
                    QVariant(groupEdt->text()), Qt::EditRole);
        model->setData(
                    model->index(model->entriesSize() - 1, StudentAbsenceModel::ILLNESS, QModelIndex()),
                    QVariant(illnessEdt->text().toInt()), Qt::EditRole);
        model->setData(
                    model->index(model->entriesSize() - 1, StudentAbsenceModel::ANOTHER, QModelIndex()),
                    QVariant(anotherEdt->text().toInt()), Qt::EditRole);
        model->setData(
                    model->index(model->entriesSize() - 1, StudentAbsenceModel::HOOKY, QModelIndex()),
                    QVariant(hookyEdt->text().toInt()), Qt::EditRole);
    }
    else
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Введены некорректные данные!"),
                                 QMessageBox::Ok);
    if(client){
        client->sendAddRequest(StudentEntry(RussianFullName(surnameEdt->text(), nameEdt->text(), patronymicEdt->text()),
                                            Group(groupEdt->text()),
                                            Absence(illnessEdt->text().toInt(), anotherEdt->text().toInt(), hookyEdt->text().toInt())));
    }
}

void AddDialog::changeNumFilledEdits()
{
    auto numFilledEdits = 0;

    foreach (const QLineEdit* edt, listEdits) {
        if(!edt->text().isEmpty() && edt->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
    }

    emit numFilledEditsChanged(numFilledEdits);
}

void AddDialog::enableAddButton(int num)
{
    addBtn->setEnabled(num == MAX_NUM_FILLED_EDITS);
}

void AddDialog::connectLineEdits()
{
    foreach (QLineEdit* edt, listEdits) {
        connect(
            edt, &QLineEdit::textChanged,
            [=] () { edt->setGraphicsEffect(Q_NULLPTR); }
        );
        connect(edt, SIGNAL( textChanged(const QString &) ), SLOT( changeNumFilledEdits() ) );
    }

    connect(this, SIGNAL( numFilledEditsChanged(int) ), SLOT( enableAddButton(int) ) );

    connect(addBtn,  SIGNAL( clicked(bool) ), SLOT( createEntry() ) );

    connect(closeBtn, SIGNAL( clicked(bool) ), SLOT( close() ) );
}


void AddDialog::addLayouts()
{
    QGridLayout *inputNameLayout = new QGridLayout;

    inputNameLayout->addWidget(surnameLbl, 0, 0);
    inputNameLayout->addWidget(surnameEdt, 0, 1);
    inputNameLayout->addWidget(nameLbl, 1, 0);
    inputNameLayout->addWidget(nameEdt, 1, 1);
    inputNameLayout->addWidget(patronymicLbl, 2, 0);
    inputNameLayout->addWidget(patronymicEdt, 2, 1);
    inputNameLayout->addWidget(groupLbl, 3, 0);
    inputNameLayout->addWidget(groupEdt, 3, 1);

    QGridLayout *inputAbsenceLayout = new QGridLayout;
    inputAbsenceLayout->addWidget(illnessLbl, 0, 0);
    inputAbsenceLayout->addWidget(illnessEdt, 0, 1);
    inputAbsenceLayout->addWidget(anotherLbl, 1, 0);
    inputAbsenceLayout->addWidget(anotherEdt, 1, 1);
    inputAbsenceLayout->addWidget(hookyLbl, 2, 0);
    inputAbsenceLayout->addWidget(hookyEdt, 2, 1);

    QVBoxLayout *absenceLayout = new QVBoxLayout;
    absenceLayout->addWidget(absenceLbl, 0, Qt::AlignCenter);
    absenceLayout->addLayout(inputAbsenceLayout);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(inputNameLayout);
    topLayout->addStretch();
    topLayout->addLayout(absenceLayout);

    QHBoxLayout *bottomRightLayout = new QHBoxLayout;
    bottomRightLayout->addStretch(1);
    bottomRightLayout->addWidget(addBtn);
    bottomRightLayout->addWidget(closeBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomRightLayout);

    setLayout(mainLayout);
}

void AddDialog::addEffect(QWidget *widget)
{
    auto effect = new QGraphicsColorizeEffect(widget);
    effect->setColor(QColor(250, 0, 0));
    widget->setGraphicsEffect(effect);
}

bool AddDialog::isName(const QString& text) const
{
    foreach (QChar ch, text) {
        if(ch.unicode() < 0x0400 || ch.unicode() > 0x04FF)
            return false;
    }
    return true;
}

bool AddDialog::verifyEdits()
{
    bool isCorrect = true;

    auto ok = true;

    if(!isName(surnameEdt->text()) || !surnameEdt->text().at(0).isUpper()){
        addEffect(surnameEdt);
        isCorrect = false;
    }

    if(!isName(nameEdt->text()) || !nameEdt->text().at(0).isUpper()){
        addEffect(nameEdt);
        isCorrect = false;
    }

    if(!isName(patronymicEdt->text()) || !patronymicEdt->text().at(0).isUpper()){
        addEffect(patronymicEdt);
        isCorrect = false;
    }

    long long num = groupEdt->text().toLongLong(&ok);
    if(!(ok) || num < 0 || groupEdt->text().length() != Group::getNumLetters()){
        addEffect(groupEdt);
        isCorrect = false;
    }

    num = illnessEdt->text().toLongLong(&ok);
    if(!(ok) || num < 0 || illnessEdt->text().length() > 4){
        addEffect(illnessEdt);
        isCorrect = false;
    }

    num = anotherEdt->text().toLongLong(&ok);
    if(!(ok) || num < 0 || anotherEdt->text().length() > 4){
        addEffect(anotherEdt);
        isCorrect = false;
    }

    num = hookyEdt->text().toLongLong(&ok);
    if(!(ok) || num < 0 || hookyEdt->text().length() > 4){
        addEffect(hookyEdt);
        isCorrect = false;
    }

    changeNumFilledEdits();
    return isCorrect;
}
