#include "tenantlist.h"
#include "ui_tenantlist.h"
#include "addnewrental.h"
#include "login.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
/*

 */
tenantList::tenantList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tenantList)
{
    ui->setupUi(this);

}

tenantList::~tenantList()
{
    delete ui;
}

void tenantList::on_pushButton_clicked()
{
    AddNewRental setTenant;
    setTenant.setTenantCode(ui->lineEdit_tenantCode->text());
    setTenant.exec();
    this->close();
}

void tenantList::setList(QString list)
{
    tenants = list;
}

void tenantList::display()
{
    qDebug()<<"in tenant list display function. list codes sent are "<<tenants;
    ui->texttenants->setText(tenants);
    this->exec();
}
