#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "inputread.h"
#include <QDebug>
#include <QtCore/qmath.h>
class RenderArea;


class Draw : public QWidget
{
    Q_OBJECT
public:
    explicit Draw(QWidget *parent = 0);
    ~Draw();


protected:
    void paintEvent(QPaintEvent *event);


signals:

public slots:
    void setCities(QVector<CityPosition> cityPositions);
    void setRoute(QVector<QVector<int> > routes);
    void computationFinished(int);


private:

    void drawArrowLine(QPainter& painter, QPoint& from, QPoint& to);

    QRect qr;
    CityPosition *cities;
    struct RoadPart *roads;
    QVector<CityPosition> mCityPositions;
    QVector<CityPosition> mScaledCityPositions;
    QVector<QVector<int> > mRoutes;

    int roadsCount;
    int citiesCount;
    int mXMax;
    int mYMax;
    void paintCities();
    void paintRoute();
};

#endif // DRAW_H
