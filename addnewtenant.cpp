#include "addnewtenant.h"
#include "ui_addnewtenant.h"
#include "login.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

AddNewTenant::AddNewTenant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewTenant)
{
    ui->setupUi(this);
}

AddNewTenant::~AddNewTenant()
{
    delete ui;
}

void AddNewTenant::on_pushSaveTenant_clicked()
{
    //open the database
    Login conn;
    conn.connOpen();
    if(!conn.connOpen())
    {
        qDebug()<<"Failed to open the database";
        return;
    }

    //save the text inputs
    lineEdit_tenantFN = ui->lineEdit_tenantFN->text();
    lineEdit_tenantLN = ui->lineEdit_tenantLN->text();
    lineEdit_tenantEmail = ui->lineEdit_tenantEmail->text();
    lineEdit_tenantPhone = ui->lineEdit_tenantPhone->text();
    lineEdit_notesTenant = ui->lineEdit_notesTenant->text();
    lineEdit_tenantCode = ui->lineEdit_tenantCode->text();

    // prepare the database queries
    QSqlQuery qry0;


    //check tenant names have been entered
    if (!(lineEdit_tenantFN=="" || lineEdit_tenantLN==""))
    {
        //first check if another tenant of the same name is already in the system
        qry0.prepare("select tenant_fn, tenant_ln from tenant where (tenant_fn = :lineEdit_tenantFN and tenant_ln = :lineEdit_tenantLN)");
        qry0.bindValue(":lineEdit_tenantFN", lineEdit_tenantFN);
        qry0.bindValue(":lineEdit_tenantLN", lineEdit_tenantLN);
        if(qry0.exec())
        {
            int count = 0;
            while(qry0.next())
                count++;
            if(count>0)
            {
                QMessageBox multipleTenant;
                multipleTenant.setText("Tenant by that name is already in the database.");
                multipleTenant.setInformativeText("Do you want to save this tenant anyway?");
                multipleTenant.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
                multipleTenant.setDefaultButton(QMessageBox::Save);
                int ret = multipleTenant.exec();
                switch (ret)
                {
                  case QMessageBox::Save:
                      {
                        generateTenantCode();
                      }
                      break;
                  case QMessageBox::Cancel:
                      multipleTenant.close();
                              //multipleTenant.hide();
                      break;
                  default:
                      // should never be reached
                      break;
                }//end switch (ret)
            }//end if tenant by that name is already in the system
            else
            {
                qDebug()<<"in no duplicates";
                qDebug()<<"tenant code:";
                qDebug()<<lineEdit_tenantCode;

                generateTenantCode();
            }//end else if no duplicate tenants where found

        }//end if(qry0.exec())
        else
        {
            QMessageBox::critical(this, tr("Error:: "), qry0.lastError().text());
            conn.connClose();
        }//end else if qry0 !exec()


    }//end if (!(lineEdit_tenantFN=="" || lineEdit_tenantLN==""))
    else
     QMessageBox::critical(this, tr("Error"),tr("Error. Fields cannot be blank."));
}//end pushSaveTenant



void AddNewTenant::generateTenantCode()
{
    QSqlQuery qry1;
    qDebug()<<"in generate tenant code";
    qDebug()<<lineEdit_tenantCode;
    //if the tenant code has not been overridden, generate one

    if(lineEdit_tenantCode=="")
    {
          //first create a test tenant code
          lineEdit_tenantCode = lineEdit_tenantFN.mid(1,2);
          QString lnAppend = lineEdit_tenantLN.mid(1,2);
          lineEdit_tenantCode.append(lnAppend);
          QString date = QDateTime::currentDateTime().toString().mid(10,1);
          QString date2 = QDateTime::currentDateTime().toString();
          lineEdit_tenantCode.append(date);
          date = QDateTime::currentDateTime().toString().mid(14,1);
          lineEdit_tenantCode.append(date);
          date = QDateTime::currentDateTime().toString().mid(0,1);
          lineEdit_tenantCode.append(date);
          date = QDateTime::currentDateTime().toString().mid(22,1);
          lineEdit_tenantCode.append(date);
          qDebug()<<"in tenant code is blank";
          qDebug()<<lineEdit_tenantCode;

    }//end if(ui->lineEdit_tenantCode->text()=="") //if the tenant code has been overridden, it is already saved   

    //check that the tenant code is unique
    qry1.prepare("select * from tenant where tenant_code = '"+lineEdit_tenantCode+"'");
    if(qry1.exec())
    {
        int count = 0;
        while(qry1.next())
        {
           count++;
        }
        //if that code is already taken
        if (count >= 1)
        {
            QMessageBox::critical(this, tr("Error"),tr("Error. That tenant code is taken. Override tenant code with another."));
        }
        //otherwise, input the data into the database
        else
        {
            qDebug()<<lineEdit_tenantCode;
            saveToDatabase();
        }//end input into database
     }//end if(qry1.exec())
}//end generateTenantCode()

void AddNewTenant::saveToDatabase()
{
    Login conn;
    conn.connOpen();
    QSqlQuery qry2;
    //save the text inputs

    qry2.prepare("insert into tenant(tenant_code, tenant_fn, tenant_ln, tenant_email, tenant_phone, tenant_notes) values (:lineEdit_tenantCode, :lineEdit_tenantFN, :lineEdit_tenantLN, :lineEdit_tenantEmail, :lineEdit_tenantPhone, :lineEdit_notesTenant)");
    qry2.bindValue(":lineEdit_tenantCode", lineEdit_tenantCode);
    qry2.bindValue(":lineEdit_tenantFN", lineEdit_tenantFN);
    qry2.bindValue(":lineEdit_tenantLN", lineEdit_tenantLN);
    qry2.bindValue(":lineEdit_tenantEmail", lineEdit_tenantEmail);
    qry2.bindValue(":lineEdit_tenantPhone", lineEdit_tenantPhone);
    qry2.bindValue(":lineEdit_notesTenant", lineEdit_notesTenant);
    if(qry2.exec())
    {
        QMessageBox::information(this, tr("Save"), tr("Tenant information saved successfully."));
        conn.connClose();
    }//end if(qry2.exec())
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());
        conn.connClose();
    }//end else if(!(qry2.exec()))
}//end saveToDatabase()




//a function that must remain or else the code is broken for some unknown reason
void AddNewTenant::on_pushSaveRentalDetails_clicked()
{
}
