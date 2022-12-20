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
    myDialog(QWidget* parent = 0);
    int point_x;
    int point_y;
};

#endif // MYDIALOG_H
