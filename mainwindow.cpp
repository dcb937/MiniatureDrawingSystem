#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawingboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionBezierCurve, &QAction::triggered, ui->board, &drawingBoard::bezierCurve);
    connect(ui->actionCircle, &QAction::triggered, ui->board, &drawingBoard::circle);
    connect(ui->actionConvex, &QAction::triggered, ui->board, &drawingBoard::convex);
    connect(ui->actionConcave, &QAction::triggered, ui->board, &drawingBoard::concave);
}

MainWindow::~MainWindow()
{
    delete ui;
}

