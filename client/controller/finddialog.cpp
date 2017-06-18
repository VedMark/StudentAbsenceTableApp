#include "finddialog.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>


FindDialog::FindDialog(ProxyModel *model, StudentAbsenceClient *client_, QWidget *parent)
    :AbstractFindDialog(model, client_, parent)
{
    okBtn->setText(tr("Найти"));
    setWindowIcon(QIcon(":/images/findEntries.png"));
    setWindowTitle(tr("Найти записи"));

    connect(closeBtn, SIGNAL( clicked(bool) ), SLOT( close() ) );
}

FindDialog::~FindDialog()
{}

void FindDialog::displayResult(const ProxyModel::Students &searchResult)
{
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
        ProxyModel *mod = new ProxyModel(this);
        mod->setStudentEntryList(searchResult);

        resultTable->setModel(mod);
        resultTable->setVisible(true);

        resize(sizeHint());

        move((QApplication::desktop()->width() - width()) / 2,
             (QApplication::desktop()->height() - height()) / 2);
    }
}

void FindDialog::closeEvent(QCloseEvent *)
{
    this->~FindDialog();
}

void FindDialog::handleOkBtn()
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
        client->sendFindRequest((SearchPattern)stackedWidget->currentIndex(),
                                list);
    }
    else
        QMessageBox::information(this, tr("Внимание!"),
                                 tr("Введены некорректные данные!"),
                                 QMessageBox::Ok);
}
