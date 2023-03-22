#include "updateincome2.h"
#include "ui_updateincome2.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
UpdateIncome2::UpdateIncome2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateIncome2)
{
    ui->setupUi(this);

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

    //set the date to current as default
    // set the current date as default date.
    QDate date = QDate::currentDate();
    ui->dateIncome->setDate(date);
}

UpdateIncome2::~UpdateIncome2()
{
    delete ui;
}

void UpdateIncome2::display(QString codeImport)
{
    code = codeImport;

    this->exec();
}

void UpdateIncome2::on_pushUpdateIncome_clicked()
{
     QMessageBox::information(this, tr("Updated"),tr("Record Updated."));
}
