#include <QtGui/QApplication>
#include "mainwindow.h"
#include "tests.cpp"
int startGUI(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}




int main(int argc, char *argv[])
{
    //return startGUI(argc,argv);
    return startGUI(argc,argv);
    //return inputReadTest();
    //return generatingInitialRouteTest();
   // return generatingInitialRouteTest();
}





