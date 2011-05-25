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
}

void MainWindow::loadDataFiles() {
    QString distanceFile = this->ui->lineEdit->text();
    if(!QFile::exists(distanceFile)) {
        this->showWarning("Distance file does not exists!");
        return;
    }
    QString positionFile = this->ui->lineEdit_2->text();
    bool generateRandom = false;
    if(!QFile::exists(positionFile)) {
        this->showWarning("Position file does not exists!\nGenerating random positions");
        generateRandom = true;
    }
    try {
        mDataReader.processFileWithCitiesDistances(distanceFile);
        if(generateRandom){
            mDataReader.generateCoordinates();
        }else{
            mDataReader.processFileWithCitiesCoordinates(positionFile);
        }
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
    mRouteSolver = new TSPSolver(mDataReader.getDistanceMatrix(), mDataReader.getCityCount(), this->ui->doubleSpinBox_2->value(), this->ui->doubleSpinBox->value(), this->ui->doubleSpinBox_3->value(), this->ui->doubleSpinBox_4->value());
    qRegisterMetaType<QVector<QVector<int> > >("QVector<QVector<int> >");
    connect(mRouteSolver,SIGNAL(newRouteComputed(QVector<QVector<int> >)),draw,SLOT(setRoute(QVector<QVector<int> >)));
    connect(mRouteSolver,SIGNAL(finalRouteComputed(int)),draw,SLOT(computationFinished(int)));
    connect(mRouteSolver,SIGNAL(finalRouteComputed(int)),this,SLOT(computationFinished(int)));
    connect(mRouteSolver,SIGNAL(generatingRouteError()),this,SLOT(generatingRouteError()));
    mComputeAction->setSolver(mRouteSolver);
    mComputeAction->start();
}

void MainWindow::showWarning(const char* text){
    QMessageBox msg(this);
    msg.setText(text);
    msg.setIcon(QMessageBox::Warning);
    msg.exec();
}

void MainWindow::browseForCoordinatesFile()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik wspó³rzêdnych", "wspolrzedne.txt");
    this->ui->lineEdit_2->setText(s);
}

void MainWindow::computationFinished(int value){
    this->ui->lineEdit_3->setText(QString::number(value));
}

void MainWindow::generatingRouteError(){
    QMessageBox msg(this);
    msg.setText("Nie uda³o siê wygenerowaæ œcie¿ki!");
    msg.setIcon(QMessageBox::Warning);
    msg.exec();
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
    mSolver->startSimulatedAnnealing();
//    mSolver->sendRoute();
}

void ComputeThread::setSolver(TSPSolver* solver) {
    mSolver = solver;
}
