#ifndef TSPSOLVER_H
#define TSPSOLVER_H
#include <QStack>
#include <QLinkedList>
#include <QTime>
#include <QDebug>
#include <QVector>
class TSPSolver : public QObject
{
    Q_OBJECT
public:

    TSPSolver(QVector<QVector<int> >& adjastancyMatrix, int nrOfCities,
              float initTemperature=0.5);
    ~TSPSolver();
    void generateStartingRoute();
    QVector< QVector<int> > twoOpt(QVector< QVector<int> > route);
    void startSimulatedAnnealing();
    QVector<QVector<int> >& getRoute(){return mRoute;}
    void sendRoute();

signals:
    void newRouteComputed(QVector<QVector<int> >);
    void finalRouteComputed();

private:
    int mRandomStartPoint;
    int findNearestNeighbour(const int& vertexNr);
    int findNearestUnexploredVertex(const int& vertexNr);
    void findWayBackToStart(int startOfTheRoute, int lastVisitedNode);
    int routeLength(QVector<QVector<int> > &adjacencyMatrix, QVector< QVector<int> > &route);
    QVector<QVector<int> > mAdjacencyMatrix;
    int mNrOfCities;
    float mInitTemperature;
    QVector<bool> mVisitedNodes;
    QVector< QVector<int> > mRoute;

};

#endif // TSPSOLVER_H
