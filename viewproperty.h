#ifndef VIEWPROPERTY_H
#define VIEWPROPERTY_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class ViewProperty;
}

class ViewProperty : public QDialog
{
    Q_OBJECT

public:
    explicit ViewProperty(QWidget *parent = 0);
    ~ViewProperty();
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

private:
    Ui::ViewProperty *ui;
    QString property;
};

#endif // VIEWPROPERTY_H
