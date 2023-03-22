#include "updateexpense2.h"
#include "ui_updateexpense2.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

UpdateExpense2::UpdateExpense2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateExpense2)
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

    //set the date to current as default
    // set the current date as default date.
    QDate date = QDate::currentDate();
    ui->dateIncome->setDate(date);
}

UpdateExpense2::~UpdateExpense2()
{
    delete ui;
}

void UpdateExpense2::display(QString expenseCode)
{
    code = expenseCode;
    this->exec();
}

void UpdateExpense2::on_pushUpdateIncome_clicked()
{
    QMessageBox::information(this, tr("Updated"),tr("Record Updated."));
}
