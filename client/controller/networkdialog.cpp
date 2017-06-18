#include "networkdialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

NetworkDialog::NetworkDialog(const QStringList &files, Mode mode, QWidget *parent)
    : QDialog(parent)
{
    fileCmb = new QComboBox(this);
    fileCmb->setEditable(false);
    fileCmb->addItems(files);

    inputEdt = new QLineEdit(this);
    if(mode == FOR_OPEN)
        inputEdt->hide();

    auto chooseBtn = new QPushButton(tr("Выбрать"), this);
    connect(chooseBtn, &QPushButton::clicked, [this, mode] {
        if(mode == FOR_SAVE && inputEdt->text() != "")
            emit fileChoosen(inputEdt->text());
        else
            emit fileChoosen(fileCmb->currentText());
        close();
    });

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel(tr("Выбрать файл"), this));
    mainLayout->addWidget(fileCmb);
    if(mode == FOR_SAVE){
        mainLayout->addWidget(new QLabel(tr("Ввести имя"),this));
        mainLayout->addWidget(inputEdt);
        inputEdt->setToolTip(tr("Будет использовано введённое имя файла"));
    }
    mainLayout->addStretch();
    mainLayout->addWidget(chooseBtn);
    setLayout(mainLayout);
    setMinimumSize(250, 150);
}
