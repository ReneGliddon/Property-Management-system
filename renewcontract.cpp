#include "renewcontract.h"
#include "ui_renewcontract.h"
#include <QDate>
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
RenewContract::RenewContract(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenewContract)
{
    ui->setupUi(this);

    // set the current date as default rental start date.
    QDate date = QDate::currentDate();
    ui->dateStart->setDate(date);

    QDate dateEnd = date.addYears(1);
    ui->dateEnd->setDate(dateEnd);

}

RenewContract::~RenewContract()
{
    delete ui;
}

void RenewContract::display(QString propertyName, QString tenant, QString occupancy)
{
    //set the values
    property = propertyName;
    tenant_code = tenant;
    occupancy_code = occupancy;

    //open the database connection
    connOpen();

    //declare the queries
    QSqlQuery qry;

    //declare the variables
    QString sewage, utility;

    qry.prepare("select rent, chargeUtilityTo, chargeSewageTo from rental where occupancy_code = :code");
    qry.bindValue(":code", occupancy_code);
    if(qry.exec())
    {
        while(qry.next())
        {
           //increase the rent by a suggested 1.5%
           int rent = qry.value(0).toInt();
           rent = rent*1.015;
           ui->spinBoxRent->setValue(rent);

           //set the charges as they were before
           utility = qry.value(1).toString();
           qDebug()<<"before, utility is set to "<<utility;
           sewage = qry.value(2).toString();
           qDebug()<<"Before, sewage is set to "<<sewage;

           if(utility == "Tenant")
           {
                ui->radioUtilitiesTenant->setChecked(1);
           }
           else
           {
               ui->radioUtilitiesOwner->setChecked(1);
           }

           if(sewage == "Tenant")
           {
               ui->radioSewageTenant->setChecked(1);
           }
           else
           {
               ui->radioSewageOwner->setChecked(1);
           }
        }
        connClose();
    }

    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }


    //populate the display
    ui->label_nickname->setText(propertyName);



    this->exec();
}

void RenewContract::on_pushRenew_clicked()
{
    //reset the dates
    date_start = ui->dateStart->date();
    date_end = ui->dateEnd->date();

    //generate a new occupancy code
    bool valid = generateOccupancyCode();

    if(valid)
    {


        //insert into database
        insertValues();
    }

}

bool RenewContract::generateOccupancyCode()
{
       bool occupancy;

       //clear the occupancy code
       newOccupancyCode = "";

       //create a variable to hold the test value
       QString testOccupancyCode;

      //if occupancy code isn't overidden, generate one.
       if(ui->lineEdit_occupancyCode->text()=="")
       {
           //first create a test occupancy code
           testOccupancyCode = occupancy_code.mid(0,4);

           QString lnAppend = date_end.toString();
           int pos = lnAppend.indexOf(" ");
           pos = pos+1;
           int pos2 = lnAppend.length();
           pos2 = pos2 - pos;
           lnAppend = lnAppend.mid(pos, pos2);
           pos = lnAppend.indexOf(" ");
           pos = pos+1;
           pos2 = lnAppend.length();
           pos2 = pos2 - pos;
           lnAppend = lnAppend.mid(pos,pos2);
           pos = lnAppend.indexOf(" ");
           pos = pos+1;
           pos2 = lnAppend.length();
           pos2 = pos2 - pos;
           lnAppend = lnAppend.mid(pos,pos2);
           testOccupancyCode.append(lnAppend);

           //check it is unique
            occupancy = testOccupancy(testOccupancyCode);

            //if it is unique
            if (occupancy == 1)
            {
                //set the occupancy code
                newOccupancyCode = testOccupancyCode;
                return 1;
            }
            else
            {
                  return 0;
            }
        }//end if occupancy code isn't overidden

        //if the code is overridden, use that value
      else
      {
            testOccupancyCode = ui->lineEdit_occupancyCode->text();

            //check that is is unique
            occupancy = testOccupancy(testOccupancyCode);

             //if it is unique
               if (occupancy ==1)
              {
                  //set the value and return a true bool
                   newOccupancyCode = testOccupancyCode;
                   return 1;
              }
              else
              {
                   return 0;
              }
      }// end else if the code is overridden, use that value
}//end generateOccupancy

bool RenewContract::testOccupancy(QString testCode)
{
    //open the connection
        connOpen();
        QSqlQuery qry1;

   //check if this occupancy code exists already
       qry1.prepare("select * from rental where occupancy_code = '"+testCode+"'");
       if(qry1.exec())
       {
           int count = 0;
           while(qry1.next())
           {
                count++;
           }
           //if there is already a record with the autogenerated code then show an error and close the connection
           if(count>=1)
           {
               QMessageBox::critical(this, tr("Error"),tr("Error. Duplicate record for the occupancy code. Please override with unique occupancy code."));
               connClose();
               return 0;
           }
           //if there is no record with that code, send a 1 back to indicate this
           else
           {
               connClose();
               return 1;
           }
       }//end if(qry1.exec())
       else
       {
           QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
           connClose();
           return 0;
       }
}

void RenewContract:: insertValues()
{
       //open the connection
       connOpen();

        //declare and set variables
        QSqlQuery qry1, qry2;
        QString current = "Y";
        double spinRent = ui->spinBoxRent->value();

        QString sewage, utility, notes;

        //set values for sewage and utilities

        //utilities
        if(ui->radioUtilitiesOwner->isChecked())
        {
            utility = "Owner";
        }
        else if(ui->radioUtilitiesTenant->isChecked())
        {
            utility = "Tenant";
        }
        else
        {
            utility = "null";
        }
        //sewage
        if(ui->radioSewageOwner->isChecked())
        {
            sewage = "Owner";
        }
        else if(ui->radioSewageTenant->isChecked())
        {
            sewage = "Tenant";
        }
        else
        {
            sewage = "null";
        }

        //set the notes
        notes = ui->lineEdit_notesRental->text();

        //next, input the values into the database
        qry1.prepare("insert into rental(occupancy_code, nickname, tenant_code, rent, chargeUtilityTo, chargeSewageTo, notes, current_tenant, dateStart, dateEnd) values (:newOccupancyCode, :nickname, :tenant_code, :spinRent, :radio_utility, :radio_sewage, :lineEdit_notesRental, :radio_current, :dateStart, :dateEnd)");
        qry1.bindValue(":newOccupancyCode", newOccupancyCode);
        qry1.bindValue(":nickname", property);
        qry1.bindValue(":tenant_code", tenant_code);
        qry1.bindValue(":spinRent", spinRent);
        qry1.bindValue(":radio_utility", utility);
        qry1.bindValue(":radio_sewage", sewage);
        qry1.bindValue(":lineEdit_notesRental", notes);
        qry1.bindValue(":radio_current", current);
        qry1.bindValue(":dateStart", date_start);
        qry1.bindValue(":dateEnd", date_end);

         //this tenant is current, therefore no other tenant is current at this address

        //let the user know they database will be updated to reflect
        QMessageBox::information(this, tr("Renew"), tr("Renewing this contract with the listed tenant as the sole current tenant."));
        qry2.prepare(("update rental set current_tenant = :notCurrent where (occupancy_code != :newOccupancyCode AND nickname == :nickname AND current_tenant ==:current)"));
        qry2.bindValue(":notCurrent", "N");
        qry2.bindValue(":newOccupancyCode", newOccupancyCode);
        qry2.bindValue(":nickname", property);
        qry2.bindValue(":current", "Y");
        if(!(qry2.exec()))
        {
           QMessageBox::critical(this, tr("Error"), qry2.lastError().text());
           connClose();
        }

          //message box to show that the information was saved
          if(qry1.exec())
          {
             QMessageBox::information(this, tr("Save"), tr("Rental contract renewed successfully."));
             connClose();
             this->close();
          }
          else
          {
              QMessageBox::critical(this, tr("Error"), qry1.lastError().text());
              connClose();
          }

}//end insertValues()
