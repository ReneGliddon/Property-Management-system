#ifndef ADDNEWRENTAL2_H
#define ADDNEWRENTAL2_H

#include <QDialog>

namespace Ui {
class AddNewRental2;
}

class AddNewRental2 : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewRental2(QWidget *parent = 0);
    ~AddNewRental2();

private:
    Ui::AddNewRental2 *ui;
};

#endif // ADDNEWRENTAL2_H
