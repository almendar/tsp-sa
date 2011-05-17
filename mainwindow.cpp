#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    draw = new Draw();
    ui->gridLayout_2->addWidget(draw);
    mDistances = NULL;
    mCitiesCount = 0;
    mRoads = NULL;
    mRoadsCount = 0;
    mCityPositions = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik", "wejscie.txt");
    this->ui->lineEdit->setText(s);
    processFile(s);
}

void MainWindow::on_pushButton_2_clicked()
{
    this->ui->label->setText("Tralalal");

//    QString text = "Dane:\n";
//    QMessageBox msgBox;
//    for(int i=0;i<size;i++){
//        for(int j=0;j<size;j++)
//            text+=QString::number(dane[i][j]);
//        text+="\n";
//    }
//    msgBox.setText(text);
//    msgBox.exec();

    draw->paintCities(mCitiesCount,mCityPositions);
    computeRoads();
//    QMessageBox msgBox;
//    QString text = "Dane:\n";
//    text+=QString::number(liczbaDrog);
//    msgBox.setText(text);
//    msgBox.exec();
    draw->paintRoads(mRoads,mRoadsCount);
}

void MainWindow::computeRoads(){
    // stala droga miedzy 1 a 2
//    drogi = new struct droga[1];
//    drogi[0].a=0;
//    drogi[0].b=1;
//    liczbaDrog = 1;

//    double parameter = this->ui->doubleSpinBox->value();
//    QMessageBox msgBox;
//    QString text = "Parametr wyzarzania:\n";
//    text+=QString::number(parameter);
//    msgBox.setText(text);
//    msgBox.exec();

    // zmienne
    struct RoadPart *temp = new struct RoadPart[mCitiesCount*mCitiesCount];
    int realSize = 0;
    bool visited[mCitiesCount];
    for(int i=0;i<mCitiesCount;i++) visited[i]=false;
    int remained = mCitiesCount;
    int last;

    // inicjalizacja
    int start=0;
    last = start;

    // start
    while(remained>0){
        int minCity = start;
        for(int i=0;i<mCitiesCount;i++){
            if(minCity!=i && i!=last)
//                if(dane[start][i]<dane[start][minCity])
                    if(!visited[i]){
                        minCity = i;
                    }
        }
        temp[realSize].a = start;
        temp[realSize].b = minCity;
        realSize++;
        if(!visited[start]){
            visited[start] = true;
            remained--;
        }
        if(!visited[minCity]){
            visited[minCity] = true;
            remained--;
        }
        last = minCity;

//            QMessageBox msgBox;
//            QString text = "Dane:\n";
//            text+=QString::number(minCity);
//            msgBox.setText(text);
//            msgBox.exec();
    }
    mRoads = new struct RoadPart[realSize];
    for(int i=0;i<realSize;i++){
        mRoads[i]=temp[i];
    }
    mRoadsCount = realSize;
}

void MainWindow::processFile(QString s){
    mDataReader.processFileWithCitiesDistances(s);
    mDistances = mDataReader.getDistanceMatrix();
    mCitiesCount = mDataReader.getCityCount();
}

void MainWindow::on_pushButton_3_clicked()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik wspó³rzêdnych", "wspolrzedne.txt");
    this->ui->lineEdit_2->setText(s);
    processFileWithCoordinates(s);
}

void MainWindow::processFileWithCoordinates(QString s){
    mDataReader.processFileWithCitiesCoordinates(s);
    mCityPositions = mDataReader.getCitiesPositions();
    mCitiesCount = mDataReader.getCityCount();
    draw->paintCities(mCitiesCount,mCityPositions);
}
