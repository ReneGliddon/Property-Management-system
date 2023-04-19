#include "updaterental1.h"
#include "ui_updaterental1.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

UpdateRental1::UpdateRental1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateRental1)
{
    ui->setupUi(this);
    //check the connection
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    // set the current date as default rental start date.
    QDate date = QDate::currentDate();
    ui->dateStart->setDate(date);
    ui->dateEnd->setDate(date);


    //populate properties comboboxes
    QSqlQuery qry, qry1, qry2;
    qry.prepare("select nickname from properties");
    if(qry.exec())
    {
        while(qry.next())
        {
           ui->comboProperty->addItem(qry.value(0).toString());
           ui->comboProperty_2->addItem(qry.value(0).toString());
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry.lastError().text());
    }


    //populate the tenant contract drop down menu
    qry1.prepare("select occupancy_code, tenant_code from rental where nickname = :property");
    qry1.bindValue(":property", ui->comboProperty->currentText());
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString occupancy_code = qry1.value(0).toString();
            qry2.prepare("select tenant_fn, tenant_ln from tenant where tenant_code = :code");
            qry2.bindValue(":code", qry1.value(1).toString());
            if(qry2.exec())
            {
                while(qry2.next())
                {
                    QString tenant = qry2.value(0).toString() + " " + qry2.value(1).toString() + " (occupancy code: " + occupancy_code + ")";
                    ui->comboTenant->addItem(tenant);
                }
            }
        }



    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
    }


}

UpdateRental1::~UpdateRental1()
{
    delete ui;
}

void UpdateRental1::on_pushUpdateRentalDetails_clicked()
{

    QMessageBox::information(this, tr("Updated"),tr("Record Updated."));
}

void UpdateRental1::on_comboProperty_currentTextChanged(const QString &tenantName)
{
    //pass
}
