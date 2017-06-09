#ifndef ABSTRACTFINDDIALOG_H
#define ABSTRACTFINDDIALOG_H

#include "../controller/abstractfinddialog.h"

class FindDialog: public AbstractFindDialog
{
public:
    FindDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);

protected slots:
    virtual void handleOkBtn();
};

#endif // ABSTRACTFINDDIALOG_H
