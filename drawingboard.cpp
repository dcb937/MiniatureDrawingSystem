#include "drawingboard.h"


drawingBoard::drawingBoard(QWidget* parent) : QWidget(parent)
{
    qDebug() << "drawingBoard initialed";
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);      //设置背景自动填充

}

void drawingBoard::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressed";
}
