#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QDialog>
#include "mainwindow.h"
#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT


public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    QSqlDatabase mydb;
    QString propertyName;
    void display();

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
    void on_pushButton_Login_clicked();

    void on_pushButton_SignUp_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
