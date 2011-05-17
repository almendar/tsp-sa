#ifndef INPUTREAD_H
#define INPUTREAD_H
#include <QString>
#include <QFile>
#include <QIODevice>
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
    int** getDistanceMatrix() { return mDistanceMatrix;};
    int getCityCount() {return mCityCountInRow;};
    CityPosition* getCitiesPositions() {return mCitiesCoordinates;};

private:
    //Variables
    bool mIsDistanceMatrixCreated;
    int mCityCountInRow;
    int mCityCountInColumn;
    int** mDistanceMatrix;
    CityPosition* mCitiesCoordinates;

    //Methods
    void initializeDistanceMatrix();
    void validateDistanceMatrix();

};

#endif // INPUTREAD_H
