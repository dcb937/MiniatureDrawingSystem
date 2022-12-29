#include "drawingboard.h"
#include <utility>
#include <iostream>
#include <unistd.h>
#include <vector>

struct Point2d {
    double x, y;
    bool operator < (const Point2d &rhs) const {
        if (x==rhs.x) return y < rhs.y;
        return x < rhs.x;
    }
};
struct Line{
    Point2d start;
    Point2d end;
};


const int inf = 0x3f3f3f3f;
const double eps = 0.001;
std::vector<Point2d> poly1;
std::vector<Point2d> poly2;
std::vector<Line> p1;
std::vector<Line> p2;
std::vector<Point2d> new1;
std::vector<Point2d> new2;
std::vector<int> pos1;
std::vector<int> pos2;
std::vector<bool> vis1;
std::vector<bool> vis2;

inline Point2d Vector(Point2d a, Point2d b);
double dis2(Point2d a, Point2d b);
double cross(Point2d A, Point2d B, Point2d P);
double dot(Point2d A, Point2d B, Point2d P);
int dir(Point2d A, Point2d B, Point2d P);
double disLine(Point2d A, Point2d B, Point2d P);
Point2d intersection(Line u, Line v);
double dis(Point2d point1, Point2d point2);
bool isPointInsidePoly(Point2d P,const std::vector<Point2d>& polyVertices);
void getIntersections(QPainter* painter);
void prework();
void display(QPainter* painter);
void work(QPainter* painter);



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

    myDialog dialog(this,
                    "Please input position of point in order:\nIf you don't want to continue to add, click cancel.",
                    "Point_x: ",
                    "Point_y: ");

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
    radius = 0;
    pointList.clear();
    qDebug() << "circle begin";
    myDialog dialog(this,
                    "Please input position of the center of the circle and radius",
                    "Point_x of center:",
                    "Point_y of center:",
                    "Radius:");

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        /*......*/
        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
        QPoint point(dialog.point_x, dialog.point_y);
        radius = dialog.radius;
        pointList << point;
    }

    status = CIRCLE;
    update();
}

void drawingBoard::convex()
{
    status = 0;
    pointList.clear();
    pointList_cw.clear();

//    qDebug() << "convex begin";
//    myDialog dialog(this,
//                    "Please input position of point in subject polygon in order:\nIf you don't want to continue to add, click cancel.",
//                    "Point_x: ",
//                    "Point_y: ");

//    // Process when OK button is clicked
//    while (dialog.exec() == QDialog::Accepted) {
//        /*......*/
//        qDebug() << "insert:" << dialog.point_x << dialog.point_y;
//        QPoint point(dialog.point_x, dialog.point_y);
//        pointList << point;
//    }

//    myDialog dialog2(this,
//                    "Please input position of point in clipping window in order:\nIf you don't want to continue to add, click cancel.",
//                    "Point_x: ",
//                    "Point_y: ");

//    // Process when OK button is clicked
//    while (dialog2.exec() == QDialog::Accepted) {
//        /*......*/
//        qDebug() << "insert:" << dialog2.point_x << dialog2.point_y;
//        QPoint point(dialog2.point_x, dialog2.point_y);
//        pointList_cw << point;
//    }

//insert: 12 254
//insert: 371, 16
//insert: 754, 260
//insert: 399, 531
//insert: 87, 52
//insert: 76, 493
//insert: 712, 488
//insert: 701, 62

    QPoint point1(12,254);
    QPoint point2(371, 16);
    QPoint point3(754, 260);
    QPoint point4(399, 531);
    QPoint point5(87, 52);
    QPoint point6(76, 493);
    QPoint point7(712, 488);
    QPoint point8(701, 62);
    pointList << point1 << point2 << point3 << point4;
    pointList_cw << point5 << point6 << point7 << point8;

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
    {
        int off_x = pointList[0].x();
        int off_y = pointList[0].y();
        int r = radius;
        int x = 0;
        int y = r;

        float d,d0;
        d0=5.0/4.0-r;
        d=d0;
        while(x<y)
        {
            if(d>0)
            {
                d = d + 2 * (x - y) + 5;
                x += 1;
                y = y - 1;
            }
            else
            {
                d = d + 2 * x + 3;
                x += 1;
            }

            painter->drawPoint(off_x + x, off_y + y);
            painter->drawPoint(off_x - x, off_y + y);
            painter->drawPoint(off_x + x, off_y - y);
            painter->drawPoint(off_x - x, off_y - y);
            painter->drawPoint(off_x + y, off_y + x);
            painter->drawPoint(off_x + y, off_y - x);
            painter->drawPoint(off_x - y, off_y + x);
            painter->drawPoint(off_x - y, off_y - x);
        }

            break;
    }
        case CONVEX:
    {
        poly1.clear();
        poly2.clear();
        for(int i = 0; i < pointList.size(); i++){
            poly1.push_back( {(double)pointList[i].x(), (double)pointList[i].y()} );
            // painter->drawLine(pointList[i], pointList[(i + 1) % pointList.size()]);
        }
        painter->setPen(Qt::blue);
        for(int i = 0; i < pointList_cw.size(); i++){
            poly2.push_back( {(double)pointList_cw[i].x(), (double)pointList_cw[i].y()} );
            // painter->drawLine(pointList_cw[i], pointList_cw[(i + 1) % pointList.size()]);
        }

        display(painter);
        // painter->drawRect(xl,yb,(xr - xl), (yt - yb));



            break;
    }
        case CONCAVE:
    {
            break;
    }
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


inline Point2d Vector(Point2d a, Point2d b)
{  //向量ab
    return{ b.x - a.x, b.y - a.y };
}

double dis2(Point2d a, Point2d b)
{          //两点间的距离的平方
    return (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
}

double cross(Point2d A, Point2d B, Point2d P)
{  //向量的外积
    Point2d AB = Vector(A,B);
    Point2d AP = Vector(A,P);
    return AB.x*AP.y - AB.y*AP.x;
}

double dot(Point2d A, Point2d B, Point2d P)
{     //向量的内积
    Point2d AB = Vector(A,B);
    Point2d AP = Vector(A,P);
    return AB.x*AP.x + AB.y*AP.y;
}

int dir(Point2d A, Point2d B, Point2d P)
{    //点与线段方位判定
    if (cross(A, B, P) > 0)  return -1;
    else if (cross(A, B, P)<0) return 1;
    else if (dot(A, B, P) < 0) return -2;
    else if (dot(A, B, P) >= 0)
    {
        if (dis2(A, B) < dis2(A, P)) return 2;
        else return 0;
    }
    return 0;
}

double disLine(Point2d A, Point2d B, Point2d P)
{   //点P到直线AB的距离
    return fabs(cross(A, B, P)) / sqrt(dis2(A, B));
}

Point2d intersection(Line u, Line v)
{
    Point2d A1 = u.start;
    Point2d A2 = u.end;
    Point2d B1 = v.start;
    Point2d B2 = v.end;
    if (dir(A1, A2, B1)*dir(A1, A2, B2) <= 0 && dir(B1, B2, A1)*dir(B1, B2, A2) <= 0) {//判断有无交点
        double t = disLine(A1, A2, B1) / (disLine(A1, A2, B1) + disLine(A1, A2, B2));
        Point2d B1B2 = Vector(B1, B2);
        Point2d inter = { B1.x + B1B2.x*t, B1.y + B1B2.y*t };
        return {inter.x, inter.y};
    } else {
        return {-inf, -inf};
    }
}

double dis(Point2d point1, Point2d point2)
{
    return sqrt((point1.x-point2.x)*(point1.x-point2.x) + (point1.y-point2.y)*(point1.y-point2.y));
}

bool isPointInsidePoly(Point2d P,const std::vector<Point2d>& polyVertices)
{
    std::size_t vertCount = polyVertices.size();
    if (vertCount < 2)
        return false;
    Point2d tmp = P;
    for (int l = 0; l < 2; l++) {
        for (int r = 0; r < 2; r++) {
            P = tmp;
            if (l % 2) P.x += 0.001;
            else P.x -= 0.001;
            if (r % 2) P.y += 0.001;
            else P.y -= 0.001;
            bool inside = false;
            for (unsigned i = 1; i <= vertCount; ++i) {
                const Point2d &A = polyVertices[i - 1];
                const Point2d &B = polyVertices[i % vertCount];
                if ((B.y <= P.y && P.y < A.y) || (A.y <= P.y && P.y < B.y)) {
                    double t = (P.x - B.x) * (A.y - B.y) - (A.x - B.x) * (P.y - B.y);
                    if (A.y < B.y)
                        t = -t;
                    if (t < 0)
                        inside = !inside;
                }
            }
            if (inside) return inside;
        }
    }
    return false;
}

void getIntersections(QPainter* painter)
{//求出所有交点以及按照顺序存放在新数组中
    int len1 = poly1.size();//求出new1
    for (int i = 0; i < len1; i++) {
        new1.push_back(poly1[i]);
        std::vector<Point2d> tmp;
        for (auto it2 : p2) {
            Point2d p = intersection({{poly1[i].x, poly1[i].y},{poly1[(i+1)%len1].x, poly1[(i+1)%len1].y}}, it2);
            if (p.x != -inf && p.y != -inf) tmp.push_back({p.x, p.y});
        }
        sort(tmp.begin(), tmp.end(), [&](Point2d p1, Point2d p2){
            return dis(p1, poly1[i]) < dis(p2, poly1[i]);
        });
        for (auto it : tmp) new1.push_back(it);
    }

    int len2 = poly2.size();//求出new2
    for (int i = 0; i < len2; i++) {
        new2.push_back(poly2[i]);
        std::vector<Point2d> tmp;
        for (auto it2 : p1) {
            Point2d p = intersection({{poly2[i].x, poly2[i].y},{poly2[(i+1)%len2].x, poly2[(i+1)%len2].y}}, it2);
            if (p.x != -inf && p.y != -inf) tmp.push_back({p.x, p.y});
        }
        sort(tmp.begin(), tmp.end(), [&](Point2d p1, Point2d p2){
            return dis(p1, poly2[i]) < dis(p2, poly2[i]);
        });
        for (auto it : tmp) new2.push_back(it);
    }
    for (int i = 0; i < new1.size(); i++) {//映射关系，给定eps为误差范围
        for (int j = 0; j < new2.size(); j++) {
            if (fabs(new1[i].x-new2[j].x) < eps && fabs(new1[i].y-new2[j].y) < eps) {
                pos1[i] = j;
                pos2[j] = i;
            }
        }
    }
    work(painter);
}

void prework()
{
    p1.clear();
    p2.clear();
    new1.clear();
    new2.clear();
    vis1.clear();
    vis2.clear();
    pos1.clear();
    pos2.clear();
}

void display(QPainter* painter)
{
    prework();//初始化

    int len1 = poly1.size();//绘制多边形
    painter->setPen(Qt::red);
    for (int i = 0; i < len1; i++) {
        QPoint point1(poly1[i].x, poly1[i].y);
        QPoint point2(poly1[(i+1)%len1].x, poly1[(i+1)%len1].y);
        painter->drawLine(point1, point2);
        // glVertex2f(poly1[i].x, poly1[i].y);
        // glVertex2f(poly1[(i+1)%len1].x, poly1[(i+1)%len1].y);
        p1.push_back({{poly1[i].x, poly1[i].y}, {poly1[(i+1)%len1].x, poly1[(i+1)%len1].y}});
    }
    int len2 = poly2.size();
    painter->setPen(Qt::blue);
    for (int i = 0; i < len2; i++) {
        QPoint point1(poly2[i].x, poly2[i].y);
        QPoint point2(poly2[(i+1)%len1].x, poly2[(i+1)%len1].y);
        painter->drawLine(point1, point2);

        // glVertex2f(poly2[i].x, poly2[i].y);
        // glVertex2f(poly2[(i+1)%len2].x, poly2[(i+1)%len2].y);
        p2.push_back({{poly2[i].x, poly2[i].y}, {poly2[(i+1)%len2].x, poly2[(i+1)%len2].y}});
    }
    painter->setPen(Qt::black);
    getIntersections(painter);
}

void work(QPainter* painter) {
    std::vector<Point2d> now;//当前选择到的点
    int len1 = new1.size();
    int len2 = new2.size();
    for (int i = 0; i < new1.size(); i++) {//new1 第一个新多边形 new2第一个新多边形
        if (vis1[i]) continue;
        int ch = 1, nowpos = i;
        while (1) {
            if (ch == 1) {//遍历第一个多边形
                if (isPointInsidePoly(new1[nowpos], poly2)) now.push_back(new1[nowpos]);
                if (vis1[nowpos]) {//如果该点遍历过

                    for (int j = 0; j < now.size(); j++) {//绘制交多边形
                        QPoint point1(now[j].x, now[j].y);
                        QPoint point2(now[(j+1)%now.size()].x, now[(j+1)%now.size()].y);
                        painter->drawLine(point1, point2);

                        // glVertex2f(now[j].x, now[j].y);
                        // glVertex2f(now[(j+1)%now.size()].x, now[(j+1)%now.size()].y);
                    }
                    now.clear();
                    break;
                }
                vis1[nowpos] = true;//给当前经历点打上标记
                if (isPointInsidePoly(new1[nowpos], poly2) && !isPointInsidePoly(new1[(nowpos+1)%len1], poly2)) {//判断是否为出点
                    ch = 2;
                    nowpos = pos1[nowpos];
                    nowpos = (nowpos + 1) % len2;
                } else {
                    nowpos = (nowpos + 1) % len1;
                }
            } else {//遍历第二个多边形
                if (isPointInsidePoly(new2[nowpos], poly1)) now.push_back(new2[nowpos]);
                if (vis2[nowpos]) {//如果该点遍历过

                    for (int j = 0; j < now.size(); j++) {//绘制交多边形
                        QPoint point1(now[j].x, now[j].y);
                        QPoint point2(now[(j+1)%now.size()].x, now[(j+1)%now.size()].y);
                        painter->drawLine(point1, point2);

                        // glVertex2f(now[j].x, now[j].y);
                        // glVertex2f(now[(j+1)%now.size()].x, now[(j+1)%now.size()].y);
                    }
                    now.clear();
                    break;
                }
                vis2[nowpos] = true;//给当前点打上标记
                if (isPointInsidePoly(new2[nowpos], poly1) && !isPointInsidePoly(new2[(nowpos+1)%len2], poly1)) {//判断是否为出点
                    ch = 1;
                    nowpos = pos2[nowpos];
                    nowpos = (nowpos + 1) % len1;
                } else {
                    nowpos = (nowpos + 1) % len2;
                }
            }
        }
    }
}


