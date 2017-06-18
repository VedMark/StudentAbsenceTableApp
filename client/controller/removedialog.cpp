#include "removedialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>

#include <algorithm>

RemoveDialog::RemoveDialog(ProxyModel *model, StudentAbsenceClient *client_, QWidget *parent)
    : AbstractFindDialog(model, client_, parent)
{
    setWindowTitle(tr("Удалить записи"));
    setWindowIcon(QIcon(":/images/removeEntries.png"));
    okBtn->setText(tr("Удалить"));

    connect(closeBtn, SIGNAL( clicked(bool) ), SLOT( close() ) );
}

RemoveDialog::~RemoveDialog()
{}

void RemoveDialog::displayResult(qint64 count)
{
    if(count == 0){
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Данных по запросу не найдено"),
                                 QMessageBox::Ok);
    }
    else{
        QMessageBox::information(
                    this, "",
                    "Удалено записей по запросу: " +
                    QString::number(count),
                    QMessageBox::Ok);
        emit notNoneResult();
    }
}

void RemoveDialog::closeEvent(QCloseEvent *)
{
    this->~RemoveDialog();
}

void RemoveDialog::handleOkBtn()
{
    if(verifyEdits())
    {
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
    else
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Введены некорректные данные!"),
                                 QMessageBox::Ok);
}
