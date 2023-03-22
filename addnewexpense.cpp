#include "addnewexpense.h"
#include "ui_addnewexpense.h"
#include "overrideexpense.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

AddNewExpense::AddNewExpense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewExpense)
{
    ui->setupUi(this);

    //check the connection is open
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    qry1.prepare("select * from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
             ui->comboProperty->addItem(qry1.value(0).toString());
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

    // set the current date as default date.
    QDate date = QDate::currentDate();
    ui->dateExpense->setDate(date);

}

AddNewExpense::~AddNewExpense()
{
    delete ui;
}

//leaving this here so my code runs
void AddNewExpense::on_pushAddIncome_2_clicked()
{
    //ignore
}

void AddNewExpense::on_pushAddExpense_clicked()
{
    //save the text inputs
    lineEdit_description = ui->lineEditOtherExpenseDescription->text();
    propertyNickname = ui->comboProperty->currentText();

    qDebug()<<"Our property is "<<propertyNickname;

    //number value
    expenseAmount = ui->doubleSpinExpense->value();

    //save purchase date
     expenseDate = ui->dateExpense->date();

    //generate an expense code
    //first create a test expense code
      expenseCode = propertyNickname.mid(1,2);
      QString lnAppend = expenseDate.toString("yyyy-MM-dd").mid(5,2);
      expenseCode.append(lnAppend);
      lnAppend = lineEdit_description.mid(0,2);
      expenseCode.append(lnAppend);

    //check it is unique
      int expenseCodeTest = testExpense();

      if(expenseCodeTest==1)
          inputValues();
      else
      {
        //expense code is not unique, prompt user to manually enter a unique code
        //create an object
        OverrideExpense override;
        //send and set the values that disappear otherwise
        override.setValues(lineEdit_description, expenseAmount, propertyNickname, expenseDate);
        //display the override request. values will be entered after a unique code is received
        override.exec();
      }
}

int AddNewExpense::testExpense()
{
    qDebug()<<"testing the code";
    //open the database
    connOpen();
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    //see if there is already a record of this code in the database
    QSqlQuery qry2;
    qry2.prepare("select * from expense where expense_code = :expenseCode");
    qry2.bindValue(":expenseCode", expenseCode);
    if(qry2.exec())
    {
        int count = 0;
        while(qry2.next())
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
        QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());
        connClose();
        return 0;
    }

}

void AddNewExpense::inputValues()
{
    connOpen();

    qDebug()<<"Inputting the expnse and the property is "<<propertyNickname;
    QSqlQuery qry;
    qry.prepare("insert into expense(expense_code, property_nickname, expense_amount, expense_description, expense_date) values (:expenseCode, :propertyNickname, :expenseAmount, :lineEdit_description, :expenseDate)");
    qry.bindValue(":expenseCode", expenseCode);
    qry.bindValue(":propertyNickname", propertyNickname);
    qry.bindValue(":expenseAmount", expenseAmount);
    qry.bindValue(":lineEdit_description", lineEdit_description);
    qry.bindValue(":expenseDate", expenseDate);
    if(qry.exec())
    {
        QMessageBox::information(this, tr("Saved"), tr("Expense saved successfully."));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }
}

//set expense code via override
void AddNewExpense::setExpenseCode(QString code, QString description, QString propertyName, double amount, QDate date)
{
    //set code
    expenseCode = code;

    //reset property and amount
    lineEdit_description = description;
    expenseAmount = amount;
    propertyNickname = propertyName;
    expenseDate = date;

    //test if this code is unique
    int test = testExpense();

    //if it is unique, input the values into the database
    if(test == 1)
    {
        //reset the other values
        //save the text inputs
        //propertyNickname=ui->comboProperty->currentText();

        //save purchase date
         //expenseDate = ui->dateExpense->date();

         //input these values
        inputValues();

        //close boxes
        this->close();
    }
    //overidden code is not unique
    else
    {
        //prompt user to enter a different code
        QMessageBox::critical(this, tr("Saved"), tr("Duplicate code. Please enter a different expense code."));
    }
}

