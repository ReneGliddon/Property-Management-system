#include "viewproperty.h"
#include "ui_viewproperty.h"
#include "viewproperty2.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

ViewProperty::ViewProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewProperty)
{
    ui->setupUi(this);
    //check the connection is open
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    qry1.prepare("select * from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
             ui->comboBox->addItem(qry1.value(0).toString());
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

}

ViewProperty::~ViewProperty()
{
    delete ui;
}

void ViewProperty::on_pushButton_clicked()
{
    //save the chosen property
    property = ui->comboBox->currentText();

    //declare an object of ViewProperty2
    ViewProperty2 showProperty;

    //set the property of ViewProperty2 to the selected property
    showProperty.setProperty(property);;

    //close this dialog
    this->close();

    //call ViewProperty2
    showProperty.display(); 

}
