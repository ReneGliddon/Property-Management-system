#include "updateincome1.h"
#include "ui_updateincome1.h"
#include "updateincome2.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

UpdateIncome1::UpdateIncome1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateIncome1)
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
       QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
       //connClose();
   }

    //save the value of property
    property = ui->comboProperty->currentText();

    //fill in income dates based on property
    QSqlQuery qry;
    QString prop;
    qry.prepare("select income_date from income where property_nickname = :nickname");
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
    incomeDate = ui->comboIncome->currentText();

    //fill in income descriptions based on date
     fillDescription();
    //fillDate();

}

void UpdateIncome1::fillDate(QString arg1)
{
    connOpen();
    //clear the current selection
    ui->comboDate->clear();
    //fill in income dates for property
    QSqlQuery qry;
    qry.prepare("select income_date from income where property_nickname = :nickname");
    qry.bindValue(":nickname", arg1);
    if(qry.exec())
    {
        while(qry.next())
        {
           ui->comboDate->addItem(qry.value(0).toString());
        }
       // connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        //connClose();
    }

    //save the date
    incomeDate = ui->comboDate->currentText();
    property = arg1;


    //fill in income descriptions based on date
    fillDescription();


}

void UpdateIncome1::fillDescription()
{
     qDebug()<<"filling the description";
    //clear the current selection
    ui->comboIncome->clear();
    //fill in income description for property and date
    QSqlQuery qry;
    qry.prepare("select income_code, income_description from income where property_nickname = :nickname and income_date =  :date");
    qry.bindValue(":nickname", property);
    qry.bindValue(":date", incomeDate);
    if(qry.exec())
    {
        while(qry.next())
        {
           QString descript = qry.value(0).toString() + ": " + qry.value(1).toString();
            ui->comboIncome->addItem(descript);
        }
       // connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        //connClose();
    }

}

UpdateIncome1::~UpdateIncome1()
{
    delete ui;
}

void UpdateIncome1::on_pushButton_clicked()
{
    code = ui->comboIncome->currentText();
    UpdateIncome2 updateincome;
    updateincome.display(code);
}

//void UpdateIncome1::on_pushButton_clicked()
//{

//}

void UpdateIncome1::on_comboProperty_currentTextChanged(const QString &arg1)
{
    fillDate(arg1);
}
