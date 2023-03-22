#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "signup.h"

#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QtSql>


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    QPixmap pix(":/resources/rr99.jpg");
    int w = ui->label_pic->width();
    int h = ui->label_pic->height();
    ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    if (!connOpen())
        ui->labelStatus->setText("Failed to open the database");
    else
       ui->labelStatus->setText("Connected...");
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_Login_clicked()
{
    //variables to hold username and password given by user
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    //check that the database is open
    /*if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
        return;
    }
*/
   // connOpen();
    QSqlQuery qry;
    qry.prepare("select * from users where username = '"+username +"' and password = '"+password+"'");
    if(qry.exec())
    {
       int count = 0;
       while(qry.next())
         {
           count++;
         }
       //if a username with matching password are found in the database, open the main window
       if (count == 1)
       {
            ui->labelStatus->setText("Username and password are correct");
            connClose();
            this->close();

            MainWindow *newmain= new MainWindow();
            newmain->show();
            this->hide();
       }
       if (count >1)
           ui->labelStatus->setText("Duplicate username and password");
       if(count <1)
           ui->labelStatus->setText("Username and password are not correct.");

    }

}

void Login::on_pushButton_SignUp_clicked()
{
    this->hide();
    ui->labelStatus->setText("Signing you up!");
    this->hide();
    SignUp signUp;
    signUp.setModal(true);
    signUp.exec();
}

void Login::display()
{
    this->exec();
}
