#include "mydialog.h"

myDialog::myDialog(QWidget* parent, QString s1, QString s2, QString s3, QString s4) : QDialog(parent)
{
    point_x = 0;
    point_y = 0;
    radius = 0;
    QFormLayout* form = new QFormLayout(this);
    // 指定了父对象的组件 new之后不用手动进行销毁
    form->addRow(new QLabel(s1, this));
    // form->addRow(new QLabel("If you don't want to continue to add, click cancel."));

    // Value1
    QString value1 = QString(s2);
    QSpinBox *spinbox1 = new QSpinBox(this);
    spinbox1->setMaximum(1000);
//    spinbox1->setDisplayIntegerBase(16);
//    spinbox1->setPrefix("0x");
    form->addRow(value1, spinbox1);
    connect(spinbox1, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i){  point_x = i; });

    // Value2
    QString value2 = QString(s3);
    QSpinBox *spinbox2 = new QSpinBox(this);
    spinbox2->setMaximum(1000);
    form->addRow(value2, spinbox2);
    connect(spinbox2, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i){  point_y = i;});

    // Value3
    if(s4 != ""){
    QString value3 = QString(s4);
    QSpinBox *spinbox3 = new QSpinBox(this);
    spinbox3->setMaximum(1000);
    form->addRow(value3, spinbox3);
    connect(spinbox3, QOverload<int>::of(&QSpinBox::valueChanged),
        [=](int i){  radius = i;});
    }


    // Add Cancel and OK button
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    form->addRow(buttonBox);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
