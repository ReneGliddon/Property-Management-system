#ifndef RENEWCONTRACT_H
#define RENEWCONTRACT_H

#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class RenewContract;
}

class RenewContract : public QDialog
{
    Q_OBJECT

public:
    explicit RenewContract(QWidget *parent = 0);
    ~RenewContract();

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

    void display(QString, QString, QString);

private slots:
    void on_pushRenew_clicked();

private:
    Ui::RenewContract *ui;
    QString property, tenant_code, occupancy_code, newOccupancyCode;
    void insertValues();
    bool generateOccupancyCode();
    bool testOccupancy(QString);
    QDate date_start, date_end;
};

#endif // RENEWCONTRACT_H
