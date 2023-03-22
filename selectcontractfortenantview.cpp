#include "selectcontractfortenantview.h"
#include "ui_selectcontractfortenantview.h"
#include "viewtenant.h"
/* Part 2.1 of View Tenant
 * Called from SelectPropertyTenantView
 * we have the tenant and we need the property to display
 * When called, tenant code is set, all properties associated with that tenant are placed in a list and displayed in the ui
 * Property is typed and the push button clicked
 * We set the property and tenant and send this to View Tenant (part 3)
 * We call View Tenant for display
 */
SelectContractForTenantView::SelectContractForTenantView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectContractForTenantView)
{
    ui->setupUi(this);
}

SelectContractForTenantView::~SelectContractForTenantView()
{
    delete ui;
}

void SelectContractForTenantView::display(QString code)
{
    //set the tenant code
    tenantCode = code;

    //check connection
    connOpen();

    if(!connOpen())
    {
        qDebug()<<"Failed to open the database";
    }

    QSqlQuery qry1;

    //add the tenant's rental contracts
    qry1.prepare("select nickname, dateStart, dateEnd from rental where tenant_code = (:code)");
    qry1.bindValue(":code", code);
    if(qry1.exec())
    {
        while(qry1.next())
        {
            QString contract = "Property: " + qry1.value(0).toString() + ".  Start: " + qry1.value(1).toString() + ". End: " + qry1.value(2).toString();
            ui->comboProperty->addItem(contract);
        }
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
        connClose();
    }

    this->exec();
}
void SelectContractForTenantView::setList(QString list)
{
    //pass
}

void SelectContractForTenantView::on_pushButton_clicked()
{
    //create an object
    ViewTenant setTenant;
    setTenant.setTenantCode(tenantCode);

    //get the contract occupancy code
    QString propertyFull = ui->comboProperty->currentText();
    int pos = propertyFull.indexOf(":");

    //get the position of the property name
    pos = pos + 2;
    int pos2 = propertyFull.indexOf(".");

    //get the length of the property name
    int strLen = pos2 - pos;

    //get the property name
    property = propertyFull.mid(pos, strLen);

    //set the property name
    setTenant.setProperty(property);

   //get the start date
   pos = pos2 + 10;
   pos2 = propertyFull.indexOf("E");
   pos2 = pos2 - 2;
   int startDateLen = pos2-pos;
   QString startDate = propertyFull.mid(pos,startDateLen);

   //get the end date
   pos = pos2 + 7;
   pos2 = propertyFull.length();
   QString endDate = propertyFull.mid(pos, pos2);

   //this object has a property, tenant and dates

   //check connection
   connOpen();

   if(!connOpen())
   {
       qDebug()<<"Failed to open the database";
   }

   //find the occupancy code
   QSqlQuery qry1;

   //add the tenant's rental contracts
   qry1.prepare("SELECT occupancy_code FROM rental WHERE tenant_code = (:code) AND nickname = (:nickname) AND dateStart = (:start) AND dateEnd = (:end)");
   qry1.bindValue(":code", tenantCode);
   qry1.bindValue(":nickname", property);
   qry1.bindValue(":start", startDate);
   qry1.bindValue(":end", endDate);
   if(qry1.exec())
   {
       while(qry1.next())
       {
           setTenant.setOccupancy(qry1.value(0).toString());
       }
       connClose();
   }
   else
   {
       QMessageBox::critical(this, tr("Error: "), qry1.lastError().text());
       connClose();
   }

    //display this object
    setTenant.display();
    this->close();
}
