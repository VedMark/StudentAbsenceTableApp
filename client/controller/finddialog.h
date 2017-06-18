#pragma once

#include "../controller/abstractfinddialog.h"


class FindDialog: public AbstractFindDialog
{
    Q_OBJECT

public:
    FindDialog(ProxyModel *model, StudentAbsenceClient *client_, QWidget *parent = Q_NULLPTR);
    ~FindDialog();

public slots:
    void displayResult(const ProxyModel::Students&);

protected:
    void closeEvent(QCloseEvent *);

protected slots:
    virtual void handleOkBtn();
};
