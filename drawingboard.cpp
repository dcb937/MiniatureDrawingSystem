#include "drawingboard.h"


drawingBoard::drawingBoard(QWidget* parent) : QWidget(parent)
{
    qDebug() << "drawingBoard initialed";
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);      //设置背景自动填充

    status = 0;
}

void drawingBoard::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressed";
    qDebug() << event->pos();
}

void drawingBoard::bezierCurve()
{
    status = 0;        // 因为实际上每次点击菜单按钮的时候实际上都会刷新着画布，为了避免点击按钮，pointList被clear后执行画图语句而导致at()函数超出范围崩溃
    pointList.clear();
    qDebug() << "bezier begin";

    myDialog dialog(this);

    // Process when OK button is clicked
    while (dialog.exec() == QDialog::Accepted) {
        /*......*/
        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
        QPoint point(dialog.point_x, dialog.point_y);
        pointList << point;
    }

    if(pointList.length() >= 2){
        status = BEZIER;
        update();
    }
}

void drawingBoard::circle()
{
    status = 0;
    pointList.clear();
    qDebug() << "circle begin";
    myDialog dialog(this);

    // Process when OK button is clicked
    while (dialog.exec() == QDialog::Accepted) {
        /*......*/
        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
        QPoint point(dialog.point_x, dialog.point_y);
        pointList << point;
    }

    status = CIRCLE;
    update();
}

void drawingBoard::convex()
{
    status = 0;
    pointList.clear();
    qDebug() << "convex begin";
    myDialog dialog(this);

    // Process when OK button is clicked
    while (dialog.exec() == QDialog::Accepted) {
        /*......*/
        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
        QPoint point(dialog.point_x, dialog.point_y);
        pointList << point;
    }

    status = CONVEX;
    update();
}

void drawingBoard::concave()
{
    status = 0;
    pointList.clear();
    qDebug() << "concave begin";
    myDialog dialog(this);

    // Process when OK button is clicked
    while (dialog.exec() == QDialog::Accepted) {
        /*......*/
        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
        QPoint point(dialog.point_x, dialog.point_y);
        pointList << point;
    }

    status = CONCAVE;
    update();
}


void drawingBoard::paintEvent(QPaintEvent *event)
{
    QPainter* painter = new QPainter(this);
    painter->setPen(Qt::red);

    switch(status){
        case BEZIER:
    {
        for(int i = 0; i < pointList.size() - 1; i++){
            painter->drawLine(pointList[i], pointList[i+1]);
        }
        QPainterPath path = getBezierPainterPath();
        painter->setPen(Qt::blue);
        painter->drawPath(path);
        break;
    }
        case CIRCLE:
            break;
        case CONVEX:
            break;
        case CONCAVE:
            break;
    }

    delete painter;
}

QPainterPath drawingBoard::getBezierPainterPath()
{
    QPainterPath path;
    path.moveTo(pointList.at(0));

    for(double t=0; t<1; t+= precision)
    {
        int n = pointList.length();
        double *coefficient = new double[n]; //系数数组
        memset(coefficient,0,sizeof(coefficient));

        coefficient[0] = 1.0;
        double u1 = 1.0-t;
        for(int j=1; j<=n-1;j++)
        {
            double saved  = 0.0;
            for(int k=0; k<j; k++)
            {
                double temp = coefficient[k];
                coefficient[k] = saved+u1*temp;
                saved = t*temp;
            }
            coefficient[j] = saved;
        }

        QPoint tempPoint(0.0,0.0);
        for(int i=0;i<pointList.length();i++)
        {
            QPoint temp  = pointList.at(i);
            tempPoint = tempPoint +temp*coefficient[i];
        }


        path.lineTo(tempPoint);
        delete[] coefficient;
    }

    return path;

}


