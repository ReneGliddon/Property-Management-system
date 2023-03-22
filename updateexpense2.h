#ifndef UPDATEEXPENSE2_H
#define UPDATEEXPENSE2_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class UpdateExpense2;
}

class UpdateExpense2 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateExpense2(QWidget *parent = 0);
    ~UpdateExpense2();
    void display(QString expenseCode);
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
    void on_pushUpdateIncome_clicked();

private:
    Ui::UpdateExpense2 *ui;
    QString code;
};

#endif // UPDATEEXPENSE2_H
