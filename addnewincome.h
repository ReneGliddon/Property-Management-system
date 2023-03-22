#ifndef ADDNEWINCOME_H
#define ADDNEWINCOME_H
#include "ui_addnewincome.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class AddNewIncome;
}

class AddNewIncome : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewIncome(QWidget *parent = 0);
    ~AddNewIncome();

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

    void setIncomeCode(QString, QString, double);
    int testIncome();
     void setDescription(QString descriptionArg);

private slots:
    void on_pushAddIncome_2_clicked();

    void on_comboProperty_currentTextChanged(const QString &arg1);

private:
    Ui::AddNewIncome *ui;
    //text inputs
    QString description,propertyNickname, payerName;
    //number value
    double incomeAmount;
    //income date
    QDate incomeDate;
    //income code
    QString incomeCode;

    //functions
    void inputValues();
    void pushCont();

};

#endif // ADDNEWINCOME_H
