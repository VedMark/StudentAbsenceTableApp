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
#include <QTableView>

#include "../model/studentabsencemodel.h"
#include "../view/HierarchicalHeaderView.h"


class AbstractFindDialog : public QDialog
{
    Q_OBJECT

public:
    AbstractFindDialog(StudentAbsenceModel *model, QWidget *parent = Q_NULLPTR);
    ~AbstractFindDialog();


signals:
    void numFilledEditsChanged(int);

protected:
    enum SearchPattern{
        FIRST,
        SECOND,
        THIRD
    };

    virtual void displayStudentEntryList(const StudentAbsenceModel::students&);

    const qint8 MAX_NUM_FILLED_EDITS = 2;


    std::function<bool (const StudentEntry &)> condition();

    void addEffect(QWidget *widget);

    bool isName(const QString& text) const;
    bool verifyEdits();


    StudentAbsenceModel *model;

    QPushButton *okBtn;
    QPushButton *closeBtn;

    QTableView *resultTable;

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
    virtual void handleOkBtn() = 0;
    virtual void changeNumFilledEdits();
    virtual void enableOkButton(int);

private:
    void configStackedWidget();
    void connectLineEdits();
    void addLayouts();
};

#endif // FINDDIALOG_H
