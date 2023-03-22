#include "viewfinances2tenant.h"
#include "ui_viewfinances2tenant.h"

viewFinances2Tenant::viewFinances2Tenant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewFinances2Tenant)
{
    ui->setupUi(this);
}

viewFinances2Tenant::~viewFinances2Tenant()
{
    delete ui;
}
