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


class ComicExtractor{
private:
    std::string ComicPath; // Permet d'identifier le Comic que l'on veut extraire (On identifie le Comic avec son adresse, pas avec son nom)
    struct archive* ComicReader; // Permet de lire le contenu du Comic sans désarchiver
    struct archive* ComicWriter; // Permet d'écrire ce qui a été lu grâce à libarchive
    std::vector<std::string> PageNames; // Permet de récupérer le nom des pages, il faudra ensuite tenir un dictionnaire de correspondance entre le numéro et le nom des pages
    std::map<int, std::string> PageNumberToName; // Dictionnaire qui associe le numéro au bon nom
public:
    ComicExtractor(const std::string& ComicPath); // La classe Comic Extractor sera appelée directement en dennant l'adresse du Comic concerné
    ~ComicExtractor();

    bool openComic(); // Permet d'ouvrir le Comic, obtenir le nom des pages sans les décompresser.
    bool extractPage(const std::string& PageName, const std::string& outputDir); // Permet d'extraire une page spécifique dont on connait le nom
    bool extractPageByNumber(int PageNumber, const std::string& outputDir); // Nouvelle méthode
    std::vector<std::string> listPages();
};

class ComicGenerator{
public:
    ComicGenerator() = default;
    bool generateFromDirectory(const std::string& inputDir, const std::string& outputDir);
};


#endif