#ifndef SELECTTENANT_H
#define SELECTTENANT_H
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class SelectTenant;
}

class SelectTenant : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTenant(QWidget *parent = 0);
    ~SelectTenant();
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

private slots:
    void on_pushSearchTenant_clicked();

private:
    Ui::SelectTenant *ui;
    QString tenant_fn;
    QString tenant_ln;
    QString tenantCode;
    int testTenant();
};

#endif // SELECTTENANT_H
