#include <iostream>

#include <QApplication>
#include <QFile>
#include <QDir>

#include "Archive.hpp"
#include "Datatypes.hpp"
#include "MainWindow.hpp"
#include "Filtrage.hpp"




int main(int argc, char* argv[])
{

    QApplication app(argc, argv);
    const QString imagePath = "/home/hugodush/Projet_C++/Comic_book_reader/data/comics/testImage2.jpg";  // Remplacer par ton fichier JPEG
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        std::cerr << "Erreur de chargement de l'image" << std::endl;
        return -1;
    }
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
}