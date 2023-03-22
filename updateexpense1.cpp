#include "updateexpense1.h"
#include "ui_updateexpense1.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include "updateexpense2.h"

UpdateExpense1::UpdateExpense1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateExpense1)
{
    ui->setupUi(this);
    //check the connection
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }


    //populate properties combobox
    QSqlQuery qry1;
    qry1.prepare("select nickname from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            ui->comboProperty->addItem(qry1.value(0).toString());
        }

   }
   else
   {
       QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
       //connClose();
   }

    //save the value of property
    property = ui->comboProperty->currentText();

    //fill in expense dates based on property
    QSqlQuery qry;
    QString prop;
    qry.prepare("select expense_date from expense where property_nickname = :nickname");
    qry.bindValue(":nickname", prop);
    if(qry.exec())
    {
        qDebug()<<"filling the date";
        while(qry.next())
        {
           qDebug()<<"filling the date in the while";
           ui->comboDate->addItem(qry.value(0).toString());
        }
        //connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        //connClose();
    }

    //save the date
    expenseDate = ui->comboExpense->currentText();

    //fill in expense descriptions based on date
    fillDescription();
   // fillDate();
}

void UpdateExpense1::fillDate(QString arg1)
{
    connOpen();
    //clear the current selection
    ui->comboDate->clear();
    //fill in income dates for property
    QSqlQuery qry;
    qry.prepare("select expense_date from expense where property_nickname = :nickname");
    qry.bindValue(":nickname", arg1);
    if(qry.exec())
    {
        qDebug()<<"filling the date";
        while(qry.next())
        {
           qDebug()<<"filling the date in the while";
           ui->comboDate->addItem(qry.value(0).toString());
        }
        //connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        //connClose();
    }

    //save the date
    expenseDate = ui->comboExpense->currentText();


    //save the date
    expenseDate = ui->comboDate->currentText();
    property = arg1;


    //fill in income descriptions based on date
    fillDescription();
}

void UpdateExpense1::fillDescription()
{
    qDebug()<<"filling the description";
   //clear the current selection
   ui->comboExpense->clear();
   //fill in income description for property and date
   QSqlQuery qry;
   qry.prepare("select expense_code, expense_description from expense where property_nickname = :nickname and expense_date =  :date");
   qry.bindValue(":nickname", property);
   qry.bindValue(":date", expenseDate);
   if(qry.exec())
   {
       while(qry.next())
       {
          QString descript = qry.value(0).toString() + ": " + qry.value(1).toString();
           ui->comboExpense->addItem(descript);
       }
      // connClose();
   }
   else
   {
       QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
       //connClose();
   }
}

UpdateExpense1::~UpdateExpense1()
{
    delete ui;
}

//void UpdateExpense1::on_pushButton_clicked()
//{
  //  QMessageBox::information(this, tr("Saved"), tr("Income saved successfully."));
   // connClose();
//}

void UpdateExpense1::on_comboProperty_currentTextChanged(const QString &arg1)
{
    fillDate(arg1);
}

void UpdateExpense1::on_pushButton_clicked()
{
    UpdateExpense2 updateexpense;
    updateexpense.display(code);
}
