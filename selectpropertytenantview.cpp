#include "selectpropertytenantview.h"
#include "selectcontractfortenantview.h"
#include "viewtenant.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>

/*Part 2 of Viewing a tenant
 * This code is called from SelectTenantView if 1 tenant with the name is in the database, or from tenantListView if there ar emultiple tenants
 * Before executing, the tenant code is sent and saved
 * This code selects the property to be displayed
 * There is no ui
 * If there is no contract found for the tenant, "no contract found" is the property name and we move onto Part 3: Viewing by calling ViewTenant
 * If we find 1 contract, we save and send the property name to Part 3: ViewTenant
 * If multiple properties are found, we need to select the one desired for displying by sending the list of properties toSelectContractForTenantView
 * We then call SelectContractForTenantView (part 2.1)
 * */
SelectPropertyTenantView::SelectPropertyTenantView()
{
    testContract();
}
void SelectPropertyTenantView::display()
{
    connOpen();
    testContract();
}

SelectPropertyTenantView::~SelectPropertyTenantView()
{
    delete this;
}

//we have the tenant, set it
void SelectPropertyTenantView::setCode(QString tenantCode)
{
    qDebug()<<"The code sent is "<<tenantCode;
    code = tenantCode;
    display();
}

void SelectPropertyTenantView::testContract()
{
     connOpen();
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
             ViewTenant setTenant;
             setTenant.setTenantCode(code);
             setTenant.setProperty(property);
             setTenant.display();

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
                 //QMessageBox::critical(this, tr("Error"),tr("Error. No tenant by that name in the database."));
                 //QMessageBox::critical(this, "Error", qry2.lastError().text());
                  qDebug()<<"error with qry2";
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
         //QMessageBox::critical(this, "Error:: ", qry1.lastError().text());
        qDebug()<<"error with qry1";
         connClose();
     }//end if !(qry1.exec())
 }//end check contract

