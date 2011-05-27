#ifndef INPUTREAD_H
#define INPUTREAD_H
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QVector>
#include <QPoint>
#include <QTime>
class CityPosition{
public:
    int x;
    int y;
};


struct RoadPart{
    int a;
    int b;
};

class InputReader
{
public:
    InputReader();
    void processFileWithCitiesDistances(QString fileName);
    void processFileWithCitiesCoordinates(QString fileName);
    void generateCoordinates();
    QVector<QVector<float> >& getDistanceMatrix() { return mDistanceMatrix;};
    int getCityCount() {return mCityCountInRow;};
    QVector<CityPosition> getCitiesPositions() {return mCitiesCoordinates;};

private:
    //Variables
    bool mIsDistanceMatrixCreated;
    int mCityCountInRow;
    int mCityCountInColumn;
    QVector<QVector<float> > mDistanceMatrix;
    QVector<CityPosition> mCitiesCoordinates;

    //Methods
    void initializeDistanceMatrix();
    void validateDistanceMatrix();

};

#endif // INPUTREAD_H
