
#include "viewtenant.h"
#include "ui_viewtenant.h"
#include "selectcontractfortenantview.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

//when this is called, we know the specific tenant adn the property

ViewTenant::ViewTenant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewTenant)
{
    ui->setupUi(this);

}

ViewTenant::~ViewTenant()
{
    delete ui;
}

//set tenant code
void ViewTenant::setTenantCode(QString code)
{
    tenantCode = code;

}//end setTenantCode

void ViewTenant::setProperty(QString selectedProperty)
{
    property = selectedProperty;
}

void ViewTenant::setOccupancy(QString selectedOccupancy)

{
    occupancy = selectedOccupancy;
}

void ViewTenant::display()
{
    qDebug()<<"tenant is "<<tenantCode;
    qDebug()<<"property is "<<property;
    qDebug()<<"occupancy code is "<<occupancy;

    //setup the page with the information
    fillTenant();

    if(property != "No contract found")
    {
        fillRental();
    }
    else
    {
        ui->inputProperty->setText(property);
    }

    void fillFinance();

    this->exec();

}//end display

void ViewTenant::fillFinance()
{

}

void ViewTenant::fillTenant()
{
    //check connection
    connOpen();

    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    //receive the data from the database
    QSqlQuery qry0;
    qry0.prepare("select * from tenant where tenant_code = :tenantCode");
    qry0.bindValue(":tenantCode", tenantCode);
    if(qry0.exec())
    {
        ui->inputCode->setText(tenantCode);
        ui->inputProperty->setText(property);
        while(qry0.next())
        {
            ui->inputFN->setText(qry0.value(1).toString());
            ui->inputLN->setText(qry0.value(2).toString());
            ui->inputEmail->setText(qry0.value(3).toString());
            ui->inputPhone->setText(qry0.value(4).toString());
            ui->inputNotes->setText(qry0.value(5).toString());

            //close the connection
            connClose();
       }
    }//end if qry0 exec
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry0.lastError().text());
        connClose();
    }//end else if qry0 does not exec
}

void ViewTenant::fillRental()
{
    //check connection
    connOpen();

    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }


    QSqlQuery qry;
   qry.prepare("select address from properties where nickname = (:property)");
   qry.bindValue(":property", property);
   if(qry.exec())
   {
       while(qry.next())
       {
         ui->inputAddress->setText(qry.value(0).toString());
         connClose();
       }
    }
    else
    {
         QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
         connClose();
    }

}//end fillRental
