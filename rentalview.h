#ifndef RENTALVIEW_H
#define RENTALVIEW_H
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDialog>

namespace Ui {
class rentalView;
}

class rentalView : public QDialog
{
    Q_OBJECT

public:
    explicit rentalView(QWidget *parent = 0);
    ~rentalView();

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
   // void on_comboProperty_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_comboProp_currentTextChanged(const QString &arg1);

    void on_comboTenant_currentTextChanged(const QString &arg1);

private:
    Ui::rentalView *ui;
    QString fn, ln, code, property, occupancy;
    //void fillTenant();
};

#endif // RENTALVIEW_H
