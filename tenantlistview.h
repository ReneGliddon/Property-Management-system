#ifndef TENANTLISTVIEW_H
#define TENANTLISTVIEW_H

#include <QDialog>

namespace Ui {
class tenantListView;
}

class tenantListView : public QDialog
{
    Q_OBJECT

public:
    explicit tenantListView(QWidget *parent = 0);
    ~tenantListView();
    void setList(QString);
    void display();

private slots:
    void on_pushButton_clicked();

private:
    Ui::tenantListView *ui;
    QString tenants;
};

#endif // TENANTLISTVIEW_H
