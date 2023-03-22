#include "tenantlistview.h"
#include "ui_tenantlistview.h"
//#include "viewtenant.h"
//#Include "selectpropertytenantview"
//#include "viewtenantselectcontract.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include "selectpropertytenantview.h"

/*code 1.1 for viewing a tenant
 * Called from SelectTenantView
 * This is called when there are multiple tenants with the same name
 * Before being called, the list of tenant codes are sent to the textbrowser for display
 * list is displayed and user chooses which tenant they want to view by typing in the tenant code
 * When tenant is selected, we know the tenant and need to find the property
 * We call SelectPropertyTenantView to find the property. This is part 2.
 * */

tenantListView::tenantListView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tenantListView)
{
    ui->setupUi(this);
}

tenantListView::~tenantListView()
{
    delete ui;
}

void tenantListView::setList(QString list)
{
    tenants = list;
}

void tenantListView::display()
{
    ui->texttenants->setText(tenants);
    this->exec();
}
void tenantListView::on_pushButton_clicked()
{
    //we know the code,now we need to check teh contract
    SelectPropertyTenantView contract;
    contract.setCode(ui->lineEdit_tenantCode->text());
    contract.display();
    this->close();


 }
