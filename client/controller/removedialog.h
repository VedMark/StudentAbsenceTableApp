#pragma once

#include "../controller/abstractfinddialog.h"


class RemoveDialog : public AbstractFindDialog
{
    Q_OBJECT

public:
    RemoveDialog(ProxyModel *model, StudentAbsenceClient *client_, QWidget *parent = Q_NULLPTR);
    ~RemoveDialog();

public slots:
    void displayResult(qint64 count);

signals:
    void notNoneResult();

protected:
    void closeEvent(QCloseEvent *);

protected slots:
    void handleOkBtn();
};
