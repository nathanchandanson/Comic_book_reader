#include <iostream>
#include <thread>

#include <QApplication>
#include <QFile>
#include <QDir>

#include "Archive.hpp"
#include "Datatypes.hpp"
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    /* Setup GUI utilities*/
    // Application et fenêtre principale
    QApplication app(argc, argv);
    MainWindow theWindow;
    // Style de la fenêtre
    QFile styleSheet("../data/stylesheet.qss");
    styleSheet.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(styleSheet.readAll());
    styleSheet.close();


    /* Setup extraction utilities */
    ComicExtractor reader("../data/comics/onePiece004.cbz");
    if(!reader.openComic()){return 1;}

    std::vector<std::string> pages = reader.listPages();    // Récupérer toutes les pages
    // std::cout<< "Pages dans l'archive:\n";
    // for(const auto& page: pages) { std::cout<< "- "<< page <<std::endl;}


    /* Setting up the Comic */
    Comic theComic(pages.size());   // On donne le nombre de pages
    // Connexion des signaux et slots
    // MainWindow vers Comic
    QObject::connect(&theWindow, &MainWindow::nextPagePressed, &theComic, &Comic::incrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::previousPagePressed, &theComic, &Comic::decrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::pageSliderValueChanged, &theComic, &Comic::setPageNumber);
    // Comic vers MainWindow
    QObject::connect(&theComic, &Comic::pageChanged, &theWindow, &MainWindow::updatePageChanges);
    // ComicExtractor vers Comic
    QObject::connect(&reader, &ComicExtractor::newPageExtracted, &theComic, &Comic::addPage);
    QObject::connect(&reader, &ComicExtractor::firstPageExtracted, &theComic, &Comic::refreshPage);
    // Comic vers ComicExtractor
    QObject::connect(&theComic, &Comic::pageChanged, &reader, &ComicExtractor::updateCurrentPage);


    /* Thread d'extraction des pages */
    std::thread extractionThread(&ComicExtractor::extractAllPages, &reader);


    /* Launching the app */
    theWindow.show();
    app.exec();
    extractionThread.join();
    
    return 0;
}