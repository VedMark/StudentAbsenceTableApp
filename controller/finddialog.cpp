#include "finddialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

FindDialog::FindDialog(StudentAbsenceModel *model, QWidget *parent)
    :AbstractFindDialog(model, parent)
{
    okBtn->setText("Найти");
    setWindowTitle(tr("Найти записи"));
}

void FindDialog::handleOkBtn()
{
    if(verifyEdits())
    {
        std::function<bool (const StudentEntry &)> cond = condition();

        StudentAbsenceModel::students searchResult;
        std::copy_if(model->getStudentEntryList().begin(),
                     model->getStudentEntryList().end(),
                     std::back_inserter(searchResult),
                     cond);

        if(searchResult.isEmpty()){
            QMessageBox::information(this, "Внимание!", "Данных по запросу не найдено", QMessageBox::Ok);
            resultTable->setVisible(false);
            resize(sizeHint());
            move((QApplication::desktop()->width() - width()) / 2,
                 (QApplication::desktop()->height() - height()) / 2);
        }
        else{
            displayStudentEntryList(searchResult);
        }
    }
    else
        QMessageBox::information(this, "Внимание!", "Введены некорректные данные!", QMessageBox::Ok);
}
