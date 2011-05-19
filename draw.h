#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "inputread.h"
class RenderArea;



class Draw : public QWidget
{
    Q_OBJECT
public:
    explicit Draw(QWidget *parent = 0);
    ~Draw();


protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);


signals:

public slots:
    void paintCities(int number,QVector<CityPosition> wsp);
    void paintRoads(struct RoadPart *drogi, int liczbaDrog);

private:
    QRect qr;
    CityPosition *cities;
    struct RoadPart *roads;
    int roadsCount;
    int citiesCount;
};

#endif // DRAW_H
