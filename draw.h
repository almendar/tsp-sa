#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
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


private:
    QRect qr;
};

#endif // DRAW_H
