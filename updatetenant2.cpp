#include "updatetenant2.h"
#include "ui_updatetenant2.h"

UpdateTenant2::UpdateTenant2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateTenant2)
{
    ui->setupUi(this);
}

UpdateTenant2::~UpdateTenant2()
{
    delete ui;
}

void UpdateTenant2::setCode(QString inputCode)
{
    code = inputCode;
}

void UpdateTenant2::display()
{

}
