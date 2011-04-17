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
    dane = NULL;
    size = 0;
    drogi = NULL;
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

    draw->paintCities(size);
    computeRoads();
    draw->paintRoads(drogi);
}

void MainWindow::computeRoads(){
    int liczbaDrog = size*(size-1); // graf skierowany
    if(drogi!=NULL)
        delete [] drogi;
    drogi = new bool[liczbaDrog];
    for(int i=0;i<liczbaDrog;i++){
        drogi[i]=rand()%2;
    }
}

void MainWindow::processFile(QString s){
    QMessageBox msgBox;
    QFile file(s);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        msgBox.setText("Blad podczas otwierania pliku!");
        msgBox.exec();
        return;
    }
    int rozmiar;
    int** tab;
    int iter=0;
    if(!file.atEnd()){
        QByteArray line = file.readLine();
        QList<QByteArray> lista = line.split(' ');
        rozmiar = lista.size();
//        msgBox.setText("Ilosc pol: "+QString::number(rozmiar));
//        msgBox.exec();
        tab=new int *[rozmiar];
        for(int i=0;i<rozmiar;i++)
            tab[i]=new int[rozmiar];
        for(int i=0;i<rozmiar;i++)
            tab[iter][i]=(lista[i]).toInt();
        iter++;
    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> lista = line.split(' ');
        for(int i=0;i<rozmiar;i++)
            tab[iter][i]=(lista[i]).toInt();
        iter++;
//        msgBox.setText(line);
//        msgBox.exec();
    }
    if(iter==rozmiar){
//        msgBox.setText("Wczytano plik");
//        msgBox.exec();
        if(dane!=NULL){
            for(int i=0;i<size;i++)
                delete [] dane[i];
            delete [] dane;
        }
        dane = tab;
        size = rozmiar;
//        QString text = "Dane:\n";
//        for(int i=0;i<rozmiar;i++){
//            for(int j=0;j<rozmiar;j++)
//                text+=QString::number(tab[i][j]);
//            text+="\n";
//        }
//        msgBox.setText(text);
//        msgBox.exec();
    }
    else{
        msgBox.setText("Blad podczas wczytywania pliku\nIlosc wierszy nierowna ilosci kolumn");
        msgBox.exec();
    }
}
