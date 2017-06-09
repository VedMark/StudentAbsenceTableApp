#include "abstractfinddialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLayout>
#include <QMessageBox>
#include <QGraphicsColorizeEffect>

#include <algorithm>
#include <functional>

AbstractFindDialog::AbstractFindDialog(StudentAbsenceModel *model_, QWidget *parent)
    : QDialog(parent)
{
    model = model_;

    switcher = new QGroupBox("Условия поиска:", this);
    firstPattern = new QRadioButton("Фамилия, группа", switcher);
    secondPattern = new QRadioButton("Фамилия, вид пропуска", switcher);
    thirdPattern = new QRadioButton("Фамилия, всего пропусков", switcher);
    firstPattern->setChecked(true);

    resultTable = new QTableView(this);
    resultTable->setVisible(false);

    HierarchicalHeaderView *header = new HierarchicalHeaderView(Qt::Horizontal, this);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setStretchLastSection(true);
    resultTable->setHorizontalHeader(header);
    resultTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    resultTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    stackedWidget = new QStackedWidget(this);
    firstGroup = new QGroupBox(stackedWidget);
    secondGroup = new QGroupBox(stackedWidget);
    thirdGroup = new QGroupBox(stackedWidget);

    surnameLbl1 = new QLabel("Фамилия", this);
    surnameEdt1 = new QLineEdit(this);
    groupLbl = new QLabel("Группа", this);
    groupEdt = new QLineEdit(this);

    surnameLbl2 = new QLabel("Фамилия", this);
    absKindlbl = new QLabel("Вид пропуска", this);
    numAbsLbl = new QLabel("Количество", this);
    surnameEdt2 = new QLineEdit(this);
    absKindCmb = new QComboBox(this);
    absKindCmb->addItem("Болезнь");
    absKindCmb->addItem("Другое");
    absKindCmb->addItem("Прогул");
    numAbsEdt = new QLineEdit(this);

    surnameLbl3 = new QLabel("Фамилия", this);
    lowBoundLbl = new QLabel("От", this);
    topBoundLbl = new QLabel("До", this);
    surnameEdt3 = new QLineEdit(this);
    lowBoundEdt = new QLineEdit(this);
    topBoundEdt = new QLineEdit(this);

    listEdits << surnameEdt1 << groupEdt
              << surnameEdt2 << numAbsEdt
              << surnameEdt3 << lowBoundEdt << topBoundEdt;

    configStackedWidget();

    okBtn         = new QPushButton("OK", this);
    okBtn->setEnabled(false);
    closeBtn        = new QPushButton("Закрыть", this);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(500, 200);
    setMaximumSize(1000, 600);

    connectLineEdits();
    addLayouts();
}

AbstractFindDialog::~AbstractFindDialog()
{}

void AbstractFindDialog::displayStudentEntryList(const StudentAbsenceModel::students &list)
{
    StudentAbsenceModel *mod = new StudentAbsenceModel(this);
    mod->setStudentEntryList(list);

    resultTable->setModel(mod);
    resultTable->setVisible(true);

    resize(sizeHint());

    move((QApplication::desktop()->width() - width()) / 2,
         (QApplication::desktop()->height() - height()) / 2);
}

std::function<bool (const StudentEntry &)> AbstractFindDialog::condition()
{
        std::function<bool (const StudentEntry &)> condition;

        switch (stackedWidget->currentIndex()) {
        case FIRST:{
            condition = [this] (const StudentEntry &entry) -> bool {
                        return entry.name.getSurname() == surnameEdt1->text() &&
                               entry.group.getValue() == groupEdt->text();
                        };
            break;
        }
        case SECOND:{
            std::function<bool (const StudentEntry&)> equalAbsence =
                    [&] (const StudentEntry& entry) -> bool {
                        switch (absKindCmb->currentIndex() + 2) {
                        case StudentAbsenceModel::ILLNESS:
                            return entry.absence.getIllness() == numAbsEdt->text().toInt();
                        case StudentAbsenceModel::ANOTHER:
                            return entry.absence.getAnother() == numAbsEdt->text().toInt();
                        case StudentAbsenceModel::HOOKY:
                            return entry.absence.getHooky() == numAbsEdt->text().toInt();
                        }
                        return false;
                    };

            condition =  [this, equalAbsence] (const StudentEntry &entry) -> bool {
                         return entry.name.getSurname() == surnameEdt2->text() &&
                                equalAbsence(entry);
                         };
            break;
        }
        case THIRD:{
            condition =  [this] (const StudentEntry &entry) -> bool {
                         return entry.name.getSurname() == surnameEdt3->text() &&
                                entry.absence.getTotal() >= lowBoundEdt->text().toInt() &&
                                entry.absence.getTotal() <= topBoundEdt->text().toInt();
                         };
            break;
        }
        }
        return condition;
}

void AbstractFindDialog::changeNumFilledEdits()
{
    int numFilledEdits = 0;

    switch (stackedWidget->currentIndex()) {
    case FIRST:
        if(!surnameEdt1->text().isEmpty() && surnameEdt1->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        if(!groupEdt->text().isEmpty() && groupEdt->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        break;
    case SECOND:
        if(!surnameEdt2->text().isEmpty() && surnameEdt2->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        if(!numAbsEdt->text().isEmpty() && numAbsEdt->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        break;
    case THIRD:
        if(!surnameEdt3->text().isEmpty() && surnameEdt3->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        if(!lowBoundEdt->text().isEmpty() && lowBoundEdt->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        if(!topBoundEdt->text().isEmpty() && topBoundEdt->graphicsEffect() == Q_NULLPTR)
            numFilledEdits++;
        break;
    }

    emit numFilledEditsChanged(numFilledEdits);
}

void AbstractFindDialog::enableOkButton(int num)
{
    switch (stackedWidget->currentIndex()) {
    case FIRST:
        okBtn->setEnabled(num == 2);
        break;
    case SECOND:
        okBtn->setEnabled(num == 2);
        break;
    case THIRD:
        okBtn->setEnabled(num == 3);
        break;
    }
}

void AbstractFindDialog::configStackedWidget()
{
    QVBoxLayout *firstLayout = new QVBoxLayout;
    QGridLayout *firstGroupLayout = new QGridLayout;
    firstGroupLayout->addWidget(surnameLbl1, 0, 0);
    firstGroupLayout->addWidget(surnameEdt1, 0, 1);
    firstGroupLayout->addWidget(groupLbl, 1, 0);
    firstGroupLayout->addWidget(groupEdt, 1, 1);
    firstLayout->addLayout(firstGroupLayout);
    firstLayout->addStretch();
    firstGroup->setLayout(firstLayout);

    QGridLayout *secondGroupLayout = new QGridLayout;
    secondGroupLayout->addWidget(surnameLbl2, 0, 0);
    secondGroupLayout->addWidget(surnameEdt2, 0, 1);
    secondGroupLayout->addWidget(absKindlbl, 1, 0);
    secondGroupLayout->addWidget(absKindCmb, 1, 1);
    secondGroupLayout->addWidget(numAbsLbl, 2, 0);
    secondGroupLayout->addWidget(numAbsEdt, 2, 1);
    secondGroup->setLayout(secondGroupLayout);

    QGridLayout *thirdGroupLayout = new QGridLayout;
    thirdGroupLayout->addWidget(surnameLbl3, 0, 0);
    thirdGroupLayout->addWidget(surnameEdt3, 0, 1);
    thirdGroupLayout->addWidget(lowBoundLbl, 1, 0);
    thirdGroupLayout->addWidget(lowBoundEdt, 1, 1);
    thirdGroupLayout->addWidget(topBoundLbl, 2, 0);
    thirdGroupLayout->addWidget(topBoundEdt, 2, 1);
    thirdGroup->setLayout(thirdGroupLayout);

    stackedWidget->addWidget(firstGroup);
    stackedWidget->addWidget(secondGroup);
    stackedWidget->addWidget(thirdGroup);
}

void AbstractFindDialog::connectLineEdits()
{
    foreach (QLineEdit* edt, listEdits) {
        connect(edt, SIGNAL( textChanged(const QString &) ), SLOT( changeNumFilledEdits() ) );
        connect(
            edt, &QLineEdit::textChanged,
            [=] () { edt->setGraphicsEffect(Q_NULLPTR); }
        );
    }

    connect(
        firstPattern, &QRadioButton::clicked,
        [this] () { stackedWidget->setCurrentIndex(0); }
    );

    connect(
        secondPattern, &QRadioButton::clicked,
        [this] () { stackedWidget->setCurrentIndex(1); }
    );

    connect(
        thirdPattern, &QRadioButton::clicked,
        [this] () { stackedWidget->setCurrentIndex(2); }
    );

    connect(this, SIGNAL( numFilledEditsChanged(int) ), SLOT( enableOkButton(int) ) );

    connect(okBtn,  SIGNAL( clicked(bool) ), SLOT( handleOkBtn() ) );

    connect(closeBtn, SIGNAL( clicked(bool) ), SLOT( close() ) );
}

void AbstractFindDialog::addLayouts()
{
    QVBoxLayout *switcherLayout = new QVBoxLayout;
    switcherLayout->addWidget(firstPattern);
    switcherLayout->addWidget(secondPattern);
    switcherLayout->addWidget(thirdPattern);
    switcherLayout->addStretch(1);
    switcher->setLayout(switcherLayout);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(switcher);
    topLayout->addWidget(stackedWidget);
    topLayout->addStretch();

    QHBoxLayout *bottomRightLayout = new QHBoxLayout;
    bottomRightLayout->addStretch(1);
    bottomRightLayout->addWidget(okBtn);
    bottomRightLayout->addWidget(closeBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(resultTable);
    mainLayout->addStretch(1);
    mainLayout->addLayout(bottomRightLayout);

    setLayout(mainLayout);
}

void AbstractFindDialog::addEffect(QWidget *widget)
{
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect(widget);
    effect->setColor(QColor(250, 0, 0));
    widget->setGraphicsEffect(effect);
}

bool AbstractFindDialog::isName(const QString& text) const
{
    foreach (QChar ch, text) {
        if(ch.unicode() < 0x0400 || ch.unicode() > 0x04FF)
            return false;
    }
    return true;
}

bool AbstractFindDialog::verifyEdits()
{
    bool isCorrect = true;
    bool *ok = new bool;
    long long num = 0;

    switch (stackedWidget->currentIndex()) {
    case FIRST:
        if(!isName(surnameEdt1->text()) || !surnameEdt1->text().at(0).isUpper()){
            addEffect(surnameEdt1);
            isCorrect = false;
        }
        num = groupEdt->text().toLongLong(ok);
        if(!(*ok) || num < 0 || groupEdt->text().length() != Group::getNumLetters()){
            addEffect(groupEdt);
            isCorrect = false;
        }
        break;
    case SECOND:
        if(!isName(surnameEdt2->text()) || !surnameEdt2->text().at(0).isUpper()){
            addEffect(surnameEdt2);
            isCorrect = false;
        }
        num = numAbsEdt->text().toLongLong(ok);
        if(!(*ok) || num < 0 || numAbsEdt->text().length() > 4){
            addEffect(numAbsEdt);
            isCorrect = false;
        }
        break;
    case THIRD:
        if(!isName(surnameEdt3->text()) || !surnameEdt3->text().at(0).isUpper()){
            addEffect(surnameEdt3);
            isCorrect = false;
        }
        num = lowBoundEdt->text().toLongLong(ok);
        if(!(*ok) || num < 0 || num > INT16_MAX){
            addEffect(lowBoundEdt);
            isCorrect = false;
        }

        num = topBoundEdt->text().toLongLong(ok);
        if(!(*ok) || num < 0 || num > INT16_MAX){
            addEffect(topBoundEdt);
            isCorrect = false;
        }
        if(lowBoundEdt->text().toInt() > topBoundEdt->text().toInt()){
            addEffect(lowBoundEdt);
            addEffect(topBoundEdt);
            isCorrect = false;
        }
        break;
    }

    delete ok;
    changeNumFilledEdits();
    return isCorrect;
}
