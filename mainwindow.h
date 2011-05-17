#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include "draw.h"
#include "inputread.h"
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


private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Draw *draw;
    void processFile(QString s);
    void processFileWithCoordinates(QString s);
    int **mDistances;
    int mCitiesCount;
    void computeRoads();
    struct RoadPart *mRoads;
    int mRoadsCount;
    struct CityPosition *mCityPositions;
    InputReader mDataReader;
};

#endif // MAINWINDOW_H
