#include "removedialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

#include <algorithm>

RemoveDialog::RemoveDialog(StudentAbsenceModel *model, StudentAbsenceClient *client_, QWidget *parent)
    : AbstractFindDialog(model, client_, parent)
{
    setWindowTitle("Удалить записи");
    setWindowIcon(QIcon(":/images/removeEntries.png"));
    okBtn->setText("Удалить");
}

RemoveDialog::~RemoveDialog()
{}


void RemoveDialog::handleOkBtn()
{
    if(verifyEdits())
    {
        StudentAbsenceModel::students &studentList = model->getStudentEntryList();
        auto length_before = model->getStudentEntryList().length();

        std::function<bool (const StudentEntry &)> cond = condition();

        studentList.erase(
                    std::remove_if(studentList.begin(),studentList.end(), cond),
                    studentList.end());

        auto length_after = model->getStudentEntryList().length();
        model->removeRows(length_after, length_before - length_after, QModelIndex());

        if(length_before - length_after == 0){
            QMessageBox::information(this, tr("Внимание!"),
                                     tr("Данных по запросу не найдено"),
                                     QMessageBox::Ok);
            if(client){
                QStringList list;
                switch ((SearchPattern)stackedWidget->currentIndex()) {
                case FIRST:
                    list.append(surnameEdt1->text());
                    list.append(groupEdt->text());
                    break;
                case SECOND:
                    list.append(surnameEdt2->text());
                    list.append(QString::number(absKindCmb->currentIndex()));
                    list.append(numAbsEdt->text());
                    break;
                case THIRD:
                    list.append(surnameEdt3->text());
                    list.append(lowBoundEdt->text());
                    list.append(topBoundEdt->text());
                    break;
                }
                client->sendRemoveRequest((SearchPattern)stackedWidget->currentIndex(),
                                        list);
            }
        }
        else
            QMessageBox::information(
                this, "",
                "Удалено записей по запросу: " +
                QString::number(length_before - length_after),
                QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Введены некорректные данные!"),
                                 QMessageBox::Ok);
}
