#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QVector>
#include "inputread.h"
#include "draw.h"
namespace Ui {
    class MainWindow;
    class Draw;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void cityPositionRead(QVector<CityPosition> cityPositions);


private slots:
    void browseForCoordinatesFile();
    void browseForDistanceFile();
    void computeRoute();

private:
    Ui::MainWindow *ui;
    Draw *draw;
    void processFile(QString s);
    void processFileWithCoordinates(QString s);
    void showWarning(const char* msg);
    QVector<QVector<int> > mDistances;
    int mCitiesCount;
    void computeRoads();
    struct RoadPart *mRoads;
    int mRoadsCount;
    QVector<CityPosition> mCityPositions;
    InputReader mDataReader;

};

#endif // MAINWINDOW_H
