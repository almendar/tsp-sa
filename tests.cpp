#include "inputread.h"
#include "tspsolver.h"
#include <QTextStream>
#include <QDebug>

int inputReadTest() {
    QString distanceFile("D:\\tomek\\programowanie\\qt-creator\\tsp-sa\\input1.txt");
    QString coordinatesFile("D:\\tomek\\programowanie\\qt-creator\\tsp-sa\\input1wsp4dobre.txt");
    InputReader reader;
    reader.processFileWithCitiesDistances(distanceFile);
    reader.processFileWithCitiesCoordinates(coordinatesFile);
    QFile file("inputReadTest.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return -2;
    }
    QTextStream out(&file);
    for(int i=0; i < reader.getCityCount(); i++) {
        for(int j=0; j < reader.getCityCount(); j++) {
            out << reader.getDistanceMatrix()[i][j];
        }
        out << "\n";
    }
    out << "\n\n";
    for(int k=0; k < reader.getCityCount(); k++) {

        out << reader.getCitiesPositions()[k].x <<","<< reader.getCitiesPositions()[k].y << "\n";
    };

    file.close();
    return 0;
}


int generatingInitialRouteTest() {
    QString distanceFile("D:\\tomek\\programowanie\\qt-creator\\tsp-sa\\input1.txt");
    QString coordinatesFile("D:\\tomek\\programowanie\\qt-creator\\tsp-sa\\input1wsp4dobre.txt");
    InputReader reader;
    reader.processFileWithCitiesDistances(distanceFile);
    reader.processFileWithCitiesCoordinates(coordinatesFile);
    TSPSolver* solver = new TSPSolver(reader.getDistanceMatrix(),reader.getCityCount(),0.4f);
    solver->generateStartingRoute();
    QVector<QVector<int> >& rut = solver->getRoute();
    for(int i=0; i < rut.size(); i++) {
        for(int k=0;k<rut[i].size();k++){
            qDebug() << rut[i][k];
        }
    }
    delete solver;
    return 0;
}
