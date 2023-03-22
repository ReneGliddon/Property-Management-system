#ifndef UPDATEPROPERTY_H
#define UPDATEPROPERTY_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class UpdateProperty;
}

class UpdateProperty : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateProperty(QWidget *parent = 0);
    ~UpdateProperty();
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
    void on_pushUpdatePropertyDetails_clicked();

private:
    Ui::UpdateProperty *ui;
};

#endif // UPDATEPROPERTY_H
