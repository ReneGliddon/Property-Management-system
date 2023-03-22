#ifndef SELECTTENANTRENTAL_H
#define SELECTTENANTRENTAL_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
namespace Ui {
class SelectTenantRental;
}

class SelectTenantRental : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTenantRental(QWidget *parent = 0);
    ~SelectTenantRental();
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

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::SelectTenantRental *ui;
    int testTenant();
    void getCodes();
    QString tenant_code;
    QString list;
    int count;
};

#endif // SELECTTENANTRENTAL_H
