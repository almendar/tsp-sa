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
    mCitiesCount = 0;
    mRoads = NULL;
    mRouteSolver = 0;
    mRoadsCount = 0;

    connect(ui->btn_BrowseCityDistance,SIGNAL(clicked()),SLOT(browseForDistanceFile()));
    connect(ui->btn_BrowseCityPosition,SIGNAL(clicked()),SLOT(browseForCoordinatesFile()));
    connect(ui->btn_Compute,SIGNAL(clicked()),SLOT(computeRoute()));
    connect(ui->btn_LoadInputData,SIGNAL(clicked()),SLOT(loadDataFiles()));
    connect(ui->btn_Compute,SIGNAL(clicked()),SLOT(computeRoute()));
    connect(this,SIGNAL(cityPositionRead(QVector<CityPosition>)),draw,SLOT(setCities(QVector<CityPosition>)));
    //connect(this,SIGNAL(cityRouteComput(QVector<QVector<int> >)),draw,SLOT(setRoute(QVector<QVector<int> >)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if(mRouteSolver != 0)
        delete mRouteSolver;
}

void MainWindow::browseForDistanceFile()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik", "wejscie.txt");
    this->ui->lineEdit->setText(s);
    //processFile(s);
}

void MainWindow::loadDataFiles() {
    QString distanceFile = this->ui->lineEdit->text();
    if(!QFile::exists(distanceFile)) {
        this->showWarning("Distance file does not exists!");
        return;
    }
    QString positionFile = this->ui->lineEdit_2->text();
    if(!QFile::exists(positionFile)) {
        this->showWarning("Position file does not exists!");
        return;
    }
    try {
        mDataReader.processFileWithCitiesDistances(distanceFile);
        mDataReader.processFileWithCitiesCoordinates(positionFile);
    }
    catch(const char* ex) {
        QMessageBox msgBox;
        msgBox.setText(ex);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    catch(...) {
        QMessageBox msgBox;
        msgBox.setText("Unknown error occured!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    emit cityPositionRead(mDataReader.getCitiesPositions());
}

void MainWindow::computeRoute()
{
    mComputeAction = new ComputeThread(this);
    mRouteSolver = new TSPSolver(mDataReader.getDistanceMatrix(), mDataReader.getCityCount());
    qRegisterMetaType<QVector<QVector<int> > >("QVector<QVector<int> >");
    connect(mRouteSolver,SIGNAL(newRouteComputed(QVector<QVector<int> >)),draw,SLOT(setRoute(QVector<QVector<int> >)));
    connect(mRouteSolver,SIGNAL(finalRouteComputed()),draw,SLOT(computationFinished()));
    mComputeAction->setSolver(mRouteSolver);
    mComputeAction->start();
//    mRouteSolver->generateStartingRoute();
//    QVector<QVector<int> >& roads =  mRouteSolver->getRoute();
//    emit cityRouteComput(roads);
}

void MainWindow::showWarning(const char* text){
    QMessageBox msg(this);
    msg.setText(text);
    msg.setIcon(QMessageBox::Warning);
    msg.exec();
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

void MainWindow::browseForCoordinatesFile()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik wspó³rzêdnych", "wspolrzedne.txt");
    this->ui->lineEdit_2->setText(s);
    //processFileWithCoordinates(s);
}



ComputeThread::ComputeThread(MainWindow *mw) {
    mMainWindow = mw;
}

/*
void ComputeThread::ComputeThread(QWidget *parent) {

}
*/
void ComputeThread::run() {
    mSolver->generateStartingRoute();
    mSolver->sendRoute();
}

void ComputeThread::setSolver(TSPSolver* solver) {
    mSolver = solver;
}
