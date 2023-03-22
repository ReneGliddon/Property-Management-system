#include "viewproperty2.h"
#include "ui_viewproperty2.h"
#include <QDebug>
#include <QDate>

ViewProperty2::ViewProperty2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewProperty2)
{
    ui->setupUi(this);
}

ViewProperty2::~ViewProperty2()
{
    delete ui;
}

void ViewProperty2::setProperty(QString selection)
{
    //set the property to be viewed as the one chosen by ViewProperty
    property = selection;
}

void ViewProperty2::display()
{
    connOpen();
    //fill the fields of the dialog for display

    //fill the property fields
    fillProperty();

    //fill the current tenant details
    fillTenant();

    //fill in the financial details
    fillFinances();

    //close connection
    connClose();

    //show the display
    this->exec();
}

void ViewProperty2::fillProperty()
{
    qDebug()<<"We are flling the property";
    //open the connection
    connOpen();

    QSqlQuery qry;
    ui->viewNickname->setText(property);
    qry.prepare("select * from properties where nickname = :property");
    qry.bindValue(":property", property);
    if(qry.exec())
    {
        while(qry.next())
       {
            ui->viewAddress->setText(qry.value(1).toString());
            purchaseCost = qry.value(2).toDouble();
            QString s1 = QString::number(purchaseCost, 'g', 10);
            ui->viewPurchaseCost->setText(s1);
            QString date = (qry.value(3).toString());

            //make the date look better
            QString year, month, day;
            //get the values
            year = date.mid(0,4);
            month =date.mid(5,2);
            day = date.mid(8,2);

            //day may be one or two digits
            QString dayTest = day.mid(0,1);
            if(dayTest=="0")
            {
                day = day.mid(1,1);
            }

            //name for the month
            if(month =="01")
                month="Jan";
            else if(month == "02")
                month = "Feb";
            else if(month == "03")
                month = "Mar";
            else if(month == "04")
                month = "Apr";
            else if(month == "05")
                month = "May";
            else if(month == "06")
                month = "Jun";
            else if(month == "07")
                month = "Jul";
            else if(month == "08")
                month = "Aug";
            else if(month == "09")
                month = "Sept";
            else if(month == "10")
                month = "Oct";
            else if(month == "11")
                month = "Nov";
            else if(month == "12")
                month = "Dec";

            //put the date together
            date = day;
            date.append(" ");
            date.append(month);
            date.append(" ");
            date.append(year);

            //set this date to the label
            ui->viewPurchaseDate->setText(date);

            //set other labels
            ui->viewMonthlyMortgage->setNum(qry.value(4).toDouble());
            ui->viewMortgageTerm->setNum(qry.value(5).toInt());
            ui->viewHOA->setNum(qry.value(6).toDouble());
            HOA = qry.value(6).toDouble();
            ui->viewSewageCompany->setText(qry.value(7).toString());
            ui->viewUtility->setText(qry.value(8).toString());
            ui->viewNotes->setText(qry.value(9).toString());

            //close the connection
            connClose();

       }//end while(qry1.next())

    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }//end else if !qry1.exec()
}

void ViewProperty2::fillTenant()
{

    qDebug()<<"We are flling the tenant";
    //open the connection
    connOpen();

    //set the default bools to 0
    sewage = 0;
    utilities = 0;

    QSqlQuery qry, qry1;
    qry.prepare("select tenant_code, rent, chargeUtilityTo, chargeSewageTo, dateStart, dateEnd from rental where nickname = :property and current_tenant = :yes");
    qry.bindValue(":property", property);
    qry.bindValue(":yes", "Y");
    if(qry.exec())
    {
        while(qry.next())
       {
            QString tenantCode = qry.value(0).toString();
            ui->viewTenantRent->setText(qry.value(1).toString());
            rent = qry.value(1).toDouble();
            QString whoPays = qry.value(2).toString();
            ui->viewUtilityBill->setText(whoPays);
            if(whoPays == "Owner")
            {
                utilities = 1;
            }
            whoPays = qry.value(3).toString();
            ui->viewSewageBill->setText(whoPays);
            if(whoPays == "Owner")
            {
                sewage = 1;
            }

            startDate = qry.value(4).toDate();
            endDate = qry.value(5).toDate();
            //endDate = startDate.addYears(1);

            qry1.prepare("select * from tenant where tenant_code = :tenantCode");
            qry1.bindValue(":tenantCode", tenantCode);
            if(qry1.exec())
            {
                while(qry1.next())
                {
                    ui->viewTenantFN->setText(qry1.value(1).toString());
                    ui->viewTenantLN->setText(qry1.value(2).toString());
                    ui->viewTenantEmail->setText(qry1.value(3).toString());
                    ui->viewTenantPhone->setText(qry1.value(4).toString());

                }

                //close the connection
                connClose();
            }
            else
            {
                QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
                connClose();
            }//end else if !qry1.exec()
       }//end while(qry1.next())

    }

    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }//end else if !qry1.exec()
}

void ViewProperty2::fillFinances()
{

    qDebug()<<"We are flling the finances";
    //open the connection
    connOpen();

    //declare variables
    QSqlQuery qry, qry1;
    double totalIncome = 0.00;
    double totalExpense = 0.00;
    double currentCost = 0.00;
    bool purchase = 0;


    //prepare the income query
    qry.prepare("select income_amount from income where property_nickname = :property");
    qry.bindValue(":property", property);
    if(qry.exec())
    {
        while(qry.next())
       {
            totalIncome = totalIncome + qry.value(0).toDouble();
       }//end while (qry.next())
    }//end if(qry.exec())
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }//end else if !qry1.exec()


    //prepare the expense query

     double sewageAmount = 0.00;
     int sewageCount = 0;
     QString searchSewage = "SEWAGE";

     double utilitiesAmount = 0.00;
     int utilitiesCount = 0;
     QString searchUtilities = "UTILITIES";


    qry1.prepare("select expense_amount, expense_description from expense where property_nickname = :property");
    qry1.bindValue(":property", property);
    if(qry1.exec())
    {
        while(qry1.next())
       {
            currentCost = qry1.value(0).toDouble();
            totalExpense = totalExpense + currentCost;

            //create a string to hold the description
            QString descript = qry1.value(1).toString();

            //if we need to pay for sewge, then calculate how much this could be on average per month
            if(sewage)
            {
                if(descript.toUpper().contains(searchSewage))
                {
                    sewageAmount = sewageAmount + currentCost;
                    sewageCount++;
                }
            }

            //if we need to pay for utilties, then calculate how much this could be on average per month
            if(utilities)
            {
                if(descript.toUpper().contains(searchUtilities))
                {
                    utilitiesAmount = utilitiesAmount + currentCost;
                    utilitiesCount++;
                }
            }
            //check if the expense of the house/unit is included in the expenses
            if(currentCost == purchaseCost)
            {
                if(descript.toUpper().contains("INITIAL PURCHASE"))
                {
                    purchase = 1;
                }
            }

       }//end while (qry1.next())
    }//end if(qry1.exec())
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }//end else if !qry1.exec()

    qDebug()<<"We are calculating total cost";

    //if the unit expense wasn't found in the expenses, add it in
    if(!purchase)
    {
        totalExpense = totalExpense + purchaseCost;
    }

    //check if the utilities and sewage are to be paid by the owner and if so, then calculate the average per month to add it
    if (sewage)
    {

       if(sewageCount>0)
       {
            sewageAmount = sewageAmount/sewageCount;
       }

    }

    if(utilities)
    {
        if(utilitiesCount>0)
        {
            utilitiesAmount = utilitiesAmount/utilitiesCount;
        }
    }

    qDebug()<<"We are calculating expense and income";

    //how much more expense do we have than income
    double outstandingExpense = totalExpense - totalIncome;

    //how much do we make per month
    double netRent = rent - HOA - sewageAmount - utilitiesAmount;

    //
    double monthsToPay = 0;
    double stillOutstanding = outstandingExpense;
    double cumulativeNetRentToDate = netRent;
    bool firstContract = true;


    //if there is no contract, pretend that today is the start date of the contract to give an estimated time of breaking even
  //  if(startDate == "" )
  //  {
        //startDate = QDate::currentDate();
    //}

    //how many months are left in this contract?
    qDebug()<<"We are counting months";

    //first set the counter of months to zero
    int monthsBetween = 0;

    //then check that the contract hasn't technically expired, if it has, allow for 6 months more at this current rental rate
    qDebug()<<"We are checking expiry date of contract";
    qDebug()<<"Our end date is "<<endDate;
    if(endDate < QDate::currentDate())
    {
        qDebug()<<"Our contract has expired so we are doing an estimate";
        endDate = QDate::currentDate();
        endDate.addMonths(6);
        monthsBetween = 6;
        QMessageBox::warning(this, tr("Warning: "),"Contract has expired. Calculating the break-even point allowing for 6 more months  of current rent before assuming an annual increase.");

    }
    else
    {
        qDebug()<<"our contract has not expired so we are counting months";
        int currentDay = QDate::currentDate().day();
        int endDay = endDate.day();

        int currentMonth = QDate::currentDate().month();
        int endMonth = endDate.month();

        int currentYear = QDate::currentDate().year();
        int endYear = endDate.year();

        // add months of the contract that we will receive rent at this rate

        //if we have the rest of the year to calculate
        if(endYear > currentYear)
        {
            for(int i = currentMonth; i<=12; i++)
            {
                monthsBetween++;
            }
            currentYear++;
        }

        //now if we have another full year or more
        while(endYear>currentYear)
        {

           monthsBetween = monthsBetween*12;

           currentYear++;
        }

        //finally if we have a few extra months
        if(endYear==currentYear)
        {
            for(int i = 1; i < endMonth; i++)
            {
                monthsBetween++;
            }
        }

    }

     qDebug()<<"the start date is "<<startDate;
     //qDebug()<<"the start month is "<<month;
     qDebug()<<"The number of months to end of this contract are "<<monthsBetween;

    while(stillOutstanding > cumulativeNetRentToDate)
    {

        //if we are calculating within the first contract period
        if(firstContract)
        {
            /*if we will not break even in this contract period, calculate how much we will make,
             * how many months we have received income for, and set the bool to false*/
           if((netRent * monthsBetween) < outstandingExpense)
           {
               cumulativeNetRentToDate = cumulativeNetRentToDate + netRent*monthsBetween;
               firstContract = false;
               monthsToPay = monthsToPay+monthsBetween;
           }
           //else if we will break even during this contract period, calulcate how many months it will take for us to do this
           else
           {
               do
               {
                   cumulativeNetRentToDate = cumulativeNetRentToDate + netRent;
                   monthsToPay++;

               }
               while(stillOutstanding > cumulativeNetRentToDate);
               firstContract = false;
           }
        }
        //else if we are now checking after the first contract ends

        else
        {
            //increase rent
            netRent = netRent*1.01;

            //if after a full year of this current rent, the cost is still not covered,
            //add the full year of rent to the running total
            if((netRent*12) < outstandingExpense)
            {
                //increase the months by 12
                monthsToPay = monthsToPay  + 12;

                //add a year of rent to the running total
                cumulativeNetRentToDate = cumulativeNetRentToDate + (netRent*12);

            }
            //else if we have exceeded the outstanding expense by the end of a year
            else if(netRent*12 > outstandingExpense)
            {
                //add in rent one month at a time;
                while(cumulativeNetRentToDate < outstandingExpense)
                {
                    monthsToPay++;
                    cumulativeNetRentToDate = cumulativeNetRentToDate + netRent;
                }
            }
            //else if we have made the cost exactly then we break out of this loop

        }//end else if we are now checking after the first contract ends
    }//end   while(stillOutstanding > cumulativeNetRentToDate

    //add those months to todays date
    QDate breakEven = QDate::currentDate().addMonths(monthsToPay);

    //display this date in the ui
    ui->viewBreakEven->setText(breakEven.toString());


}//end void fillFinances()
