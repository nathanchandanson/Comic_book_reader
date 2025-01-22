#include <iostream>

#include <QApplication>
#include <QFile>
#include <QDir>

#include "Archive.hpp"
#include "Datatypes.hpp"
#include "MainWindow.hpp"

// int main(int argc, char *argv[])
// {

//     


//     
//     theComic.addPage(QPixmap("../data/comics/testImage2.jpg"), 1);
//     theComic.addPage(QPixmap("../data/comics/testImage3.jpg"), 2);   
    

    

//     
// }

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


    /* Setup the Comic */
    Comic theComic;
    // Connexion des signaux et slots
    // MainWindow vers Comic
    QObject::connect(&theWindow, &MainWindow::nextPagePressed, &theComic, &Comic::incrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::previousPagePressed, &theComic, &Comic::decrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::pageSliderValueChanged, &theComic, &Comic::setPageNumber);
    // Comic vers MainWindow
    QObject::connect(&theComic, &Comic::pageChanged, &theWindow, &MainWindow::updatePageChanges);


    /* Setup extraction utilities */
    ComicExtractor reader("../../data/comics/onePiece005.cbz");
    if(!reader.openComic()){return 1;}

    std::vector<std::string> pages = reader.listPages();
    std::cout<< "Pages dans l'archive:\n";
    for(const auto& page: pages) { std::cout<< "- "<< page <<std::endl;}


    /* Extracting all the pages */
    for(int i = 0; i < pages.size(); i++){
        reader.extractPageByNumber(i, "../users_data");
        QString currentPagePath("../users_data/");
        currentPagePath += pages[i];
        theComic.addPage(QPixmap(currentPagePath), i);
    }

    /* Launching the app */
    theWindow.show();
    return app.exec();
}