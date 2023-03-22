#include "viewtenantselectcontract.h"
//#include "ui_viewtenantselectcontract.h"
#include "selectcontractfortenantview.h"
#include "viewtenant.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

viewTenantSelectContract::viewTenantSelectContract(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewTenantSelectContract)
{
    ui->setupUi(this);
    void testContract();
}

viewTenantSelectContract::~viewTenantSelectContract()
{
    delete ui;
}

//we have the tenant, set it
void viewTenantSelectContract::setCode(QString tenantCode)
{
    code = tenantCode;
}

void viewTenantSelectContract::display()
{
 //not neeeded
}

void viewTenantSelectContract::testContract()
{
    QSqlQuery qry1, qry2;

     //check how many rental contracts this tenant has
     qry1.prepare("select nickname from rental where tenant_code = (:tenantCode)");
     qry1.bindValue(":tenantCode", code);
     if(qry1.exec())
     {
         int count = 0;
         while(qry1.next())
         {
             count++;
         }
         if (count == 0)
         {
             property = "No contract found";

         }
         else if (count == 1)
         {
             qry2.prepare("select nickname from rental where (tenant_code =:tenantCode)");
             qry2.bindValue(":tenantCode", code);
             if(qry2.exec())
             {
                 while(qry2.next())
                 {
                     property = qry2.value(0).toString();
                 }
                 //we have the property and the tenant
                 ViewTenant setTenant;
                 setTenant.setTenantCode(code);
                 setTenant.setProperty(property);
                 setTenant.display();
                 this->close();
             }
         }
         else if(count>1)
         {

             //variable for the property nicknames
             QString list;
             //save the values to the list (it must be done twice or it won't print to the textBrowser. Don't ask me why)
             qry2.prepare("select nickname from rental where (tenant_code =:tenantCode)");
             qry2.bindValue(":tenantCode", code);
             if(qry2.exec())
             {
                 while(qry2.next())
                 {
                     list.append(qry2.value(0).toString());
                     for(int i = 1; i <count; i++)
                     {
                         //add the latest code to the list
                         list.append(qry2.value(i).toString());
                         //new line
                         list.append("\n");
                     }
                 }
             }//end if(qry2.exec())
             else
             {
                 QMessageBox::critical(this, tr("Error"), qry2.lastError().text());
                 connClose();

             }//end else if !qry2.exec()

             //bring up a selection of properties for user to choose the contract from
             SelectContractForTenantView choose;
             choose.setList(list);
             choose.display(code);


         }//end if count >1
      }//end (qry1.exec())
     else
     {
         QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());

         connClose();
     }//end if !(qry1.exec())
 }//end check contract

