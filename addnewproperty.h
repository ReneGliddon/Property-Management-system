#ifndef ADDNEWPROPERTY_H
#define ADDNEWPROPERTY_H

#include <QDialog>

namespace Ui {
class AddNewProperty;
}

class AddNewProperty : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewProperty(QWidget *parent = 0);
    ~AddNewProperty();

private slots:
    void on_pushSavePropertyDetails_clicked();

private:
    Ui::AddNewProperty *ui;
};

#endif // ADDNEWPROPERTY_H
