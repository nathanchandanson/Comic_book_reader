#include <iostream>
#include "Datatypes.hpp"
#include "Archive.hpp"
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!" << std::endl;

    ComicExtractor reader("/home/hugodush/Projet_C++/Comic_book_reader/data/comics/onePiece005.cbz");
    if(!reader.openComic()){return 1;}

    std::vector<std::string> pages = reader.listPages();
    std::cout<< "Pages dans l'archive:\n";
    for(const auto& page: pages)
    {
        std::cout<< "- "<< page <<std::endl;
    }

/*     std::string PageToExtract = pages[0];
    std::cout << "Page à extraire : " << PageToExtract << std::endl;
    if(reader.extractPage(PageToExtract, "/home/hugodush/Projet_C++/Comic_book_reader/data/comics"))
    {
        std::cout<< "Fichier extrait avec succès :" << PageToExtract << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de l'extraction de la page." << std::endl;
    }

    return 0; */

    int PageToExtract = 1;
    std::cout << "Numéro de page à extraire : " << PageToExtract << std::endl;
    if(reader.extractPageByNumber(PageToExtract, "/home/hugodush/Projet_C++/Comic_book_reader/data/comics"))
    {
        std::cout<< "Page extraite avec succès :" << PageToExtract << std::endl;
    }
    else
    {
        std::cerr << "Erreur lors de l'extraction de la page." << std::endl;
    }

    std::string inputDir = "/home/hugodush/Projet_C++/Comic_book_reader/data_test";  
    std::string outputDir = "/home/hugodush/Projet_C++/Comic_book_reader/archive_test"; 

    ComicGenerator generator;

    if (generator.generateFromDirectory(inputDir, outputDir)) {
        std::cout << "Fichier CBZ créé avec succès." << std::endl;
    } else {
        std::cerr << "Échec de la création du fichier CBZ." << std::endl;
    }


    // return 0;
}