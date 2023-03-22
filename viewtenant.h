#ifndef VIEWTENANT_H
#define VIEWTENANT_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class ViewTenant;
}

class ViewTenant : public QDialog
{
    Q_OBJECT

public:
    explicit ViewTenant(QWidget *parent = 0);
    ~ViewTenant();
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
    void setTenantCode(QString);
    void setProperty(QString);
    void setOccupancy(QString);
    void display();
    int contract;
    void setContract(int);

private:
    Ui::ViewTenant *ui;
    QString tenantCode;
    QString property;
    QString occupancy;
    int checkContracts();
    void fillRental();
    void fillTenant();
    void fillFinance();
};

#endif // VIEWTENANT_H
