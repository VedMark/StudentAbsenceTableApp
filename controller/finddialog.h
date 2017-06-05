#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QRadioButton>
#include <QStackedWidget>

#include "../model/studentabsencemodel.h"
#include "../view/studenttableview.h"

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);
    ~FindDialog();


signals:
    void numFilledEditsChanged(int);

protected:
    enum SearchPattern{
        FIRST,
        SECOND,
        THIRD
    };

    void displayStudentEntryList(const QList<StudentEntry>&);

    const qint8 MAX_NUM_FILLED_EDITS = 2;


    std::function<bool (const StudentEntry &)> condition();

    void addEffect(QWidget *widget);

    bool isName(const QString& text) const;
    bool verifyEdits();


    StudentAbsenceModel *model;

    QPushButton *okBtn;
    QPushButton *closeBtn;

    StudentTableView *resultTable;

    QStackedWidget *stackedWidget;

    QGroupBox *firstGroup;
    QGroupBox *secondGroup;
    QGroupBox *thirdGroup;

    QLabel *surnameLbl1;
    QLabel *groupLbl;
    QLineEdit *surnameEdt1;
    QLineEdit *groupEdt;

    QLabel *surnameLbl2;
    QLabel *absKindlbl;
    QLabel *numAbsLbl;
    QLineEdit *surnameEdt2;
    QComboBox *absKindCmb;
    QLineEdit *numAbsEdt;

    QLabel *surnameLbl3;
    QLabel *lowBoundLbl;
    QLabel *topBoundLbl;
    QLineEdit *surnameEdt3;
    QLineEdit *lowBoundEdt;
    QLineEdit *topBoundEdt;

    QGroupBox *switcher;
    QRadioButton *firstPattern;
    QRadioButton *secondPattern;
    QRadioButton *thirdPattern;

    QList<QLineEdit*> listEdits;

protected slots:
    virtual void handleOkBtn();
    virtual void changeNumFilledEdits();
    virtual void enableOkButton(int);

private:
    void configStackedWidget();
    void connectLineEdits();
    void addLayouts();
};

#endif // FINDDIALOG_H
