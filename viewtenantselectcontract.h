#ifndef VIEWTENANTSELECTCONTRACT_H
#define VIEWTENANTSELECTCONTRACT_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class viewTenantSelectContract;
}

class viewTenantSelectContract : public QDialog
{
    Q_OBJECT

public:
    explicit viewTenantSelectContract(QWidget *parent = 0);
    ~viewTenantSelectContract();
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
    void setCode(QString);
    void display();
private:
    Ui::viewTenantSelectContract *ui;
    QString code, property;
    void testContract();
};

#endif // VIEWTENANTSELECTCONTRACT_H
