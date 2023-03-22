#ifndef UPDATETENANT1_H
#define UPDATETENANT1_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class UpdateTenant1;
}

class UpdateTenant1 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateTenant1(QWidget *parent = 0);
    ~UpdateTenant1();
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
    void on_pushSearchTenant_clicked();

    void on_pushSaveTenant_clicked();

private:
    Ui::UpdateTenant1 *ui;
    int testTenant();
    QString code, list;
};

#endif // UPDATETENANT1_H
