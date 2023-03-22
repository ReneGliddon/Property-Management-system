#ifndef ADDNEWEXPENSE_H
#define ADDNEWEXPENSE_H
#include "ui_addnewexpense.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class AddNewExpense;
}

class AddNewExpense : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewExpense(QWidget *parent = 0);
    ~AddNewExpense();
    //define the database connection
    QSqlDatabase mydb;

    void connClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    bool connOpen()
    {
        mydb = QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/QT/Qt5.3.0/Tools/QtCreator/bin/LoginApp/database.db");
        if (!mydb.open())
        {
            qDebug()<<("Failed to open the database");
            return false;
        }
        else
        {
            qDebug()<<("Connected...");
            return true;
        }
    }

    void setExpenseCode(QString, QString, QString, double, QDate);
    int testExpense();

private slots:
    void on_pushAddIncome_2_clicked();

    void on_pushAddExpense_clicked();

private:
    Ui::AddNewExpense *ui;
    QString lineEdit_description, propertyNickname, lineEdit_notes, expenseCode;
    double expenseAmount;
    QDate expenseDate;
    void inputValues();
    //QString property;
    double amount;
};

#endif // ADDNEWEXPENSE_H
