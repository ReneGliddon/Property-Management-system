#include "addnewproperty.h"
#include "ui_addnewproperty.h"
#include "login.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

AddNewProperty::AddNewProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewProperty)
{
    ui->setupUi(this);
    // set the current date as default purchase date.
    QDate date = QDate::currentDate();
    ui->datePurchase->setDate(date);

}

AddNewProperty::~AddNewProperty()
{
    delete ui;
}

void AddNewProperty::on_pushSavePropertyDetails_clicked()
{
    //open the database
    Login conn;
    conn.connOpen();
    if(!conn.connOpen())
    {
        qDebug()<<"Failed to open the database";
        return;
    }

    //save the text inputs
    QString lineEdit_nickname, lineEdit_address, lineEdit_sewage, lineEdit_utility, lineEdit_notes;

    lineEdit_nickname = ui->lineEdit_nickname->text();
    lineEdit_address = ui->lineEdit_address->text();
    lineEdit_sewage = ui->lineEdit_sewage->text();
    lineEdit_utility = ui->lineEdit_utility->text();
    lineEdit_notes = ui->lineEdit_notes->text();

    //number values
    double purchaseCost = ui->doubleSpinPurchaseCost->value();
     qDebug()<<purchaseCost;
      qDebug()<<ui->doubleSpinPurchaseCost->value();
    double mortgage = ui->doubleSpinMonthlyMortgage->value();
     qDebug()<<mortgage;
      qDebug()<<ui->doubleSpinMonthlyMortgage->value();
    double HOA = ui->doubleSpinHOA->value();
    int mortgageYears = ui->spinMortgageYears->value();

    //save purchase date
    QDate date_purchase = ui->datePurchase->date();

    // prepare the database queries
    QSqlQuery qry1;
    QSqlQuery qry2;
    //check some property details aren't blank
    if (!(lineEdit_nickname=="" || lineEdit_address=="" || lineEdit_sewage==""|| lineEdit_utility==""))
    {
        //check that the given nickname is unique
        qry1.prepare("select * from properties where nickname = '"+lineEdit_nickname+"'");
        if(qry1.exec())
        {
            int count = 0;
            while(qry1.next())
            {
               count++;
            }
            //if that nickname is already in use
            if (count >= 1)
            {
                QMessageBox::critical(this, tr("Error"),tr("Error. That nickname is taken. Try another."));
            }
            //otherwise, input the data into the database
            else
            {
                qry2.prepare("insert into properties(nickname, address,  purchase_cost, purchase_date, monthly_mortgage, mortgage_term_years, HOA, sewage, utility, notes) values (:lineEdit_nickname, :lineEdit_address, :purchaseCost, :purchase_date, :mortgage, :mortgageYears, :HOA, :lineEdit_sewage,:lineEdit_utility,:lineEdit_notes)");
                qry2.bindValue(":lineEdit_nickname", lineEdit_nickname);
                qry2.bindValue(":lineEdit_address", lineEdit_address);
                qry2.bindValue(":purchaseCost", purchaseCost);
                qry2.bindValue(":purchase_date", date_purchase);
                qry2.bindValue(":mortgage", mortgage);
                qry2.bindValue(":mortgageYears", mortgageYears);
                qry2.bindValue(":HOA", HOA);
                qry2.bindValue(":lineEdit_sewage",lineEdit_sewage);
                qry2.bindValue(":lineEdit_utility", lineEdit_utility);
                qry2.bindValue(":lineEdit_notes", lineEdit_notes);

                if(qry2.exec())
                {
                    QMessageBox::information(this, tr("Save"), tr("Property information saved successfully."));
                    conn.connClose();
                }
                else
                    QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());

            }//end input into database

        }//end  if(qry1.exec())
        else
            QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());

    }
 else
     QMessageBox::critical(this, tr("Error"),tr("Error. Fields cannot be blank."));
}//end on_pushSavePropertyDetails_clicked()
