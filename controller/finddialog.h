#pragma once

#include "../controller/abstractfinddialog.h"


class FindDialog: public AbstractFindDialog
{
public:
    FindDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);
    ~FindDialog();

protected slots:
    virtual void handleOkBtn();
};
