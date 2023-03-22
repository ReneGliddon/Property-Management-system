#include "viewfinances2.h"
#include "ui_viewfinances2.h"

viewFinances2::viewFinances2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewFinances2)
{
    ui->setupUi(this);
}

viewFinances2::~viewFinances2()
{
    delete ui;
}

void viewFinances2::display()
{

    this->exec();
}
