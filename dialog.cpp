#include "dialog.h"
#include "ui_dialog.h"
#include "addnewincome.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString description = ui->lineEdit->text();
    AddNewIncome back;
    back.setDescription(description);
    this->close();

}
