#include "inputread.h"


InputReader::InputReader() : mIsDistanceMatrixCreated(false), mDistanceMatrix(0)
{
}


void InputReader::initializeDistanceMatrix() {
    mDistanceMatrix.resize(mCityCountInRow);
    for(int i=0;i<mCityCountInRow;i++)
        mDistanceMatrix[i].resize(mCityCountInRow);
    mIsDistanceMatrixCreated = true;
}

void InputReader::processFileWithCitiesCoordinates(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw "File could not be opened";
    }

    mCitiesCoordinates.resize(mCityCountInColumn);

    int i = 0;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        QList<QByteArray> lista = line.split(' ');
        if(lista.size()==2){
            mCitiesCoordinates[i].x = (lista[0].trimmed()).toInt();
            mCitiesCoordinates[i].y = (lista[1].trimmed()).toInt();
            i++;
        }
        else{
            throw "Number of entries in one of the rows is not proper";
        }
        if(i>mCityCountInColumn){
            throw "thrown an exception, becasue list is too long";
        }
    }
    if(i<mCityCountInColumn) {
        throw "thrown an exception, list was too short";
    }
}


void InputReader::validateDistanceMatrix() {
    if(this->mCityCountInColumn != this->mCityCountInRow) {
        throw "Distance matrix should be a square!";
    }

    for(int i=0 ; i < mCityCountInRow ; i++) {
        if(mDistanceMatrix[i][i] != 0) {
            throw "All values on diagonal must me zeroes!";
        }
    }
}

void InputReader::processFileWithCitiesDistances(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //File could not be successuflly opened. Need to report an bug
        throw "File could not be opened!";
    }
    mCityCountInColumn = 0;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        line = line.trimmed();
        QList<QByteArray> singleRow = line.split(' ');
        if(mDistanceMatrix.isEmpty()) {
            mCityCountInRow = singleRow.size();
            initializeDistanceMatrix();
        }
        else {
            //check if cityCount is the same
            if(mCityCountInRow != singleRow.size()) {
                throw -1;
            }
        }
        for(int i=0;i<mCityCountInRow;i++)
            mDistanceMatrix[mCityCountInColumn][i]=(singleRow[i].trimmed()).toInt();
        mCityCountInColumn++;
    }
    this->validateDistanceMatrix();
}
