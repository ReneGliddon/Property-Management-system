#include "selecttenant.h"
#include "ui_selecttenant.h"
#include "viewtenant.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

SelectTenant::SelectTenant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTenant)
{
    ui->setupUi(this);
}

SelectTenant::~SelectTenant()
{
    delete ui;
}

void SelectTenant::on_pushSearchTenant_clicked()
{
    tenant_fn = ui->lineEdit_tenantFN->text();
    tenant_ln=ui->lineEdit_tenantLN->text();

    //test whether this tenant exists, and if there are more than one by that name, choose which tenant
    int i = testTenant();

    if( i == 1)
    {
        ViewTenant viewTenant;
        viewTenant.setTenantCode(tenantCode);
        viewTenant.exec();
        this->close();
    }
}

int SelectTenant::testTenant()
{
    //check the connection is open
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
        return 0;
    }

    QSqlQuery qry1;

    qry1.prepare("select * from tenant where tenant_fn = '"+tenant_fn+" and tenant_ln = '"+tenant_ln+"'");
    if(qry1.exec())
    {
        int count = 0;
        while(qry1.next())
        {
            count++;
        }

        if(count>0)
        {
            connClose();
            return 1;
        }
        else
        {
            tenantCode = qry1.value(0);
            qDebug()<<"tenant code is "<<tenantCode;
            connClose();
            return 0;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
        return 0;
    }


    return 1;
}
