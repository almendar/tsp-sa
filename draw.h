#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
class RenderArea;

class cityPosition{
public:
    int x;
    int y;
};

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
    void paintCities(int number);
    void paintRoads(bool *drogi);

private:
    QRect qr;
    cityPosition *cities;
    bool *drogi;
    int citiesCount;
};

#endif // DRAW_H
