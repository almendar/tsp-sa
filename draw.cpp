#include "draw.h"

Draw::Draw(QWidget *parent) :
    QWidget(parent)
{
    //this->setMinimumSize(200,400);
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(size_policy);
    this->qr.setRect(1,1,30,46);
    cities = NULL;
    citiesCount = 0;
    drogi = NULL;
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    int height = size().height();
    int width = size().width();
    qp.drawRect(1,1,width-3,height-3);
    qp.drawRect(this->qr);
    for(int i=0;i<citiesCount;i++){
//        qp.drawPoint(cities[i].x,cities[i].y);
        qp.drawRect(cities[i].x,cities[i].y,3,3);
        qp.drawText(cities[i].x-8,cities[i].y-5,30,20,0,QString::number(i+1));
    }
    if(drogi!=NULL)
        for(int i=0;i<(citiesCount*(citiesCount-1));i++){
            if(drogi[i]){
                int mA=i/citiesCount;
                int mB=i%citiesCount;
                qp.drawLine(cities[mA].x,cities[mA].y,cities[mB].x,cities[mB].y);
            }
        }
}

void Draw::mousePressEvent(QMouseEvent *event)
{
    const int x = event->x();
    const int y = event->y();
    this->qr.setRect(1,1,x,y);
    emit repaint();
}

Draw::~Draw()
{
}

void Draw::paintCities(int number){
    if(cities!=NULL)
        delete [] cities;
    cities = new cityPosition[number];
    citiesCount = number;
    for(int i=0;i<number;i++){
        cities[i].x=rand()%size().width();
        cities[i].y=rand()%size().height();
    }
    emit repaint();
}

void Draw::paintRoads(bool *drogi){
    this->drogi = drogi;
}
