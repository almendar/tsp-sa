#include "draw.h"

Draw::Draw(QWidget *parent) :
    QWidget(parent)
{
    //this->setMinimumSize(200,400);
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(size_policy);
    this->qr.setRect(1,1,30,46);
}


void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    int height = size().height();
    int width = size().width();
    qp.drawRect(1,1,width-3,height-3);
    qp.drawRect(this->qr);
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
