#ifndef SELECTTENANTVIEW_H
#define SELECTTENANTVIEW_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class SelectTenantView;
}

class SelectTenantView : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTenantView(QWidget *parent = 0);
    ~SelectTenantView();
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

private:
    Ui::SelectTenantView *ui;
    void testContract();
    QString tenantCode, property, occupancyCode;

};

#endif // SELECTTENANTVIEW_H
