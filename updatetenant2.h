#ifndef UPDATETENANT2_H
#define UPDATETENANT2_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class UpdateTenant2;
}

class UpdateTenant2 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTenant2(QWidget *parent = 0);
    ~UpdateTenant2();
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
    Ui::UpdateTenant2 *ui;
    QString code;
};

#endif // UPDATETENANT2_H
