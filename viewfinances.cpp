#include "viewfinances.h"
#include "ui_viewfinances.h"
#include "viewfinances2.h"
#include "viewfinances2tenant.h"

ViewFinances::ViewFinances(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFinances)
{
    ui->setupUi(this);
}

ViewFinances::~ViewFinances()
{
    delete ui;
}

void ViewFinances::on_pushButton_clicked()
{
 viewFinances2 viewFinances;
 viewFinances.display();

 viewFinances2Tenant viewTenant;
 viewTenant.exec();
}
