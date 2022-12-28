#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>

class myDialog : public QDialog
{
    Q_OBJECT
public:
    myDialog(QWidget* parent = 0, QString s1 = "line1", QString s2 = "line2", QString s3 = "line3", QString s4 = "");
    int point_x;
    int point_y;
    int radius;
};

#endif // MYDIALOG_H
