#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include "login.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>


namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = 0);
    ~SignUp();

private slots:
    void on_pushButton_Save_clicked();

private:
    Ui::SignUp *ui;
};

#endif // SIGNUP_H
