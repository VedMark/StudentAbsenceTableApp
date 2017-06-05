#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "../model/studentabsencemodel.h"


class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);

signals:
    void numFilledEditsChanged(int);

protected slots:
    virtual void createEntry();
    virtual void changeNumFilledEdits();
    virtual void enableAddButton(int num);

private:
    void connectLineEdits();
    void setOrders();
    void addLayouts();

    void addEffect(QWidget *widget);

    bool isName(const QString& text) const;
    bool verifyEdits();

    const qint8 MAX_NUM_FILLED_EDITS = 7;

    StudentAbsenceModel *model;

    QPushButton *addBtn;
    QPushButton *closeBtn;

    QLabel *surnameLbl;
    QLabel *nameLbl;
    QLabel *patronymicLbl;
    QLabel *groupLbl;
    QLabel *absenceLbl;
    QLabel *illnessLbl;
    QLabel *anotherLbl;
    QLabel *hookyLbl;

    QLineEdit *surnameEdt;
    QLineEdit *nameEdt;
    QLineEdit *patronymicEdt;
    QLineEdit *groupEdt;
    QLineEdit *illnessEdt;
    QLineEdit *anotherEdt;
    QLineEdit *hookyEdt;

    QList<QLineEdit*> listEdits;
};

#endif // ADDDIALOG_H
