#include "addnewincome.h"
#include "ui_addnewincome.h"
#include "overrideincome.h"
#include "dialog.h"
#include <QMessageBox>
#include "overrideincome.h"
#include <QtSql>
#include <QDebug>
#include <QDate>

AddNewIncome::AddNewIncome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewIncome)
{
    ui->setupUi(this);

    //check the connection is open
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1, qry2;

    //fill properties
    qry1.prepare("select nickname from properties");
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


    // set the current date as default date.
    QDate date = QDate::currentDate();
    ui->dateIncome->setDate(date);


    //place the most common income descriptions in a drop down
    ui->comboDescription->addItem("Rent");
    ui->comboDescription->addItem("Late Fee");
    ui->comboDescription->addItem("Other");
}

AddNewIncome::~AddNewIncome()
{
    delete ui;
}



int AddNewIncome::testIncome()
{
    //open the database
    connOpen();
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    //see if there is already a record of this code in the database
    QSqlQuery qry;
    qry.prepare("select * from income where income_code = :incomeCode");
    qry.bindValue(":incomeCode", incomeCode);
    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        //code already exists in database
        if(count>0)
        {
            connClose();
            return 0;
        }
        //code is unique
        else
        {
            connClose();
            return 1;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
        return 0;
    }
}

void AddNewIncome::on_pushAddIncome_2_clicked()
{
    //save the text inputs
    description=ui->comboDescription->currentText();


    //if we have an unlisted description, ask for input
    if(description == "Other")
    {
         Dialog descriptionSet;
         descriptionSet.exec();
         // qDebug()<<"the descript is now "<<description;
    }
    else
    {
        qDebug()<<description<<" "<<propertyNickname<<" "<<payerName;
        pushCont();
    }

}
void AddNewIncome::pushCont()
{
 qDebug()<<"the descript is now "<<description;
 //reset  the values
 propertyNickname=ui->comboProperty->currentText();
 payerName = ui->comboReceived->currentText();
qDebug()<<description<<" "<<propertyNickname<<" "<<payerName;
    //format the payer
    if(payerName != "Other")
    {
        int pos = payerName.indexOf(":");
        pos = pos + 2;
        int pos2 = payerName.length();
        pos2 = pos2 - pos - 1;
        payerName = payerName.mid(pos, pos2);
        qDebug()<<"payer name is "<<payerName;
    }

    //number value
    incomeAmount = ui->doubleSpinIncome->value();

    //save income date
     incomeDate = ui->dateIncome->date();

    //generate an income code
    //first create a test income code
      incomeCode = propertyNickname.mid(1,2);
      QString lnAppend = incomeDate.toString("yyyy-MM-dd").mid(5,2);
      incomeCode.append(lnAppend);
      lnAppend = description.mid(0,2);
      incomeCode.append(lnAppend);

    //check it is unique
      int incomeCodeTest = testIncome();

      if(incomeCodeTest==1)
          inputValues();
      else
      {
        //expense code is not unique, prompt user to manually enter a unique code
          //create an object
          OverrideIncome override;

        //send and set the values from the ui
        override.setValues(description, propertyNickname, incomeAmount, incomeDate, payerName);

        //display the override request. values will be entered after a unique code is received
        override.exec();
      }
}

void AddNewIncome::inputValues()
{
    connOpen();
    QSqlQuery qry;
    qry.prepare("insert into income(income_code, property_nickname, income_amount, income_description, income_date, payer) values (:incomeCode, :propertyNickname, :incomeAmount, :lineEdit_description, :incomeDate, :payer)");
    qry.bindValue(":incomeCode", incomeCode);
    qry.bindValue(":propertyNickname", propertyNickname);
    qry.bindValue(":incomeAmount", incomeAmount);
    qry.bindValue(":lineEdit_description", description);
    qry.bindValue(":incomeDate", incomeDate);
    qry.bindValue(":payer", payerName);
    if(qry.exec())
    {
        QMessageBox::information(this, tr("Saved"), tr("Income saved successfully."));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }
}

void AddNewIncome::setIncomeCode(QString code, QString descriptionArg, double amount)
{
    //set code
     incomeCode = code;

     //reset property and amount
     description = descriptionArg;
     incomeAmount = amount;

     //test if this code is unique
     int test = testIncome();

     //if it is unique, input the values into the database
     if(test == 1)
     {
         //reset the other values
         //save the text inputs
         propertyNickname=ui->comboProperty->currentText();

         //save income date
         incomeDate = ui->dateIncome->date();

        //input these values
        inputValues();

        //close boxes
         this->close();

      }
      //else if overidden code is not unique
      else
      {
          //prompt user to enter a different code
            QMessageBox::critical(this, tr("Saved"), tr("Duplicate code. Please enter a different income code."));
      }
}

void AddNewIncome::on_comboProperty_currentTextChanged(const QString &arg1)
{
    //clear the old list
    ui->comboReceived->clear();

    //open the connection;
    connOpen();

    //fill possible payers

     QSqlQuery qry1, qry2;

    qry1.prepare("Select distinct tenant_code from rental where nickname = :property");
    qry1.bindValue(":property", arg1);
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString tenantCode = qry1.value(0).toString();
            qDebug()<<"we have "<<tenantCode;

            qry2.prepare("select tenant_fn, tenant_ln from tenant where tenant_code = :code");
            qry2.bindValue(":code", tenantCode);
            if(qry2.exec())
            {
                qDebug()<<"we are searching for "<<tenantCode<<"'s name ";
                while(qry2.next())
                {
                    qDebug()<<"we have found soemthing";
                    QString payer = qry2.value(0).toString();
                    qDebug()<<"payer is "<<payer;
                    payer = payer + " " + qry2.value(2).toString() + " (code: "+tenantCode+")";
                    qDebug()<<"payer is "<<payer;
                    ui->comboReceived->addItem(payer);
                }
            }
            else
            {
                QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());
                connClose();
            }

        }//end if(qry1.exec())


    }//end if(qry1.exec())

       ui->comboReceived->addItem("Other");

    connClose();
}//end on_comboProperty_currentTextChanged

void AddNewIncome::setDescription(QString descriptionArg)
{
    description = descriptionArg;
    qDebug()<<"the descript is now in set abd is "<<description;
    pushCont();
}
