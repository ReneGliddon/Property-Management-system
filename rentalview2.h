#ifndef RENTALVIEW2_H
#define RENTALVIEW2_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class rentalView2;
}

class rentalView2 : public QDialog
{
    Q_OBJECT

public:
    explicit rentalView2(QWidget *parent = 0);
    ~rentalView2();
    //define the database connection
        QSqlDatabase mydb;

        void connClose()
        {
            mydb.close();
            mydb.removeDatabase(QSqlDatabase::defaultConnection);
        }

        bool connOpen()
        {
            mydb = QSqlDatabase::addDatabase("QSQLITE");
            mydb.setDatabaseName("C:/QT/Qt5.3.0/Tools/QtCreator/bin/LoginApp/database.db");
            if (!mydb.open())
            {
                qDebug()<<("Failed to open the database");
                return false;
            }
            else
            {
                qDebug()<<("Connected...");
                return true;
            }
        }
        void setValues(QString, QString, QString, QString, QString);
        void display();
protected:
        QString fn, ln, code, property, occupancy, begin, end, rentAmount;
        int yearStartInt, monthStartInt, dayStartInt, yearNowInt, monthEndInt, dayEndInt, yearEndInt;
        int years, months, days;
        void populate();
        void yearStart();
        void yearEnd();
        void yearNow();
        void monthStart();
        void monthEnd();
        void monthNow();
        void dayStart();
        void dayEnd();
        void dayNow();
        void numYears();
        void numMonths();
        void numDays();
        void rentMonths();
        void rentYears();
        void proratedMonthRent();
        void setYearNow();
        void outstandingRent();
        double rent, rentDue, incomeDue, outstandingRentAmount, proRatedRent, daysInMonth, yearRent, monthRent, dayRent, incomeDbl, rentReceived;

private:
    Ui::rentalView2 *ui;


};

#endif // RENTALVIEW2_H
