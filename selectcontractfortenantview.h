#ifndef SELECTCONTRACTFORTENANTVIEW_H
#define SELECTCONTRACTFORTENANTVIEW_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class SelectContractForTenantView;
}

class SelectContractForTenantView : public QDialog
{
    Q_OBJECT

public:
    explicit SelectContractForTenantView(QWidget *parent = 0);
    ~SelectContractForTenantView();
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

    void setList(QString);
    void setTenant(QString);
    void display(QString);

private slots:
    void on_pushButton_clicked();

private:
    Ui::SelectContractForTenantView *ui;
    QString property, list, tenantCode;
};

#endif // SELECTCONTRACTFORTENANTVIEW_H
