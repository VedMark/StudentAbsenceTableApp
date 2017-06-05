#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include "../controller/finddialog.h"

class RemoveDialog : public FindDialog
{
    Q_OBJECT

public:
    RemoveDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);

protected:
    void displayStudentEntryList(const QList<StudentEntry>&) = delete;

protected slots:
    void handleOkBtn();
};

#endif // REMOVEDIALOG_H
