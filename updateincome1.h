#ifndef UPDATEINCOME1_H
#define UPDATEINCOME1_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class UpdateIncome1;
}

class UpdateIncome1 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateIncome1(QWidget *parent = 0);
    ~UpdateIncome1();
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
    void on_pushButton_clicked();
    void on_comboProperty_currentTextChanged(const QString &arg1);

private:
    Ui::UpdateIncome1 *ui;
    QString property, incomeDate;
    QString code;
     void fillDate(QString arg1);
     void fillDescription();
};

#endif // UPDATEINCOME1_H
