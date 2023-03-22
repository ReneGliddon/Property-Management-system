#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addnewtenant.h"
#include "addnewexpense.h"
#include "addnewincome.h"
#include "addnewproperty.h"
#include "addnewrental.h"
#include "rentalview.h"
#include "selecttenantrental.h"
#include "viewfinances.h"
#include "viewproperty.h"
#include "selecttenantview.h"
#include "login.h"
#include "updateexpense1.h"
#include "updateincome1.h"
#include "updateproperty.h"
#include "updaterental1.h"
#include "updatetenant1.h"
#include "renewcontract.h"
#include <QMessageBox>
#include <QPixmap>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //check the connection is open
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    //populate properties
    qry1.prepare("select * from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
             ui->comboProperty->addItem(qry1.value(0).toString());
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

    property = ui->comboProperty->currentText();

    //fill tenant
    fillTenant(property);

}
void MainWindow::fillTenant(QString property)
{
    connOpen();

    QString first, last;

    QSqlQuery qry, qry1;

    //reset the code values
    code = "";
    occupancy_code = "";

    //get the new tenant code and occupancy code
    qry.prepare("select occupancy_code, tenant_code from rental where nickname = :property and current_tenant = :yes");
    qry.bindValue(":property", property);
    qry.bindValue(":yes", "Y");
    if(qry.exec())
    {
        while(qry.next())
        {
            occupancy_code = qry.value(0).toString();
            code = qry.value(1).toString();
        }

    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }


    //get the tenants details
    qry1.prepare("select tenant_fn, tenant_ln from tenant where tenant_code = :code");
    qry1.bindValue(":code", code);
    if(qry1.exec())
    {
        while(qry1.next())
        {
            first = qry1.value(0).toString();
            last = qry1.value(1).toString();


            ui->labelfirst->setText(first);
            ui->labellast->setText(last);

        }

        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

    //if there is no tenant at this contract
    if(code=="")
    {
        ui->labelfirst->setText("No current contract at this property");
        ui->labellast->setText("No current contract at this property");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushAdd_clicked()
{
    if (ui->radioAddNewTenant->isChecked())
    {
         AddNewTenant addNewTenant;
         addNewTenant.exec();
    }
    else if (ui->radioAddNewProperty->isChecked())
    {
         AddNewProperty addNewProperty;
         addNewProperty.exec();
    }
    else if (ui->radioAddNewRental->isChecked())
    {
        SelectTenantRental selectTenant;
        selectTenant.exec();
    }
    else if (ui->radioAddNewExpense->isChecked())
    {
        AddNewExpense addNewExpense;
        addNewExpense.exec();
    }
    else if (ui->radioAddNewIncome->isChecked())
    {
        AddNewIncome addNewIncome;
        addNewIncome.exec();
    }
    //if no selection was made, send an error message
    else
    {
        QMessageBox::critical(this, tr("Error"),tr("Error. No selection made."));
    }
}

void MainWindow::on_pushView_clicked()
{
    if (ui->radioViewTenant->isChecked())
    {
       SelectTenantView viewTenant;
       viewTenant.exec();
    }
    else if (ui->radioViewProperty->isChecked())
    {
       ViewProperty viewProperty;
       viewProperty.exec();
    }
    else if (ui->radioViewRental->isChecked())
    {
        rentalView viewRental;
        viewRental.exec();
    }

    else if (ui->radioViewFinances->isChecked())
    {
        ViewFinances viewFinances;
        viewFinances.exec();
    }
    //if no selection was made, send an error message
    else
    {
        QMessageBox::critical(this, tr("Error"),tr("Error. No selection made."));
    }
}//end on_pushView_clicked()

void MainWindow::on_pushRenew_clicked()
{
    //QString property, first, last;
    //property = ui->comboProperty->currentText();

    RenewContract renew;
    renew.display(property, code, occupancy_code);

}

//if a different property is selected
void MainWindow::on_comboProperty_currentTextChanged(const QString &arg1)
{
    property =  ui->comboProperty->currentText();

    fillTenant(arg1);
}


void MainWindow::on_pushUpdate_clicked()
{
    if (ui->radioUpdateExpense->isChecked())
    {
       UpdateExpense1 findExpense;
       findExpense.exec();
    }
    else if (ui->radioUpdateIncome->isChecked())
    {
       UpdateIncome1 findIncome;
       findIncome.exec();
    }
    else if (ui->radioUpdateProperty->isChecked())
    {
        UpdateProperty findProperty;
        findProperty.exec();
    }

    else if (ui->radioUpdateRental->isChecked())
    {
        UpdateRental1 findRental;
        findRental.exec();
    }
    else if (ui->radioUpdateTenant->isChecked())
    {
        UpdateTenant1 findTenant;
        findTenant.exec();
    }
    //if no selection was made, send an error message
    else
    {
        QMessageBox::critical(this, tr("Error"),tr("Error. No selection made."));
    }
}
