#include "selecttenantrental.h"
#include "ui_selecttenantrental.h"
#include "login.h"
#include "tenantlist.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QTextDocument>
#include "addnewrental.h"

SelectTenantRental::SelectTenantRental(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTenantRental)
{
    ui->setupUi(this);

    //populate tenants
    Login conn;
    conn.connOpen();
    QSqlQuery qry1;
    
    qry1.prepare("select tenant_fn, tenant_ln, tenant_code from tenant");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString name = qry1.value(0).toString() + " " + qry1.value(1).toString() + " code: " + qry1.value(2).toString();
            ui->comboBox->addItem(name);
        }
        conn.connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

    QString tenantFull = ui->comboBox->currentText();
    int pos = tenantFull.indexOf(":");
    tenant_code = tenantFull.mid(pos+2,-1);

}

SelectTenantRental::~SelectTenantRental()
{
    delete ui;
}

void SelectTenantRental::on_pushSearchTenant_clicked()
{
     AddNewRental newRental;
     newRental.setTenantCode(tenant_code);

     newRental.exec();
     this->close();

}


void SelectTenantRental::on_comboBox_currentTextChanged(const QString &arg1)
{

    QString tenantFull = arg1;
    int pos = tenantFull.indexOf(":");
    tenant_code = tenantFull.mid(pos+2,-1);
}
