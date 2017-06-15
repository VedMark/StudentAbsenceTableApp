#pragma once

#include "../controller/abstractfinddialog.h"


class FindDialog: public AbstractFindDialog
{
public:
    FindDialog(StudentAbsenceModel *model, StudentAbsenceClient *client_, QWidget *parent = Q_NULLPTR);
    ~FindDialog();

protected slots:
    virtual void handleOkBtn();
};
