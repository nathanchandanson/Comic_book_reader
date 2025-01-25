#ifndef ARCHIVE_HPP
#define ARCHIVE_HPP

#include <archive.h>
#include <archive_entry.h>
#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <QObject>
#include "Datatypes.hpp"


class ComicExtractor : public QObject
{
    Q_OBJECT

private:
    std::string ComicPath; // Permet d'identifier le Comic que l'on veut extraire (On identifie le Comic avec son adresse, pas avec son nom)
    struct archive* ComicReader; // Permet de lire le contenu du Comic sans désarchiver
    struct archive* ComicWriter; // Permet d'écrire ce qui a été lu grâce à libarchive
    std::vector<std::string> PageNames; // Permet de récupérer le nom des pages, il faudra ensuite tenir un dictionnaire de correspondance entre le numéro et le nom des pages
    std::map<int, std::string> PageNumberToName; // Dictionnaire qui associe le numéro au bon nom
    std::vector<bool> extractedPages; // Permet de savoir quelles pages ont déjà été extraites
    bool allPagesExtracted; // Permet de savoir si toutes les pages ont été extraites
    int currentPage; // Le numéro de la page que l'utilisateur veut voir
    bool currentPageChanged; // Permet de savoir si on a changé de page actuelle

public:
    ComicExtractor(const std::string& ComicPath); // La classe Comic Extractor sera appelée directement en dennant l'adresse du Comic concerné
    ~ComicExtractor();

    bool openComic(); // Permet d'ouvrir le Comic, obtenir le nom des pages sans les décompresser.
    bool extractPage(const std::string& PageName, const std::string& outputDir); // Permet d'extraire une page spécifique dont on connait le nom
    bool extractPageByNumber(int PageNumber, const std::string& outputDir); // Nouvelle méthode
    std::vector<std::string> listPages();
    void extractAllPages();

signals: 
    void newPageExtracted(Image thePage, int thePageNumber);

public slots:
    void updateCurrentPage(Image newPage, int newPageNumber, int maxPage) { currentPage = newPageNumber; currentPageChanged = 1; };
};

class ComicGenerator{
public:
    ComicGenerator() = default;
    bool generateFromDirectory(const std::string& inputDir, const std::string& outputDir);
};


#endif