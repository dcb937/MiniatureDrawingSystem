#include "mydialog.h"

myDialog::myDialog(QWidget* parent) : QDialog(parent)
{
    point_x = 0;
    point_y = 0;
    QFormLayout* form = new QFormLayout(this);
    form->addRow(new QLabel("Please input position of point in order:"));
    form->addRow(new QLabel("If you don't want to continue to add, click cancel."));
    // Value1
    QString value1 = QString("Point_x: ");
    QSpinBox *spinbox1 = new QSpinBox(this);
    spinbox1->setMaximum(1000);
//    spinbox1->setDisplayIntegerBase(16);
//    spinbox1->setPrefix("0x");
    form->addRow(value1, spinbox1);
    connect(spinbox1, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i){  point_x = i; });

    // Value2
    QString value2 = QString("Point_y: ");
    QSpinBox *spinbox2 = new QSpinBox(this);
    spinbox2->setMaximum(1000);
    form->addRow(value2, spinbox2);
    connect(spinbox2, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i){  point_y = i;});

    // Add Cancel and OK button
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    form->addRow(buttonBox);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
