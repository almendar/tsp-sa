#include "draw.h"

Draw::Draw(QWidget *parent) :
    QWidget(parent),mXMax(INT_MIN),mYMax(INT_MIN)
{
    this->setMinimumSize(200,400);
    QSizePolicy size_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setSizePolicy(size_policy);
    this->qr.setRect(1,1,30,46);
    cities = NULL;
    citiesCount = 0;
    roads = NULL;


}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);
    int height = size().height();
    int width = size().width();
    qp.drawRect(1,1,width-3,height-3);
    paintCities();
}


void Draw::paintCities() {
    QPainter qp(this);
    int height = size().height();
    int width = size().width();

    if(!mCityPositions.isEmpty()) {
        for(int i=0; i < mCityPositions.size(); i++) {
            double x = mCityPositions[i].x;
            double y = mCityPositions[i].y;
            int xScaled = (int)((x/(mXMax+8))*width);
            int yScaled = (int)((y/(mYMax+8))*height);
            mScaledCityPositions[i].x = xScaled;
            mScaledCityPositions[i].x = yScaled;
            qp.drawRect(xScaled,yScaled,3,3);
            qp.drawText(xScaled-8,yScaled-8,30,20,0,QString::number(i+1));
        }
    }
}

void Draw::paintRoute() {
    QPainter qp(this);
    if(mRoutes.isEmpty()) {

    }
}

Draw::~Draw()
{
}

void Draw::setCities(QVector<CityPosition> cityPositions) {
    mCityPositions = cityPositions;
    mScaledCityPositions.resize(mCityPositions.size());
    for(int i=0; i < cityPositions.size();i++) {
        if(cityPositions[i].x > mXMax)
            mXMax = cityPositions[i].x;
        if(cityPositions[i].y > mYMax)
            mYMax = cityPositions[i].y;
    }
    update();
}

void Draw::setRoute(QVector<QVector<int> > routes) {
    mRoutes = routes;
    update();
}
