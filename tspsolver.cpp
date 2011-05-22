#include "tspsolver.h"
#include <QtCore/qmath.h>



TSPSolver::TSPSolver(QVector<QVector<int> >& adjacencyMatrix,
                     int nrOfCities, float initTemperature):
mAdjacencyMatrix(adjacencyMatrix), mNrOfCities(nrOfCities), mInitTemperature(initTemperature)
{
    //QObject::QObject(0);
    mVisitedNodes.resize(nrOfCities);
    for(int i=0; i < nrOfCities;i++) {mVisitedNodes[i] = false;};
    mRoute.resize(nrOfCities);
}

TSPSolver::~TSPSolver() {
}

void TSPSolver::generateStartingRoute() {
    //Start from random City
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int startVertexNr = (qrand() % mNrOfCities);
    mRandomStartPoint = startVertexNr;
    int notVisitedVerticesNumber = mNrOfCities;
    int currentVertexNr = startVertexNr;
    while(notVisitedVerticesNumber > 0) {
        qDebug() << "Left cities:" <<notVisitedVerticesNumber;
        int bestNeighbour = findNearestNeighbour(currentVertexNr);
        //Check if all neighbours connected to current node were visited?
        if(bestNeighbour == -1) {
            //if there are more vertices look for them.
            if(notVisitedVerticesNumber>1) {
                mVisitedNodes[currentVertexNr] = true;
                int unexplored = findNearestUnexploredVertex(currentVertexNr);
                currentVertexNr = unexplored;
                --notVisitedVerticesNumber;
            }
            else if(notVisitedVerticesNumber == 1 //&& !mVisitedNodes[currentVertexNr]
                    ) {
                //current vertex is the last one.
                --notVisitedVerticesNumber;
                mVisitedNodes[currentVertexNr] = true;
            }
        }
        else {
            mRoute[currentVertexNr].append(bestNeighbour);
            mVisitedNodes[currentVertexNr] = true;
            currentVertexNr = bestNeighbour;
            --notVisitedVerticesNumber;
        }
    }
    findWayBackToStart(startVertexNr,currentVertexNr);

}

/**
  Finds nearest neighbour that was:
  - not visited yet
  - distance to is greater than zero.
  If no neighbour meets requiremnt return -1.

  */
int TSPSolver::findNearestNeighbour(const int& vertexNr) {
    QVector<int>& currentVertexNeighboursArray = mAdjacencyMatrix[vertexNr];
    int tmpNearestNeighbourBestDistance = -1;
    int tmpBestNeighbourNumber = -1;
    for(int i=0; i < mNrOfCities; i++) {
        int currentDistanceChecked = currentVertexNeighboursArray[i];
        if(!mVisitedNodes[i] && currentDistanceChecked > 0 && tmpNearestNeighbourBestDistance <= currentDistanceChecked) {
            tmpBestNeighbourNumber = i;
            tmpNearestNeighbourBestDistance = currentDistanceChecked;
        }
    }
    return tmpBestNeighbourNumber;
}

/**
Finds nearest vertex that was not explored yet. Direct connection is not needed.
Some vertices on the way can be visited more than once.
This method changes object state.
Based od DFS algorithm
*/
int TSPSolver::findNearestUnexploredVertex(const int& vertexNr) {
    QVector<bool> visited;
    visited.resize(mNrOfCities);
    for(int i=0;i<mNrOfCities;i++) {visited[i]=false;};
    QVector<QStack<int> > neighboursToSee;
    neighboursToSee.resize(mNrOfCities);
    QStack<int> route;
    int currentVertex = vertexNr;
    route.push(currentVertex);
    bool stopSearch = false;
    while(true) {
        QVector<int>& neighboursRow = mAdjacencyMatrix[currentVertex];
        if(!visited[currentVertex]) {
            for(int k=0;k<mNrOfCities;k++) {
                //if we found unvisited node in TSP, we can stop searching
                if(neighboursRow[k]>0 && !mVisitedNodes[k]) {
                    route.push(k);
                    stopSearch = true;
                    break;
                }
                //add all neighbours that were not visited, and there is a connection
                else if(!visited[k] && neighboursRow[k] > 0) {
                    neighboursToSee[currentVertex].push(k);
                }
            }
            visited[currentVertex] = true;
            if(stopSearch)
                break;
        }

        if(!neighboursToSee[currentVertex].isEmpty()) {
            //Take vertex, set is as the current and start all over again
            int neighbourVertex = neighboursToSee[currentVertex].pop();
            route.push(neighbourVertex);
            currentVertex = neighbourVertex;
        }
        else {
            //dead end, remove from the stack

            route.pop();

            //go back to previous vertex and start again
            currentVertex = route.top();
            /*this should only happen if we have already been at this
              vertex */
            Q_ASSERT(visited[currentVertex]);
        }
    }
    int destinationVertex = route.pop();
    int end = destinationVertex;
    int start = -1;
    while(!route.isEmpty()) {
        start = route.pop();
        mRoute[start].append(end);
        end = start;

        if(end!=destinationVertex) {
            Q_ASSERT(mVisitedNodes[end] == true);
        }
    }
    mVisitedNodes[destinationVertex] = true;
    return destinationVertex;
}

void TSPSolver::findWayBackToStart(int startOfTheRoute, int lastVisitedNode) {
    bool visited[mNrOfCities];
    for(int i=0;i<mNrOfCities;i++) {visited[i]=false;};
    QStack<int> neighboursToSee[mNrOfCities];
    QStack<int> route;

    //Nothing to do!
    if(startOfTheRoute == lastVisitedNode) {
        return;
    }

    int currentVertex = lastVisitedNode;
    route.push(currentVertex);
    while(true) {
        QVector<int>& neighboursRow = mAdjacencyMatrix[currentVertex];
        if(!visited[currentVertex]) {
            for(int k=0;k<mNrOfCities;k++) {
                if(!visited[k] && neighboursRow[k] > 0) {
                    neighboursToSee[currentVertex].push(k);
                }
            }
            visited[currentVertex] = true;
        }

        if(!neighboursToSee[currentVertex].isEmpty()) {
            //Take vertex, set is as the current and start all over again
            int neighbourVertex = neighboursToSee[currentVertex].pop();
            route.push(neighbourVertex);
            currentVertex = neighbourVertex;
            if(currentVertex == startOfTheRoute) {
                //We found what we were looking for.
                break;
            }
        }
        else {
            //dead end, remove from the stack
            route.pop();
            //go back to previous vertex and start again
            currentVertex = route.top();
            /*this should only happen if we have already been at this
              vertex */
            Q_ASSERT(visited[currentVertex]);
        }
    }
    int destinationVertex = route.pop();
    int end = destinationVertex;
    int start = -1;
    while(!route.isEmpty()) {
        start = route.pop();
        mRoute[start].append(end);
        end = start;

        if(end!=destinationVertex) {
            Q_ASSERT(mVisitedNodes[end] == true);
        }
    }
}

QVector< QVector<int> > TSPSolver::twoOpt(QVector< QVector<int> > route){
    int segmentsCount = 0;
    for(int i=0; i<route.size(); i++)
        segmentsCount += route[i].size();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    int firstSegment = (qrand() % segmentsCount);
    int secondSegment = (qrand() % (segmentsCount-1));
    if(secondSegment >= firstSegment)
        secondSegment++;

    int x1=0, x2=0, y1=0, y2=0;
    int fS = firstSegment, sS = secondSegment;
    bool ready1 = false, ready2=false;
    int i=0;
    while(!ready1 || !ready2){
        if(!ready1){
            if(route[i].size() <= fS){
                fS -= route[i].size();
                x1++;
            }else{
                y1=fS;
                ready1 = true;
            }
        }
        if(!ready2){
            if(route[i].size() <= sS){
                sS -= route[i].size();
                x2++;
            }else{
                y2=sS;
                ready2 = true;
            }
        }
        i++;
    }

    int temp = route[x2][y2];
    route[x2][y2]=route[x1][y1];
    route[x1][y1]=temp;

    return route;
}

void TSPSolver::startSimulatedAnnealing(){
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QVector< QVector<int> > bestRoute = mRoute;
    QVector< QVector<int> > actualRoute = mRoute;
    int bestLength = routeLength(mAdjacencyMatrix, mRoute);
    int actualLength = routeLength(mAdjacencyMatrix, actualRoute);
    float actualTemperature = mInitTemperature;
    actualTemperature = 10.0;
    while(actualTemperature>0.0){
        QVector< QVector<int> > newRoute = twoOpt(actualRoute);
        int newLength = routeLength(mAdjacencyMatrix, newRoute);
        if(newLength<=actualLength){
            actualRoute = newRoute;
            actualLength = routeLength(mAdjacencyMatrix, actualRoute);
            if(actualLength<bestLength){
                bestRoute = actualRoute;
                bestLength = routeLength(mAdjacencyMatrix, mRoute);
            }
        }else{
            int rand = qrand();
            double drand = rand/ (static_cast<double>(RAND_MAX));
            double expw = (newLength-actualLength)*actualTemperature;
            double exp = qExp(expw);
            if(drand<exp){
                actualRoute = newRoute;
            }
        }
        // wys³aæ aktualn¹ trasê
        actualTemperature-=1.0;
    }
    // wys³aæ najlepsz¹ trasê
}

int TSPSolver::routeLength(QVector<QVector<int> > &adjacencyMatrix, QVector< QVector<int> > &route){
    int sum = 0;
    for(int i=0;i<route.size();i++){
        for(int j=0;j<route[i].size();j++){
            sum += adjacencyMatrix[i][route[i][j]];
        }
    }
    return sum;
}

void TSPSolver::sendRoute(){
    QVector<QVector<int> >& route =  getRoute();
    emit newRouteComputed(route);
}
