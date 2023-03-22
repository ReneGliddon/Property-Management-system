#ifndef VIEWFINANCES2_H
#define VIEWFINANCES2_H

#include <QDialog>

namespace Ui {
class viewFinances2;
}

class viewFinances2 : public QDialog
{
    Q_OBJECT

public:
    explicit viewFinances2(QWidget *parent = 0);
    ~viewFinances2();
    void display();

private:
    Ui::viewFinances2 *ui;
};

#endif // VIEWFINANCES2_H
