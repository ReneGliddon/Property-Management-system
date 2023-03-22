#ifndef TENANTLIST_H
#define TENANTLIST_H

#include <QDialog>
#include <QTextBrowser>

namespace Ui {
class tenantList;
}

class tenantList : public QDialog
{
    Q_OBJECT

public:
    explicit tenantList(QWidget *parent = 0);
    ~tenantList();
    void setList(QString);
    void display();

private slots:
    void on_pushButton_clicked();

private:
    Ui::tenantList *ui;
    QString tenants;
};

#endif // TENANTLIST_H
