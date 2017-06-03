#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "../controller/modelcontroller.h"
#include "../model/studentabsencemodel.h"


class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QAbstractTableModel *model, QWidget *parent = Q_NULLPTR);

signals:
    void entryCreated(const StudentEntry&);
    void numFilledEditsChanged(bool);

private slots:
    StudentEntry *createEntry();
    void changeNumFilledEdits();
    void enableFindButton(bool);

private:
    void connectLineEdits();
    void setOrders();
    void addLayouts();

    bool isName(const QString& text) const;
    bool verifyEdits();

    const qint8 MAX_NUM_FILLED_EDITS = 7;

    QAbstractTableModel *model;

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
