#include "tspsolver.h"



TSPSolver::TSPSolver(QVector<QVector<int> >& adjacencyMatrix,
                     int nrOfCities, float initTemperature, float temperatureStep, float minTemperature, float insideLoop):
mAdjacencyMatrix(adjacencyMatrix), mNrOfCities(nrOfCities), mInitTemperature(initTemperature), mTemperatureStep(temperatureStep), mMinTemperature(minTemperature), mInsideLoop(insideLoop)
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
                if(unexplored==-1){
                    mRoute.clear();
                    return;
                }
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

            if(route.size()==0){
                return -1;
            }
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

    QVector<int> path;
    path.append(mRandomStartPoint);
    int actCity = mRandomStartPoint;
    int y[route.size()];
    for(int i=0;i<route.size();i++)
         y[i]=0;
    for(int i=1;i<segmentsCount+1;i++){
        y[actCity]++;
        actCity=route[actCity][y[actCity]-1];
        path.append(actCity);
    }

    int s1,s2; //starts of segments
    s1 = (qrand() % segmentsCount);
    s2 = (qrand() % (segmentsCount-3));
    if(s1==0)
        s2+=2;
    else if(s1==1)
        s2+=3;
    else if(s1>1){
        if(s1==segmentsCount-1)
            s2+=1;
        else if(s1<segmentsCount-1 && s2>=s1-1)
            s2+=3;
    }

    if(s2<s1){ // 1 ma byæ mniejszy ni¿ 2
        int temp = s2;
        s2 = s1;
        s1 = temp;
    }

    s1+=1; // interesuje nas koniec 1 odcinka
    int temp = path[s1];
    path[s1] = path[s2];
    path[s2] = temp;

    // wyrzucenie powtórzeñ miast, optymalizacja
    for(int i=0;i<path.size()-2;i++){
        if(path[i]==path[i+1]){
            for(int j=i+1;j<path.size()-1;j++){
                path[j]=path[j+1];
            }
            path.remove(path.size()-1,1);
        }
    }

    // powrót do poprzedniej reprezentacji
    QVector< QVector<int> > result;
    for(int i=0;i<route.size();i++)
        result.append(QVector<int>());
    for(int i=1;i<path.size();i++){
        result[path[i-1]].append(path[i]);
    }

    return result;
}

void TSPSolver::startSimulatedAnnealing(){
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    if(mRoute.isEmpty()){
        emit generatingRouteError();
        return;
    }

    QVector< QVector<int> > bestRoute = mRoute;
    QVector< QVector<int> > actualRoute = mRoute;
    int bestLength = routeLength(mAdjacencyMatrix, mRoute);
    int actualLength = routeLength(mAdjacencyMatrix, actualRoute);
    float actualTemperature = mInitTemperature;
    while(actualTemperature>mMinTemperature){
        for(int i=0;i<mInsideLoop;i++){
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
        }
        // wys³aæ aktualn¹ trasê
        actualTemperature *= mTemperatureStep;
        mRoute = actualRoute;
        emit newRouteComputed(mRoute);
    }
    // wys³aæ najlepsz¹ trasê
    mRoute = bestRoute;
    emit finalRouteComputed(routeLength(mAdjacencyMatrix, mRoute));
}

int TSPSolver::routeLength(QVector<QVector<int> > &adjacencyMatrix, QVector< QVector<int> > &route){
    int sum = 0;
    for(int i=0;i<route.size();i++){
        for(int j=0;j<route[i].size();j++){
            int val = adjacencyMatrix[i][route[i][j]];

            if(val==0){
//                val=1000000;  // jeœli zliczaæ ka¿de wyst¹pienie braku drogi
                i=route.size(); // jeœli uznaæ, ¿e ka¿dy brak drogi jest tak samo z³y
                sum=2000000000;
                break;
            }

            sum += val;
        }
    }
    return sum;
}

void TSPSolver::sendRoute(){
    QVector<QVector<int> >& route =  getRoute();
    emit newRouteComputed(route);
}
