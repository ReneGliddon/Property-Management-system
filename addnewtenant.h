#ifndef ADDNEWTENANT_H
#define ADDNEWTENANT_H

#include <QDialog>

namespace Ui {
class AddNewTenant;
}

class AddNewTenant : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewTenant(QWidget *parent = 0);
    ~AddNewTenant();
    void generateTenantCode();
    void saveToDatabase();

private slots:
    void on_pushSaveRentalDetails_clicked();

    void on_pushSaveTenant_clicked();

private:
    Ui::AddNewTenant *ui;
    QString  lineEdit_tenantCode, lineEdit_tenantFN, lineEdit_tenantLN, lineEdit_tenantEmail, lineEdit_tenantPhone, lineEdit_notesTenant;
};

#endif // ADDNEWTENANT_H
