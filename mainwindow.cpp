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
    distances = NULL;
    citiesCount = 0;
    roads = NULL;
    roadsCount = 0;
    cityPositions = NULL;
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

    draw->paintCities(citiesCount,cityPositions);
    computeRoads();
//    QMessageBox msgBox;
//    QString text = "Dane:\n";
//    text+=QString::number(liczbaDrog);
//    msgBox.setText(text);
//    msgBox.exec();
    draw->paintRoads(roads,roadsCount);
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
    struct roadPart *temp = new struct roadPart[citiesCount*citiesCount];
    int realSize = 0;
    bool visited[citiesCount];
    for(int i=0;i<citiesCount;i++) visited[i]=false;
    int remained = citiesCount;
    int last;

    // inicjalizacja
    int start=0;
    last = start;

    // start
    while(remained>0){
        int minCity = start;
        for(int i=0;i<citiesCount;i++){
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
    roads = new struct roadPart[realSize];
    for(int i=0;i<realSize;i++){
        roads[i]=temp[i];
    }
    roadsCount = realSize;
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
            tab[iter][i]=(lista[i].trimmed()).toInt();
        iter++;
    }
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> lista = line.split(' ');
        for(int i=0;i<rozmiar;i++)
            tab[iter][i]=(lista[i].trimmed()).toInt();
        iter++;
//        msgBox.setText(line);
//        msgBox.exec();
    }
    if(iter==rozmiar){
//        msgBox.setText("Wczytano plik");
//        msgBox.exec();
        if(distances!=NULL){
            for(int i=0;i<citiesCount;i++)
                delete [] distances[i];
            delete [] distances;
        }
        distances = tab;
        citiesCount = rozmiar;
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

void MainWindow::on_pushButton_3_clicked()
{
    QString s = QFileDialog::getOpenFileName(this, "Wybierz plik wspó³rzêdnych", "wspolrzedne.txt");
    this->ui->lineEdit_2->setText(s);
    processFileWithCoordinates(s);
}

void MainWindow::processFileWithCoordinates(QString s){
    QMessageBox msgBox;
    QFile file(s);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        msgBox.setText("Blad podczas otwierania pliku!");
        msgBox.exec();
        return;
    }
    struct cityPosition *temp;
    temp = new cityPosition[citiesCount];

    int i = 0;
    bool error = false;
    while (!file.atEnd() && !error) {
        QByteArray line = file.readLine();
        QList<QByteArray> lista = line.split(' ');
        if(lista.size()==2){
            temp[i].x = (lista[0].trimmed()).toInt();
            temp[i].y = (lista[1].trimmed()).toInt();
            i++;
        }else{
            error = true;
        }
        if(i>citiesCount){
            error = true;
        }
    }
    if(i<citiesCount)
        error = true;
    if(error){
        if(i>=citiesCount){
            // wiêcej danych ni¿ trzeba, czy tak zostawiæ? // Narazie zawsze tak, ominiecie obslugi
            msgBox.setText("Blad podczas wczytywania pliku\nIlosc wierszy wieksza niz ilosc miast\nCzy wczytac jedynie wymagana czesc danych?");
            msgBox.exec();
            // dodac obsluge Tak/Nie
        }else{
            // za ma³o danych, albo z³e dane
            msgBox.setText("Blad podczas wczytywania pliku\nIlosc wierszy mniejsza niz ilosc miast lub bledne dane");
            msgBox.exec();
        }
        return;
    }
    cityPositions = temp;
    draw->paintCities(citiesCount,cityPositions);
}
