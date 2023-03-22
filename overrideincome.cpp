#include "overrideincome.h"
#include "ui_overrideincome.h"
#include "addnewincome.h"
#include <QMessageBox>
#include <QDebug>

OverrideIncome::OverrideIncome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverrideIncome)
{
    ui->setupUi(this);
}

OverrideIncome::~OverrideIncome()
{
    delete ui;
}

void OverrideIncome::on_pushButton_clicked()
{
    setIncomeCode();
}

//set the values that won't set with the others for some reason
void OverrideIncome::setValues(QString descript, QString property, double amount, QDate date, QString payer)
{
    //text inputs
    propertyNickname = property;
    description = descript;
    incomeAmount = amount;
    payerCode = payer;

    //number value
    incomeAmount = amount;

    //income date
    incomeDate = date;
}

void OverrideIncome::setIncomeCode()
{
    //set code
     incomeCode = ui->lineEdit->text();

     //test if this code is unique
     int test = testIncome();

     //if it is unique, input the values into the database
     if(test == 1)
     {
        //input these values
        inputValues();

        //close boxes
         this->close();

      }
      //else if overidden code is not unique
      else
      {
          //prompt user to enter a different code
            QMessageBox::critical(this, tr("Saved"), tr("Duplicate code. Please enter a different income code."));
      }
}

int OverrideIncome::testIncome()
{
    //open the database
    connOpen();
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    //see if there is already a record of this code in the database
    QSqlQuery qry;
    qry.prepare("select * from income where income_code = :incomeCode");
    qry.bindValue(":incomeCode", incomeCode);
    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        //code already exists in database
        if(count>0)
        {
            connClose();
            return 0;
        }
        //code is unique
        else
        {
            connClose();
            return 1;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
        return 0;
    }
}

void OverrideIncome::inputValues()
{
    connOpen();
    QSqlQuery qry;
    qry.prepare("insert into income(income_code, property_nickname, income_amount, income_description, income_date, payer) values (:incomeCode, :propertyNickname, :incomeAmount, :lineEdit_description, :incomeDate, :payerCode)");
    qry.bindValue(":incomeCode", incomeCode);
    qry.bindValue(":propertyNickname", propertyNickname);
    qry.bindValue(":incomeAmount", incomeAmount);
    qry.bindValue(":lineEdit_description", description);
    qry.bindValue(":incomeDate", incomeDate);
    qry.bindValue(":payerCode", payerCode);
    if(qry.exec())
    {
        QMessageBox::information(this, tr("Saved"), tr("Income saved successfully."));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }
}
