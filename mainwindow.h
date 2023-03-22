#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql>
#include <QDebug>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase mydb;
    QString propertyName;

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
    void on_pushAdd_clicked();

    void on_pushView_clicked();

    void on_pushRenew_clicked();

    void on_comboProperty_currentTextChanged(const QString &arg1);

    void on_pushUpdate_clicked();


private:
    Ui::MainWindow *ui;
    void fillTenant(QString);
    QString property, code, occupancy_code;
};

#endif // MAINWINDOW_H
