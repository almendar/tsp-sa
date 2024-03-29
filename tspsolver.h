#ifndef TSPSOLVER_H
#define TSPSOLVER_H
#include <QStack>
#include <QLinkedList>
#include <QTime>
#include <QDebug>
#include <QVector>
#include <QtCore/qmath.h>
#include <QMainWindow>
class TSPSolver : public QObject
{
    Q_OBJECT
public:

    TSPSolver(QVector<QVector<float> >& adjastancyMatrix, int nrOfCities,
              float initTemperature=0.5, float temperatureStep=0.1, float minTemperature=0.01, float mInsideLoop=1.0);
    ~TSPSolver();
    void generateStartingRoute();
    QVector< QVector<int> > twoOpt(QVector< QVector<int> > route);
    void startSimulatedAnnealing();
    QVector<QVector<int> >& getRoute(){return mRoute;}
    void sendRoute();

signals:
    void newRouteComputed(QVector<QVector<int> >);
    void finalRouteComputed(float);
    void generatingRouteError();

private:
    int mRandomStartPoint;
    int findNearestNeighbour(const int& vertexNr);
    int findNearestUnexploredVertex(const int& vertexNr);
    void findWayBackToStart(int startOfTheRoute, int lastVisitedNode);
    float routeLength(QVector<QVector<float> > &adjacencyMatrix, QVector< QVector<int> > &route);
    QVector<QVector<float> > mAdjacencyMatrix;
    int mNrOfCities;
    float mInitTemperature;
    float mTemperatureStep;
    float mMinTemperature;
    float mInsideLoop;
    QVector<bool> mVisitedNodes;
    QVector< QVector<int> > mRoute;

};

#endif // TSPSOLVER_H
