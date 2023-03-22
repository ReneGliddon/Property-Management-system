#ifndef SELECTPROPERTYTENANTVIEW_H
#define SELECTPROPERTYTENANTVIEW_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
class SelectPropertyTenantView
{
public:
    SelectPropertyTenantView();
    ~SelectPropertyTenantView();
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
        void setCode(QString);
        void display();
private:
        QString code, property;
        void testContract();

};


#endif // SELECTPROPERTYTENANTVIEW_H
