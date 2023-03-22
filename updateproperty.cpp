#include "updateproperty.h"
#include "ui_updateproperty.h"
#include <QMessageBox>
#include <QtSql>
#include <QDebug>
#include <QDate>

UpdateProperty::UpdateProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateProperty)
{
    ui->setupUi(this);

    connOpen();
    //populate properties combobox
    QSqlQuery qry1;
    qry1.prepare("select nickname from properties");
    if(qry1.exec())
    {
        while(qry1.next())
        {
            ui->comboBox->addItem(qry1.value(0).toString());
        }

   }
   else
   {
       QMessageBox::critical(this, tr("Error:: "), qry1.lastError().text());
       //connClose();
   }




}

UpdateProperty::~UpdateProperty()
{
    delete ui;
}

void UpdateProperty::on_pushUpdatePropertyDetails_clicked()
{

    QMessageBox::information(this, tr("Updated"),tr("Record Updated."));
}
