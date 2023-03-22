#ifndef OVERRIDEEXPENSE_H
#define OVERRIDEEXPENSE_H
#include <QDate>
#include <QDialog>

namespace Ui {
class OverrideExpense;
}

class OverrideExpense : public QDialog
{
    Q_OBJECT

public:
    explicit OverrideExpense(QWidget *parent = 0);
    ~OverrideExpense();
    void setValues(QString, double, QString, QDate);

private slots:
    void on_pushButton_clicked();

private:
    Ui::OverrideExpense *ui;
    QString description, propertyNickname;
    double expenseAmount;
    QDate dateOfExpense;
};

#endif // OVERRIDEEXPENSE_H
