#include "addnewrental.h"
#include "ui_addnewrental.h"
//#include "login.h"
#include "tenantlist.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QTextDocument>

AddNewRental::AddNewRental(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewRental)
{
    ui->setupUi(this);

    // set the current date as default rental start date.
    defaultStart = QDate::currentDate();
    defaultEnd = defaultStart.addYears(1);
    ui->dateStart->setDate(defaultStart);
    ui->dateEnd->setDate(defaultEnd);


    connOpen();
    QSqlQuery qry1,qry2;

    qry2.prepare("select * from properties");
    if(qry2.exec())
    {
        while(qry2.next())
        {
                ui->comboProperty->addItem(qry2.value(0).toString());
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry2.lastError().text());
        connClose();
    }
}

AddNewRental::~AddNewRental()
{
    delete ui;
}

void AddNewRental::on_pushSaveRentalDetails_clicked()
{
        //set values from the ui
         setValues();

         //if contract has been terminated (tenant is not current), check that end date has been entered
         if(radio_current == "N" && date_end.toString("YYYY-MM-DD") == defaultEnd.toString())
         {
              QMessageBox::critical(this, tr("Error"),tr("Please enter the end date for this terminated rental contract or list tenant as 'current'."));
         }

         //if mandatory fields are not blank, then input into the database
          if (!(radio_current=="X"))
         {
              //Generate an occupancy code
               int j = generateOccupancy();

             //if the occupancy code is unique, input the values into the database
              if(j==1)
             {
                inputValues();
             }

        }//end if no blanks
        //if the mandatory fields were left blank, show error message
        else
        {
          QMessageBox::critical(this, tr("Error"),tr("Error. Tenant's current status field cannot be blank."));
        }//end if fields were left blank

}//end on_pushSaveRentalDetails_clicked()


 void AddNewRental::setValues()
 {

     //save mandatory values
     //rent
     spinRent = ui->spinBoxRent->value();
     //save date values
     date_start = ui->dateStart->date();
     date_end = ui->dateEnd->date();

     //save optional values if present
     //notes
      lineEdit_notesRental = ui->lineEdit_notesRental->text();

     //set values for sewage and utilities
     //utilities
     if(ui->radioUtilitiesOwner->isChecked())
     {
         radio_utility = "Owner";
     }
     else if(ui->radioUtilitiesTenant->isChecked())
     {
         radio_utility = "Tenant";
     }
     else
     {
      radio_utility = "null";
     }
     //sewage
     if(ui->radioSewageOwner->isChecked())
     {
         radio_sewage = "Owner";
     }
     else if(ui->radioSewageTenant->isChecked())
     {
         radio_sewage = "Tenant";
     }
     else
     {
         radio_sewage = "null";
     }
     //current tenant
     if(ui->radioCurrentTenant->isChecked())
     {
         radio_current = "Y";
     }
     else if(ui->radioNotCurrent->isChecked())
     {
         radio_current = "N";
     }
     else
     {
         radio_current = "X";
     }

 }//end setValues()


 int AddNewRental::generateOccupancy()
 {
      int occupancy;

     //if occupancy code isn't overidden, generate one.
      if(ui->lineEdit_occupancyCode->text()=="")
      {
            //first create a test occupancy code
             QString testOccupancyCode = ui->comboProperty->currentText().mid(1,3);
             QString lnAppend = tenant_code.mid(1,3);
             testOccupancyCode.append(lnAppend);
             lnAppend = date_start.toString("yyyy-MM-dd").mid(2,2);
             testOccupancyCode.append(lnAppend);
             lineEdit_occupancyCode = testOccupancyCode;

            //check it is unique
           occupancy = testOccupancy();

           //if it is unique
             if (occupancy == 1)
             {
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
              lineEdit_occupancyCode= ui->lineEdit_occupancyCode->text();
            //check that is is unique
                 occupancy = testOccupancy();
            //if it is unique
                 if (occupancy ==1)
                {
                    return 1;
                }
                else
                {
                  return 0;
                }
        }// end else if the code is overridden, use that value

 }//end generateOccupancy()


 int AddNewRental::testOccupancy()
 {
     //open the connection
         connOpen();
         QSqlQuery qry1;

    //check if this occupancy code exists already
        qry1.prepare("select * from rental where occupancy_code = '"+lineEdit_occupancyCode+"'");
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
                QMessageBox::critical(this, tr("Error"),tr("Error. Duplicate record for this property and tenant last name. Please override with unique code if this record is still required."));
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
            QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
            connClose();
            return 0;
        }
 }//end testOccupancy()

 void AddNewRental::inputValues()
 {
     //open the connection
        connOpen();

     //declare variables
     QSqlQuery qry1, qry2;
     QString current = "Y";
     QString lineEdit_nickname = ui->comboProperty->currentText();

     //next, input the values into the database
     qry1.prepare("insert into rental(occupancy_code, nickname, tenant_code, rent, chargeUtilityTo, chargeSewageTo, notes, current_tenant, dateStart, dateEnd) values (:lineEdit_occupancyCode, :lineEdit_nickname, :tenant_code, :spinRent, :radio_utility, :radio_sewage, :lineEdit_notesRental, :radio_current, :dateStart, :dateEnd)");
     qry1.bindValue(":lineEdit_occupancyCode", lineEdit_occupancyCode);
     qry1.bindValue(":lineEdit_nickname", lineEdit_nickname);
     qry1.bindValue(":tenant_code", tenant_code);
     qry1.bindValue(":spinRent", spinRent);
     qry1.bindValue(":radio_utility", radio_utility);
     qry1.bindValue(":radio_sewage", radio_sewage);
     qry1.bindValue(":lineEdit_notesRental", lineEdit_notesRental);
     qry1.bindValue(":radio_current", radio_current);
     qry1.bindValue(":dateStart", date_start);
     qry1.bindValue(":dateEnd", date_end);

      //if this tenant is current, no other tenant is current at this address
       if(radio_current == current)
       {
           //let the user know they database will be updated to reflect
            QMessageBox::information(this, tr("Update"), tr("Updating this tenant as the sole current tenant."));
            qry2.prepare(("update rental set current_tenant = :notCurrent where (occupancy_code != :lineEdit_occupancyCode AND nickname == :lineEdit_nickname AND current_tenant ==:current)"));
            qry2.bindValue(":notCurrent", "N");
            qry2.bindValue(":lineEdit_occupancyCode", lineEdit_occupancyCode);
            qry2.bindValue(":lineEdit_nickname", lineEdit_nickname);
            qry2.bindValue(":current", "Y");
            if(!(qry2.exec()))
            {
                 QMessageBox::critical(this, tr("Error"), qry2.lastError().text());
                 connClose();
            }
       }
       //message box to show that the information was saved
       if(qry1.exec())
       {
          QMessageBox::information(this, tr("Save"), tr("Rental information saved successfully."));
          connClose();
          this->close();
       }
       else
       {
           QMessageBox::critical(this, tr("Error"), qry1.lastError().text());
           connClose();
       }
}//end inputValues()

 void AddNewRental::setTenantCode(QString code)
 {
     tenant_code = code;

     qDebug()<<"The tenant code os "<<code;
 }

