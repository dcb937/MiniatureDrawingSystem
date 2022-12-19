#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawingboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    QWidget* test = new drawingBoard(this);
//    test->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

