#ifndef OVERRIDEINCOME_H
#define OVERRIDEINCOME_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class OverrideIncome;
}

class OverrideIncome : public QDialog
{
    Q_OBJECT

public:
    explicit OverrideIncome(QWidget *parent = 0);
    ~OverrideIncome();

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

    void setValues(QString, QString, double, QDate, QString payer);

private slots:
    void on_pushButton_clicked();

private:
    Ui::OverrideIncome *ui;
    //number value
    double incomeAmount;
    //text inputs
    QString propertyNickname, description, payerCode;
    //income date
    QDate incomeDate;
    //income code
    QString incomeCode;

    //functions
    void inputValues();
    void setIncomeCode();
    int testIncome();
};

#endif // OVERRIDEINCOME_H
