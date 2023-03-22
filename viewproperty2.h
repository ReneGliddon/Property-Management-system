#ifndef VIEWPROPERTY2_H
#define VIEWPROPERTY2_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>
#include <QDate>
namespace Ui {
class ViewProperty2;
}

class ViewProperty2 : public QDialog
{
    Q_OBJECT

public:
    explicit ViewProperty2(QWidget *parent = 0);
    ~ViewProperty2();
    void setProperty(QString);
    void display();
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

private:
    Ui::ViewProperty2 *ui;
    QString property, notes;
    QDate startDate, endDate;
    double purchaseCost, rent, HOA;
    bool sewage, utilities;

    void fillProperty();
    void fillTenant();
    void fillFinances();

};

#endif // VIEWPROPERTY2_H
