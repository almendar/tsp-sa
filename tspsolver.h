#ifndef TSPSOLVER_H
#define TSPSOLVER_H
#include <QStack>
#include <QLinkedList>
#include <QTime>
class TSPSolver
{
public:
    TSPSolver(QVector<QVector<int> >& adjastancyMatrix, int nrOfCities,
              float initTemperature=0.5);
    ~TSPSolver();
    void generateStartingRoute();
    QVector<QVector<int> >& getRoute(){return mRoute;};

private:
    int mRandomStartPoint;
    int findNearestNeighbour(const int& vertexNr);
    int findNearestUnexploredVertex(const int& vertexNr);
    void findWayBackToStart(int startOfTheRoute, int lastVisitedNode);
    QVector<QVector<int> > mAdjacencyMatrix;
    int mNrOfCities;
    float mInitTemperature;
    QVector<bool> mVisitedNodes;
    QVector< QVector<int> > mRoute;


};

#endif // TSPSOLVER_H
