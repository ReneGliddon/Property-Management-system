#include "rentalview.h"
#include "ui_rentalview.h"
#include "rentalview2.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

rentalView::rentalView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rentalView)
{
    ui->setupUi(this);

    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    //populate properties
    qry1.prepare("select nickname from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            ui->comboProp->addItem(qry1.value(0).toString());
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }

    //set the property value
    property = ui->comboProp->currentText();

    //close the connection
    connClose();

}//end constructor

rentalView::~rentalView()
{
    delete ui;
}

void rentalView::on_comboProp_currentTextChanged(const QString &arg1)
{

    //reset the drop down menu
    ui->comboTenant->clear();
    ui->comboOccupancy->clear();

    //reset the name to blank
    QString name = "";
    code = "";

    //set the property
    property = arg1;

    //open the database connection
    connOpen();

    //declare the queries
    QSqlQuery qry;

    //declare the variables
    QString start, end;

    //get the different tenant codes
    qry.prepare("select distinct tenant_code from rental where nickname = :choice");
    qry.bindValue(":choice", property);
    if(qry.exec())
    {
        while(qry.next())
        {
           //store the data from retrieval
           code = qry.value(0).toString();
           //start = qry.value(1).toString();
           //end = qry.value(2).toString();


           //reopen the connection fully because otherwise it doesn't work for some reason
           mydb = QSqlDatabase::addDatabase("QSQLITE");
           mydb.setDatabaseName("C:/QT/Qt5.3.0/Tools/QtCreator/bin/LoginApp/database.db");
           if (!mydb.open())
           {
                  qDebug()<<("Failed to open the database");
           }
           else
           {
                qDebug()<<("Connected...");
           }

           //get the names of the tenants
           QSqlQuery qry1;
           qry1.prepare("select tenant_fn, tenant_ln from tenant where tenant_code = :code");
           qry1.bindValue(":code", code);
           if(qry1.exec())
           {
              while(qry1.next())
              {
                 //add the first and last names, and the unique code and display it on the drop down
                 //simultaneously save the names to the variables for use in on_pushButton_clicked()
                 name = qry1.value(0).toString();
                 fn = name;
                 name = name.append(" ");
                 name = name.append(qry1.value(1).toString());
                 ln = qry1.value(1).toString();
                 name =  name.append(" (");
                 name = name.append(code);
                 name = name.append(")");
                 ui->comboTenant->addItem(name);
               }//end while(qry1.next())
            }//end if(qry1.exec())
           else
           {
               QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
               connClose();
           }
       }//end while(qry.next())

       //note to user that no tenants are listed at this property is this is the case
       if (name == "")
       {
           name = "No tenants listed";
           QString occupancy = "No contracts listed";
           ui->comboTenant->addItem(name);
           ui->comboOccupancy->addItem(occupancy);
           fn = "N/A";
           ln = "N/A";
           code = "N/A";
       }

       connClose();
   }//end if(qry.exec())
   else
   {
       QMessageBox::critical(this, tr("Error: "), qry.lastError().text());
       connClose();
   }//end else if !qry.exec()

    //close the connection
    connClose();

}//end on_comboProperty_currentTextChanged

void rentalView::on_comboTenant_currentTextChanged(const QString &arg1)
{
    //reset the values
    occupancy = "";
    ui->comboOccupancy->clear();

    //extract the tenant code
    code = arg1;
    int pos= code.indexOf("(");
    pos = pos +1;
    int pos2 = code.indexOf(")");
    pos2=pos2-pos;
    code = code.mid(pos,pos2);

    //get the property name
    //property = ui->comboProp->currentText();

    //get the rental contract details
    connOpen();
    QSqlQuery qry;
    QString start, end, occupancyString;
    qry.prepare("select occupancy_code, dateStart, dateEnd from rental where nickname = :property and tenant_code = :code");
    qry.bindValue(":property", property);
    qry.bindValue(":code", code);
    if(qry.exec())
    {
        while(qry.next())
        {
            //create a string for the occupancy details
            occupancy = qry.value(0).toString();
            qDebug()<<"our occupancy code is "<<occupancy;
            start = qry.value(1).toString();
            end = qry.value(2).toString();

            occupancyString = occupancy;
            occupancyString = occupancyString.append(": from ");
            occupancyString = occupancyString.append(start);
            occupancyString = occupancyString.append(" to ");
            occupancyString = occupancyString.append(end);

            //display the name and occupancy details
            ui->comboOccupancy->addItem(occupancyString);
       }//end while
    }//end if(qry.exec())
}//end on_comboTenant_currentTextChanged

void rentalView::on_pushButton_clicked()
{

    //display the details of the tenant and property selected in a separate dialog, rentalView2

    //declare in instance of rentalView2
    rentalView2 displayRental;

    //set the new values

    qDebug()<<"the values at the moment are fn: "<<fn<<" ln: "<<ln<<" property: "<<property<<" tenant: "<<code<<" occupancy: "<<occupancy;
    QString currentName = ui->comboTenant->currentText();
    int pos2 = currentName.indexOf(" ");
    int pos = 0;
    QString first = currentName.mid(pos, pos2);
    fn = first;
    if(fn == "no")
    {
        fn = "N/A";
    }

    //ln
    pos = pos2 + 1;
    pos2 = currentName.indexOf("(");
    int pos3 = pos2 - pos - 1;
    QString last = currentName.mid(pos, pos3);
    ln = last;
    if(ln == "tenants listed")
    {
        ln = "N/A";
    }

    //code
    pos = pos2 + 1;
    pos2 = currentName.indexOf(")");
    pos3 = pos2 - pos;
    QString tenantCode = currentName.mid(pos, pos3);
    code = tenantCode;

    //property
    property = ui->comboProp->currentText();

    //occupancy
   occupancy = ui->comboOccupancy->currentText();
   pos2 = occupancy.indexOf(":");
   occupancy = occupancy.mid(0,pos2);
   qDebug()<<"Occupancy code is "<<occupancy;

qDebug()<<"the recaptureed  are fn: "<<fn<<" ln: "<<ln<<" property: "<<property<<" tenant: "<<code<<" occupancy: "<<occupancy;

    //set the names and code in rentalView2
    displayRental.setValues(fn, ln, code, property, occupancy);

    //display the details
    displayRental.display();
}


