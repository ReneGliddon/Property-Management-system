#include "selecttenantview.h"
#include "ui_selecttenantview.h"
#include "tenantlistview.h"
#include "selectcontractfortenantview.h"
#include "selectpropertytenantview.h"
#include "viewtenantselectcontract.h"
#include "viewtenant.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
/* code 1 for viewing a tenant
 *  We test whether this tenant exists, and if there are more than one by that name, we call tenantListView to choose which tenant
 *  If one tenant found, code is saved and sent to SelectPropertyTenantView. 1 is returned.
 *  Call SelectPropertyTenantView for part 2 of viewing a tenant code. We need to find the property
 *  If 0 tenants are found, 0 is returned and an error is sent. Code is ended in this case.
 */


SelectTenantView::SelectTenantView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTenantView)
{
    ui->setupUi(this);

    //check connection
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    qry1.prepare("select * from tenant");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString tenant = qry1.value(1).toString() + " " + qry1.value(2).toString() + " (code: " + qry1.value(0).toString() + ")";
             ui->comboTenant->addItem(tenant);
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
        connClose();
    }
}

SelectTenantView::~SelectTenantView()
{
    delete ui;
}

void SelectTenantView::on_pushSearchTenant_clicked()
{
    QString tenantFull = ui->comboTenant->currentText();
    qDebug()<<"the tenant full is "<<tenantFull;

    int pos = tenantFull.indexOf(":");
    pos = pos + 2;
    int pos2 = tenantFull.indexOf(")");
    int codeLen = pos2-pos;

    QString code = tenantFull.mid(pos,codeLen);
    tenantCode = code;
    testContract();
}

void SelectTenantView::testContract()
{
    //check connection
    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

   QSqlQuery qry1, qry2;

   qDebug()<<"we are testing the contract for the tenant of the code "<<tenantCode;

    //check how many rental contracts this tenant has
    qry1.prepare("select occupancy_code from rental where (tenant_code = :tenantCode)");
    qry1.bindValue(":tenantCode", tenantCode);
    if(qry1.exec())
    {
        int count = 0;


        while(qry1.next())
        {
            count++;
            occupancyCode = qry1.value(0).toString();
        }
        if (count == 0)
        {
            property = "No contract found";
            occupancyCode = "No contract found";

            //create an object
            ViewTenant setTenant;

            //this object has a tenant code only
           setTenant.setProperty(property);
           setTenant.setTenantCode(tenantCode);
                            setTenant.setOccupancy(occupancyCode);

                            qDebug()<<"our tenant has the following values "<<occupancyCode<< " "<< property<<" "<<tenantCode;

                            //close this dialog
                            this->close();

                            //display this object
                            setTenant.display();

        }
        else if (count == 1)
        {

            qDebug()<<"we found one contract";
            qry2.prepare("select nickname from rental where (tenant_code =:tenantCode)");
            qry2.bindValue(":tenantCode", tenantCode);
            if(qry2.exec())
            {
                while(qry2.next())
                {
                    qDebug()<<"our occupancy codde s "<<occupancyCode;
                    property = qry2.value(0).toString();
                    qDebug()<<"our property is "<<property;
                }
                //call the next ui

                //create an object
                 ViewTenant setTenant;

                //this object has a property, tenant and occupancy code
                setTenant.setProperty(property);
                setTenant.setTenantCode(tenantCode);
                setTenant.setOccupancy(occupancyCode);

                qDebug()<<"our tenant has the following values "<<occupancyCode<< " "<< property<<" "<<tenantCode;

                //close this dialog
                this->close();

                //display this object
                setTenant.display();

            }
        }
        else if(count>1)
        {
            //close this dialog
            this->close();

            //bring up a selection of propertiesand occupancies for user to choose the contract from
            SelectContractForTenantView choose;
            choose.display(tenantCode);



        }//end if count >1
     }//end (qry1.exec())
    else
    {
        QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
        connClose();
    }//end else
}//end check contract
