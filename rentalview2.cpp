#include "rentalview2.h"
#include "ui_rentalview2.h"

rentalView2::rentalView2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rentalView2)
{
    ui->setupUi(this);
}

rentalView2::~rentalView2()
{
    delete ui;
}

void rentalView2::setValues(QString first, QString last, QString tenantCode, QString prop, QString occu)
{
    fn = first;
    ln = last;
    code = tenantCode;
    property = prop;
    occupancy = occu;
}

void rentalView2::display()
{
    //populate the display
    populate();
    this->exec();
}

void rentalView2::populate()
{

    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }


    //tenant details
    ui->viewTenantFNRental->setText(fn);
    ui->viewTenantLNRental->setText(ln);
    ui->viewTenantCodeRental->setText(code);
    ui->viewOccupancyCodeRental->setText(occupancy);

    //queries
    QSqlQuery qry, qry2, qry3, qry4, qry5;

    //variables
    QString blank;

    //add in the rental information
    qry.prepare("select * from rental where tenant_code = :code and occupancy_code = :occupancy");
    qry.bindValue(":code", code);
    qry.bindValue(":occupancy", occupancy);
    if(qry.exec())
    {
        while(qry.next())
        {
             ui->viewTenantRentRental->setText(qry.value(3).toString());
             ui->viewUtility->setText(qry.value(4).toString());
             ui->viewSewage->setText(qry.value(5).toString());

             //if there are any notes, add them to the notes string
             if(qry.value(6).toString() != "")
             {

                 ui->TenantAndProperty->setText("Contract Notes: ");
                 ui->TenantAndProperty->append(qry.value(6).toString());
             }

             ui->viewTenantCurrent->setText(qry.value(7).toString());
             begin = qry.value(8).toString();
             ui->viewFirst->setText(qry.value(8).toString());
             end = qry.value(9).toString();
             ui->viewEnd->setText(qry.value(9).toString());

        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());
        connClose();
    }

    //check that the contract has been correctly dated
    if(begin == end || begin < end)
    {
        QMessageBox::critical(this, tr("Error"), "The dates of the rental contract are incorrect. Please correct them to receive accurate information ");
    }

    //property details
    ui->viewNicknameRental->setText(property);

    qry2.prepare("select address, notes from properties where nickname = :property");
    qry2.bindValue(":property", property);
    if(qry2.exec())
    {
       while(qry2.next())
       {
         ui->viewInputAddressRental->setText(qry2.value(0).toString());

         if(qry2.value(1).toString()!= "")
         {
             ui->TenantAndProperty->append(blank);

            //add the heading to the display widget
            ui->TenantAndProperty->append("Property Notes: ");

            //add the notes to the display widget
            ui->TenantAndProperty->append(qry2.value(1).toString());

            //add a new line
            ui->TenantAndProperty->append(blank);
         }
       }
   }
   else
   {
        QMessageBox::critical(this, tr("Error: "), qry2.lastError().text());
        connClose();
   }

    //add tenant details
    qry3.prepare("select * from tenant where tenant_code = :code");
    qry3.bindValue(":code", code);
    if(qry3.exec())
    {
        while(qry3.next())
        {

           if(qry3.value(5)!= "")
           {
              //add the notes to the display widget
              ui->TenantAndProperty->append("Tenant Notes: ");

              //add the heading to the display widget
              ui->TenantAndProperty->append(qry3.value(5).toString());

           }
           else
           {
               ui->TenantAndProperty->append("Tenant Notes: ");
               ui->TenantAndProperty->append("No notes");
           }

        }//end while qry3.next
    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry3.lastError().text());
        connClose();
    }//end else if qry3 ! exec

    //add in the financial details

    //strings to be used
    QString expense, income;

    //ints to be used
    incomeDbl = 0.00;
    double expenseDbl = 0.00;
    rentReceived = 0.00;

    //set the title of the text box
    ui->textExpenses->setText("Expenditure ");


    //select the required values from expense
    qry4.prepare("select expense_amount, expense_description, expense_date from expense where property_nickname = :property");
    qry4.bindValue(":property", property);
    if(qry4.exec())
    {
        while(qry4.next())
        {
            //select only the expenses that occured during this occupancy
            if(qry4.value(2).toString()>= begin and qry4.value(2).toString() <= end)
            {
                //set a new line
                ui->textExpenses->append(blank);

                //create a string to store the line values of the expense
               expense = "Expense: " + qry4.value(1).toString();

               ui->textExpenses->append(expense);

               expense.clear();
               expense = "Amount: $" + qry4.value(0).toString();
               expenseDbl = expenseDbl + qry4.value(0).toDouble();
               ui->textExpenses->append(expense);

               expense.clear();
               expense = "Date:" + qry4.value(2).toString();
               ui->textExpenses->append(expense);
            }

        }//end while qry4.next
    }//end if qry4.exec
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry4.lastError().text());
        connClose();
    }//end else if qry4 ! exec

    //print out the total expense
    expense.clear();
    ui->textExpenses->append(blank);
    QString totalExpenses = QString::number(expenseDbl);
    expense = "Total Expenditure: " + totalExpenses;
    ui->textExpenses->append(expense);

    //incomes

    //set the title
    ui->textIncome->setText("Income");

    //select the required values from income
    qry5.prepare("select income_amount, income_description, income_date from income where property_nickname = :property");
    qry5.bindValue(":property", property);
    if(qry5.exec())
    {
        while(qry5.next())
        {
            //select only the incomes that occured during this occupancy
            if(qry5.value(2).toString()>= begin and qry5.value(2).toString() <= end)
            {
                //set a new line
                ui->textIncome->append(blank);

                //create a string to hold the description
                QString descript = qry5.value(1).toString();
                QString rentSearch = "RENT";

                //create a string to store the line values of the expense
               income = "Income: " + qry5.value(1).toString();
               ui->textIncome->append(income);


               bool rentFound = descript.toUpper().contains(rentSearch);
               if(rentFound)
               {
                   rentReceived = rentReceived + qry5.value(0).toDouble();
                   qDebug()<<"found some rent! We now have $"<<rentReceived;
               }

               income.clear();
               income = "Amount: $" + qry5.value(0).toString();
               incomeDbl = incomeDbl + qry5.value(0).toDouble();
               ui->textIncome->append(income);

               income.clear();
               income = "Date:" + qry5.value(2).toString();
               ui->textIncome->append(income);
            }
        }
     }//end if qry5.exec
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry5.lastError().text());
        connClose();
    }//end else if qry5 ! exec

    //print out the total income
    income.clear();
    ui->textIncome->append(blank);
    QString totalIncome = QString::number(incomeDbl);
    income = "Total Income: " + totalIncome;
    ui->textIncome->append(income);


    //calculate how much income is outstanding

   //set the rent amoutn to zero


    //how much is rent?
    rent = ui->viewTenantRentRental->text().toDouble();

    //how much rent is due to date?

        //when did the contract  begin?

                qDebug()<<"begin is "<<begin;

                //set the start year
                yearStart();

                //set the start month
                monthStart();

                //set the start day
                dayStart();

        //when did the contract end, or is it currently in place?

        //if still in place
        if(ui->viewTenantCurrent->text() == "Y" or ui->viewTenantCurrent->text() == "y")
        {

            //set the current year
            yearNow();
            setYearNow();

            //set the current month
            monthNow();

            //set the current day
            dayNow();

            //calculate number of years
            numYears();

            //calculate the number of months
            numMonths();

            //calculate the number of days
            numDays();


            //if the contract to date is less than one year
            if(years <= 0)
            {
                //find the rent due in months, which in turn calls prorated months
                rentMonths();

                //calculate the rent outstanding for this contract
                outstandingRent();

            }//end if(years <= 0)
            //else if there has been at least one full year
            else if (years > 0)
            {
                //whole year rent due
                rentYears();

                //extra whole months of rent due, which in turn calls prorated months rent
                rentMonths();

                //calculate the rent outstanding for this contract
                outstandingRent();

            } //end else if (years > 0)

            //otherwise, there is an error
            else if (yearNowInt < yearStartInt)
            {
                QMessageBox::critical(this, tr("Error: "), "Start date of contract is after end date.");

                //set the amount due on the dialog
                ui->viewAmountOwedRental->setText("Error");
            }//end else if else if (yearNowInt < yearStartInt)

        }//end if contract is still in place

        //if it is a past contract
        else
        {
            //set the end year
            yearEnd();

            //find the current year
            yearNow();

            if(yearEndInt > yearNowInt)
            {
                 QMessageBox::critical(this, tr("Error: "), "End date of contract is after current date on a terminated contract. Accurate rent owed cannot be calculated. Rent calculated based on end date being current date.");

                 //set the end date of the contract as the current date
                 setYearNow();
                 dayNow();
                 monthNow();

            }//end if(yearEndInt > yearNowInt)
            else
            {
                //set the end month
                monthEnd();

                //set the end day
                dayEnd();
            }//end else if the end year is not after current year on a terminated contract


            //calculate number of years
            numYears();

            //calculate the number of days
            numDays();

            //calculate the number of months
            numMonths();


            //calculate the rent for the years
            rentYears();

            //calculate the rent for the months (which in turn calls for prorated days)
            rentMonths();

        }//end else if it is a past contract

}

void rentalView2::yearStart()
{
    QString yearStart = begin.mid(0,4);

    //convert year to int
    yearStartInt = yearStart.toInt();
    qDebug()<<"Start year in int is "<<yearStartInt;
}

void rentalView2:: yearEnd()
{
    QString yearEnd = end.mid(0,4);

    //convert to int
    yearEndInt = yearEnd.toInt();
    qDebug()<<"The year end in int is "<<yearEndInt;
}

void rentalView2::yearNow()
{
    yearNowInt = QDate::currentDate().year();
    qDebug()<<"the current year in int is "<<yearNowInt;

}

void rentalView2::setYearNow()
{
    yearEndInt = yearNowInt;
}

void rentalView2::monthStart()
{
    int pos = begin.indexOf("-");
    int pos2;
    QString monthStart;
    monthStart = begin;
    pos = pos +1;
    pos2 = monthStart.length();
    pos2 = pos2 - pos;
    monthStart = monthStart.mid(pos, pos2);
    pos2=monthStart.indexOf("-");
    monthStart = monthStart.mid(0,pos2);

    //convert month to int
    monthStartInt = monthStart.toInt();
    qDebug()<<"month start in int is "<<monthStartInt;
}

void rentalView2::monthEnd()
{
    QString monthEnd = end.mid(5,2);
    monthEndInt = monthEnd.toInt();
    qDebug()<<"The month end in int is "<<monthEndInt;
}

void rentalView2::monthNow()
{
    monthEndInt = QDate::currentDate().month();
    qDebug()<<"the current month in int should be "<<monthEndInt;
}

void rentalView2::dayStart()
{
    QString dayStart = begin.mid(8,2);
    dayStartInt = dayStart.toInt();
    qDebug()<<"The day of start in int is "<<dayStartInt;
}

void rentalView2::dayEnd()
{
    QString dayEnd = end.mid(8,2);
    dayEndInt = dayEnd.toInt();
    qDebug()<<"The end day in int is "<<dayEndInt;
}

void rentalView2::dayNow()
{
    dayEndInt = QDate::currentDate().day();
    qDebug()<<"The current day in int is "<<dayEndInt;
}


void rentalView2::numYears()
{
    years = yearEndInt - yearStartInt;
    qDebug()<<"The contract has been "<<years<<" years long to date";
}

void rentalView2::numMonths()
{


    months = monthEndInt - monthStartInt;
    qDebug()<<"It  has been  "<<months<<" months of the contract";

    if(dayStartInt > dayEndInt)
    {
        //we haven't completed a full month, so reduce months by 1
        qDebug()<<"We are reducing months by one here. We had "<<months<<" of rent due, but now it is ";
        months--;
        qDebug()<<months<<" of rent due";

    }
}

void rentalView2::numDays()
{
    //days if rent has been due to be paid this month already
    if(dayEndInt >= dayStartInt)
    {
        days = dayEndInt - dayStartInt;
        qDebug()<<"the last day is "<<dayEndInt;
        qDebug()<<"the first day is "<<dayStartInt;
        qDebug()<<"It has been "<<days<<" of contract";
        if(days < 0)
        {
            days = 0;
            qDebug()<<" check days for error? [from numDays()]";
        }
    }
    else
    {
        //calculate the number of days this month
       // daysInMonth = QDate::currentDate().daysInMonth();

        //int to hold the number of days left to pay since the previous months rent was  paid
        int daysLastMonth = QDate::currentDate().month();
        daysLastMonth--;
        qDebug()<<"last month in int was "<<daysLastMonth;
        if (daysLastMonth == 2)
        {
            if((yearNowInt % 400==0) || (yearNowInt %4==0 &&yearNowInt %100!=0))
                        daysLastMonth = 29;
            else
                        daysLastMonth = 28;
        }
        else if(daysLastMonth == 4 || daysLastMonth == 6 || daysLastMonth == 9 || daysLastMonth == 11)
        {
            daysLastMonth = 30;
        }
        else
            daysLastMonth = 31;

        //how many days rent are due from last month since the last rent due date
        days = daysLastMonth - dayStartInt;

        //add in the days of rent from this month
        days = days + dayEndInt;

        qDebug()<<"start day is later than current day in number, so our days are "<<days<<"since last rent was due";
    }
}

void rentalView2::rentMonths()
{
    qDebug()<<"In rentMonths.";

    //calculate the rent due in full months
    if(months >= 0)
    {
        monthRent = rent * months;
        qDebug()<<"Rent due in full months is $"<<monthRent;
        rentDue = rentDue + monthRent;
        qDebug()<<"total rent due is "<<rentDue;

        //prorate the rent for the days
        proratedMonthRent();

        //calculate the rent outstanding for this contract
        outstandingRent();

    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), "There has been an error in inputting the contract dates. Please check and recalculate.");
        //set the amount due on the dialog
        ui->viewAmountOwedRental->setText("Error");
    }


}

void rentalView2::rentYears()
{
    //whole year rent due

    yearRent = rent * 12 * years;
    qDebug()<<"Rent due in full years is $"<<yearRent;
    rentDue = yearRent;
    qDebug()<<"total rent due is "<<rentDue;
    rentAmount = QString::number(rentDue);
    ui->viewAmountOwedRental->setText(rentAmount);
}

void rentalView2::proratedMonthRent()
{
     //add in days from this month
    daysInMonth = QDate::currentDate().daysInMonth();
    dayRent = days/daysInMonth;
     qDebug()<<"We have spent "<<dayRent<<" of this rental month";
    dayRent = dayRent * rent;
    qDebug()<<"there is a prorated amount of "<<dayRent<<" accumulated so far due to "<<days<<" days occupied.";

    proRatedRent = rentDue + dayRent;

}

void rentalView2::outstandingRent()
{
    //calculate the amount of rent that is outstanding on this contract
   outstandingRentAmount = proRatedRent - rentReceived;

   //set the amount due on the dialog
   rentAmount = "$" + QString::number(outstandingRentAmount);
   ui->viewAmountOwedRental->setText(rentAmount);

   qDebug()<<"We recevied $"<<rentReceived<<" and we are due $"<<proRatedRent<<", so we still need $"<<outstandingRentAmount;


}
