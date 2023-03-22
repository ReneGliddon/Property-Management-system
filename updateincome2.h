#ifndef UPDATEINCOME2_H
#define UPDATEINCOME2_H

#include <QDialog>

namespace Ui {
class UpdateIncome2;
}

class UpdateIncome2 : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateIncome2(QWidget *parent = 0);
    ~UpdateIncome2();
    void display(QString code);

private slots:
    void on_pushUpdateIncome_clicked();

private:
    Ui::UpdateIncome2 *ui;
    QString property, code;
};

#endif // UPDATEINCOME2_H
