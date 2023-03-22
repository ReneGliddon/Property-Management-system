#include "overrideexpense.h"
#include "ui_overrideexpense.h"
#include "addnewexpense.h"
#include <QMessageBox>
#include <QDebug>

OverrideExpense::OverrideExpense(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverrideExpense)
{
    ui->setupUi(this);
}

OverrideExpense::~OverrideExpense()
{
    delete ui;
}

void OverrideExpense::on_pushButton_clicked()
{
    AddNewExpense code;
    QString overrideCode = ui->lineEdit->text();
    qDebug()<<"Our over ride code is "<<overrideCode;
    code.setExpenseCode(overrideCode, description, propertyNickname, expenseAmount, dateOfExpense);
    this->close();
}

//set the values that won't set with the others for some reason
void OverrideExpense::setValues(QString descript, double amount, QString name, QDate expenseDate)
{
    description = descript;
    expenseAmount = amount;
    propertyNickname = name;
    dateOfExpense = expenseDate;
}
