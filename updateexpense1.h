#ifndef UPDATEEXPENSE1_H
#define UPDATEEXPENSE1_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QDialog>

namespace Ui {
class UpdateExpense1;
}

class UpdateExpense1 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateExpense1(QWidget *parent = 0);
    ~UpdateExpense1();
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
    //void on_pushButton_clicked();
    
    void on_comboProperty_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::UpdateExpense1 *ui;
    QString property, expenseDate;
    QString code;
     void fillDate(QString arg1);
     void fillDescription();
};

#endif // UPDATEEXPENSE1_H
