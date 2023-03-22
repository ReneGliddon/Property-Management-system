#include "updatetenant1.h"
#include "ui_updatetenant1.h"
#include "updatetenant2.h"

UpdateTenant1::UpdateTenant1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTenant1)
{
    ui->setupUi(this);
    //check the connection
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }


    //populate properties combobox
    QSqlQuery qry1;
    qry1.prepare("select tenant_fn, tenant_ln, tenant_code from tenant");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString tenant = qry1.value(0).toString() + " " + qry1.value(1).toString() + " (code: " + qry1.value(2).toString() + ")";
            ui->comboBox->addItem(tenant);
        }

   }
   else
   {
       QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
       //connClose();
   }
}

UpdateTenant1::~UpdateTenant1()
{
    delete ui;
}

void UpdateTenant1::on_pushSearchTenant_clicked()
{
    //just here so the code runs
}




void UpdateTenant1::on_pushSaveTenant_clicked()
{
     QMessageBox::information(this, tr("Updated"),tr("Record Updated."));
    //connClose();
}
