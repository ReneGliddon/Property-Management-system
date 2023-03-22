#include "signup.h"
#include "ui_signup.h"
#include <QMessageBox>

SignUp::SignUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    Login conn;
    if (!conn.connOpen())
        ui->label_signUpStatus->setText("Failed to open the database");
    else
        ui->label_signUpStatus->setText("Connected...");
}

SignUp::~SignUp()
{
    delete ui;
}


void SignUp::on_pushButton_Save_clicked()
{
    Login conn;
    QString username, password;
    username = ui->txt_username->text();
    password = ui->txt_password->text();

    if(!conn.connOpen())
    {
        qDebug()<<"Failed to open the database";
        return;
    }

    conn.connOpen();
    QSqlQuery qry;
    QSqlQuery qry2;
    if (!(username=="" || password==""))
    {
        qry2.prepare("select * from users where username = '"+username+"'");
        if(qry2.exec())
        {
            int count = 0;
            while(qry2.next())
            {
               count++;
            }
            if (count >= 1)
            {
                QMessageBox::critical(this, tr("Error"),tr("Error. That username is taken. Try another."));
            }
            else
            {
                qry.prepare("insert into users(username, password) values ('"+username+"', '"+password+"')");
                if(qry.exec())
                {
                    QMessageBox::information(this, tr("Save"), tr("Username and password saved successfully."));
                    conn.connClose();
                    MainWindow *newmain= new MainWindow();
                    newmain->show();
                    hide();

                }
                else
                    QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());

            }

        }
        else
            QMessageBox::critical(this, tr("Error:: "), qry.lastError().text());

       }
    else
        QMessageBox::critical(this, tr("Error"),tr("Error. Username and password cannot be blank."));
}
