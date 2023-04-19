#ifndef UPDATERENTAL1_H
#define UPDATERENTAL1_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class UpdateRental1;
}

class UpdateRental1 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateRental1(QWidget *parent = 0);
    ~UpdateRental1();
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
    void on_pushUpdateRentalDetails_clicked();




    void on_comboProperty_currentTextChanged(const QString &arg1);

private:
    Ui::UpdateRental1 *ui;
};

#endif // UPDATERENTAL1_H
