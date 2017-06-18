#pragma once

#include "../view/proxymodel.h"
#include "../studentabsenceclient.h"

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(ProxyModel *model, StudentAbsenceClient * client, QWidget *parent = Q_NULLPTR);
    ~AddDialog();

signals:
    void numFilledEditsChanged(int);
    void notNoneResult();

protected:
    void closeEvent(QCloseEvent *);

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

    ProxyModel *model;
    StudentAbsenceClient *client;

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
