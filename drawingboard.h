#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include "mydialog.h"

#define BEZIER 1
#define CIRCLE 2
#define CONVEX 3
#define CONCAVE 4

class drawingBoard: public QWidget
{
    Q_OBJECT
public:
    double precision=0.001;
    drawingBoard(QWidget* parent = 0);
    QList<QPoint> pointList;

private:
    int status;
    QPainterPath getBezierPainterPath();

public slots:
    void bezierCurve();
    void circle();
    void convex();
    void concave();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

};

#endif // DRAWINGBOARD_H
