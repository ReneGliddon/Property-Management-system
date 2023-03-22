#include "addnewrental2.h"
#include "ui_addnewrental2.h"

AddNewRental2::AddNewRental2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewRental2)
{
    ui->setupUi(this);
}

AddNewRental2::~AddNewRental2()
{
    delete ui;
}
