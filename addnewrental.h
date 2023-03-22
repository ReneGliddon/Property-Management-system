#ifndef ADDNEWRENTAL_H
#define ADDNEWRENTAL_H

#include <QDialog>
#include <QDate>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
namespace Ui {
class AddNewRental;
}

class AddNewRental : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewRental(QWidget *parent = 0);
    ~AddNewRental();
    //set tenant code
    void setTenantCode(QString);

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

private slots:
    void on_pushSaveRentalDetails_clicked();

private:
    Ui::AddNewRental *ui;
    //string values
    QString radio_utility, radio_sewage, radio_current, lineEdit_notesRental, tenant_code, lineEdit_nickname, lineEdit_occupancyCode;
    //int value
    int spinRent;
    //date values
    QDate date_start;
    QDate date_end;
    QDate defaultStart, defaultEnd;

    //set values
    void setValues();

    //test whether tenant already has a listing
    int testTenant();

    //input values into database
    void inputValues();

    //generate an occupancy code
    int generateOccupancy();

    //test occupancy code
    int testOccupancy();

    //test property nickname
    int testProperty();

};

#endif // ADDNEWRENTAL_H
