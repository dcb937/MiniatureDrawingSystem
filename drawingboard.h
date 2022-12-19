#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H

#include <QObject>
#include <QWidget>
#include <QDebug>

class drawingBoard: public QWidget
{
    Q_OBJECT
public:
    drawingBoard(QWidget* parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // DRAWINGBOARD_H
