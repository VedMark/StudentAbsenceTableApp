#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "../model/studentabsencemodel.h"
#include "../controller/modelcontroller.h"


class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);

signals:
    void entryCreated(const StudentEntry&);
    void numFilledEditsChanged(bool);

private slots:
    void createEntry();
    void changeNumFilledEdits();
    void enableFindButton(bool);

private:
    void connectLineEdits();
    void setOrders();
    void addLayouts();

    void addEffect(QWidget *widget);
    void removeEffect();

    bool isName(const QString& text) const;
    bool verifyEdits();

    const qint8 MAX_NUM_FILLED_EDITS = 7;

    StudentAbsenceModel *model;

    QPushButton *findBtn;
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
