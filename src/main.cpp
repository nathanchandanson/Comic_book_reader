#include <iostream>

#include <QApplication>

#include "Datatypes.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow theWindow;
    Comic theComic;

    // Initialisation du comic
    theComic.setPageNumber(0);
    theComic.setMaxPageNumber(0);
    theComic.addPage(QPixmap("../data/comics/testImage1.jpg"), 0);
    theComic.addPage(QPixmap("../data/comics/testImage2.jpg"), 1);
    theComic.addPage(QPixmap("../data/comics/testImage3.jpg"), 2);   
    
    // Connexion des signaux et slots
    // MainWindow vers Comic
    QObject::connect(&theWindow, &MainWindow::nextPagePressed, &theComic, &Comic::incrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::previousPagePressed, &theComic, &Comic::decrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::pageSliderValueChanged, &theComic, &Comic::setPageNumber);
    // Comic vers MainWindow
    QObject::connect(&theComic, &Comic::pageChanged, &theWindow, &MainWindow::updatePageChanges);
    

    theWindow.show();
    return app.exec();
}