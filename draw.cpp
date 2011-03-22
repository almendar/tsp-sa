#include "draw.h"

Draw::Draw(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(200,400);
    this->qr.setRect(1,1,30,46);
}


void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
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
