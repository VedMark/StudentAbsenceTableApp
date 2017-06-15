#pragma once

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>

enum Mode{
    FOR_OPEN,
    FOR_SAVE,
};

class NetworkDialog : public QDialog
{
    Q_OBJECT

public:
    NetworkDialog(const QStringList &files, Mode, QWidget *parent);

signals:
    void fileChoosen(const QString&);

private:
    QComboBox *fileCmb;
    QLineEdit *inputEdt;
};
