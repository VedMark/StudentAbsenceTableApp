#include "finddialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>


FindDialog::FindDialog(StudentAbsenceModel *model, StudentAbsenceClient *client_, QWidget *parent)
    :AbstractFindDialog(model, client_, parent)
{
    okBtn->setText(tr("Найти"));
    setWindowIcon(QIcon(":/images/findEntries.png"));
    setWindowTitle(tr("Найти записи"));
}

FindDialog::~FindDialog()
{}

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
            QMessageBox::information(this, tr("Внимание!"),
                                     tr("Данных по запросу не найдено"),
                                     QMessageBox::Ok);
            resultTable->setVisible(false);
            resize(sizeHint());
            move((QApplication::desktop()->width() - width()) / 2,
                 (QApplication::desktop()->height() - height()) / 2);
        }
        else{
            displayStudentEntryList(searchResult);
        }
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
            client->sendFindRequest((SearchPattern)stackedWidget->currentIndex(),
                                    list);
        }

    }
    else
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Введены некорректные данные!"),
                                 QMessageBox::Ok);
}
