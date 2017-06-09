#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include "../controller/abstractfinddialog.h"

class RemoveDialog : public AbstractFindDialog
{
    Q_OBJECT

public:
    RemoveDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);
    ~RemoveDialog();

protected:
    void displayStudentEntryList(StudentAbsenceModel::students);

protected slots:
    void handleOkBtn();
};

#endif // REMOVEDIALOG_H
