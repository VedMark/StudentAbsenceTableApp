#include "removedialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

#include <algorithm>

RemoveDialog::RemoveDialog(StudentAbsenceModel *model, QWidget *parent)
    : FindDialog(model, parent)
{
    setWindowTitle("Удалить записи");
    okBtn->setText("Удалить");
}

void RemoveDialog::handleOkBtn()
{
    if(verifyEdits())
    {
        QList<StudentEntry>& studentsList = model->getStudentEntryList();
        qint64 length_before = model->getStudentEntryList().length();
        studentsList.erase(std::remove_if(studentsList.begin(), studentsList.end(), condition()),
                           studentsList.end());

        qint64 length_after = model->getStudentEntryList().length();
        if(length_before - length_after == 0){
            QMessageBox::information(this, "", "Данных по запросу не найдено", QMessageBox::Ok);
        }
        else
            QMessageBox::information(
                this, "",
                "Удалено записей по запросу: " +
                QString::number(length_before - length_after),
                QMessageBox::Ok);
    }
    else
        QMessageBox::information(this, "Внимание!", "Введены некорректные данные!", QMessageBox::Ok);
}
