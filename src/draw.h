#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "src/dataRepresentation/map.h"
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
    void paintCities(int number,struct cityPosition *wsp);
    void paintRoads(struct roadPart *drogi, int liczbaDrog);

private:
    QRect qr;
    cityPosition *cities;
    struct roadPart *roads;
    int roadsCount;
    int citiesCount;
};

#endif // DRAW_H
