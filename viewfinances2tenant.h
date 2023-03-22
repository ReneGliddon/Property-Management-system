#ifndef VIEWFINANCES2TENANT_H
#define VIEWFINANCES2TENANT_H

#include <QDialog>

namespace Ui {
class viewFinances2Tenant;
}

class viewFinances2Tenant : public QDialog
{
    Q_OBJECT

public:
    explicit viewFinances2Tenant(QWidget *parent = 0);
    ~viewFinances2Tenant();

private:
    Ui::viewFinances2Tenant *ui;
};

#endif // VIEWFINANCES2TENANT_H
