#include "inputread.h"


InputReader::InputReader() : mIsDistanceMatrixCreated(false), mDistanceMatrix(0)
{
}


void InputReader::initializeDistanceMatrix() {
    mDistanceMatrix=new int*[mCityCountInRow];
    for(int i=0;i<mCityCountInRow;i++)
        mDistanceMatrix[i]=new int[mCityCountInRow];
}

void InputReader::processFileWithCitiesCoordinates(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //throw an exception
        return;
    }

    mCitiesCoordinates = new CityPosition[mCityCountInColumn];

    int i = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> lista = line.split(' ');
        if(lista.size()==2){
            mCitiesCoordinates[i].x = (lista[0].trimmed()).toInt();
            mCitiesCoordinates[i].y = (lista[1].trimmed()).toInt();
            i++;
        }
        else{
            //throw an exception
        }
        if(i>mCityCountInColumn){
            //throw an exception, becasue list is too long
        }
    }
    if(i<mCityCountInColumn) {
        //throw an exception, list was too short
    }
}


void InputReader::validateDistanceMatrix() {
    if(this->mCityCountInColumn != this->mCityCountInRow) {
        //throw an exception
    }

    for(int i=0 ; i < mCityCountInRow ; i++) {
        if(mDistanceMatrix[i][i] != 0) {
            //throw an exception
        }
    }
}

void InputReader::processFileWithCitiesDistances(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //File could not be successuflly opened. Need to report an bug
        return;
    }
    mCityCountInColumn = 0;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QList<QByteArray> singleRow = line.split(' ');
        if(mDistanceMatrix == 0) {
            mCityCountInRow = singleRow.size();
            initializeDistanceMatrix();
        }
        else {
            //check if cityCount is the same
            if(mCityCountInRow != singleRow.size()) {
                //Throw an exception
            }
        }
        for(int i=0;i<mCityCountInRow;i++)
            mDistanceMatrix[mCityCountInColumn][i]=(singleRow[i].trimmed()).toInt();
        mCityCountInColumn++;
    }
    this->validateDistanceMatrix();
}
