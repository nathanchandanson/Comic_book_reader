#include <iostream>

#include <QApplication>
#include <QFile>
#include <QDir>

#include "Archive.hpp"
#include "Datatypes.hpp"
#include "MainWindow.hpp"

#include "Filtrage.hpp"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

// int main(int argc, char *argv[])
// {

//     


//     
//     theComic.addPage(QPixmap("../data/comics/testImage2.jpg"), 1);
//     theComic.addPage(QPixmap("../data/comics/testImage3.jpg"), 2);   
    

    

//     
// }
//int argc, char* argv[]
int main(int argc, char* argv[])
{

    QApplication app(argc, argv);
    const QString imagePath = "/home/hugodush/Projet_C++/Comic_book_reader/data/comics/testImage2.jpg";  // Remplacer par ton fichier JPEG
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        std::cerr << "Erreur de chargement de l'image" << std::endl;
        return -1;
    }
    printf("ok6\n");
    // Convertir le QPixmap en QImage
    QImage image = pixmap.toImage();
    if (isTextDominant(image))
    {
        std::cout << "Image avec texte dominant. Application du filtre de netteté." << std::endl;
        QImage sharpened = sharpenImage(image);
        sharpened.save("/home/hugodush/Projet_C++/Comic_book_reader/build/users_data/filtered_text.jpg");
    }
    else 
    {
        std::cout << "Image graphique dominante. Application du filtre d'amélioration des couleurs." << std::endl;
        QImage enhanced = enhanceColors(image);
        enhanced.save("/home/hugodush/Projet_C++/Comic_book_reader/build/users_data/filtered_graphic.jpg");
    }
    return 0;

    /* Setup GUI utilities*/
    /*
    // Application et fenêtre principale
    QApplication app(argc, argv);
    MainWindow theWindow;
    // Style de la fenêtre
    QFile styleSheet("../data/stylesheet.qss");
    styleSheet.open(QFile::ReadOnly | QFile::Text);
    app.setStyleSheet(styleSheet.readAll());
    styleSheet.close();
    */

    /* Setup the Comic */
    /*
    Comic theComic;
    // Connexion des signaux et slots
    // MainWindow vers Comic
    QObject::connect(&theWindow, &MainWindow::nextPagePressed, &theComic, &Comic::incrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::previousPagePressed, &theComic, &Comic::decrementPageNumber);
    QObject::connect(&theWindow, &MainWindow::pageSliderValueChanged, &theComic, &Comic::setPageNumber);
    // Comic vers MainWindow
    QObject::connect(&theComic, &Comic::pageChanged, &theWindow, &MainWindow::updatePageChanges);

    */
    /* Setup extraction utilities */
    /*
    ComicExtractor reader("../../data/comics/onePiece005.cbz");
    if(!reader.openComic()){return 1;}

    std::vector<std::string> pages = reader.listPages();
    std::cout<< "Pages dans l'archive:\n";
    for(const auto& page: pages) { std::cout<< "- "<< page <<std::endl;}

    */
    /* Extracting all the pages */
    /*
    for(int i = 0; i < pages.size(); i++){
        reader.extractPageByNumber(i, "../users_data");
        QString currentPagePath("../users_data/");
        currentPagePath += pages[i];
        theComic.addPage(QPixmap(currentPagePath), i);
    }
    */
    /* Launching the app */
    /*
    theWindow.show();
    return app.exec();
    */
}