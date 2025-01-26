#include "Archive.hpp"
#include <iostream>
#include <fstream>
#include <archive.h>
#include <archive_entry.h>

#include <filesystem>

ComicExtractor::ComicExtractor(const std::string& ComicPath): ComicPath(ComicPath), ComicReader(nullptr), ComicWriter(nullptr), currentPageChanged(false), allPagesExtracted(false)
{}
ComicExtractor::~ComicExtractor()
{
    if(ComicReader != nullptr){archive_read_free(ComicReader);}
    if(ComicWriter != nullptr){archive_write_free(ComicWriter);}
}

bool ComicExtractor::openComic()
{
    if (ComicReader != nullptr)
    {
        archive_read_free(ComicReader); // Libérer la mémoire si déjà initialisé
        ComicReader = nullptr;
        
    }
    ComicReader= archive_read_new(); // On utilise les fonctions de libarchive pour effectuer la lecture et l'écriture des fichiers compressés
    archive_read_support_format_all(ComicReader); // Cette fonction active la prise en charge de tous les formats d'archives(zip, tar...)
    archive_read_support_filter_all(ComicReader); // Pareil mais pour les types de compression

    int result= archive_read_open_filename(ComicReader, ComicPath.c_str(), 20480); // Pourquoi c_str()? et 10240?
    if(result != ARCHIVE_OK)
    {
        std::cerr << "Erreur lors de l'ouverture de l'archive : " << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    struct archive_entry* entry;
    PageNames.clear(); // Réinitialise le nom des pages
    PageNumberToName.clear(); // Réinitialise le dictionnaire qui associe le nom de la page à son numéro

    int PageNumber = 0; // Si on commence la numérotation des pages à 0
    while(archive_read_next_header(ComicReader, &entry) == ARCHIVE_OK)
    {
        const char* PageName = archive_entry_pathname(entry); // Permet de récupérer le nom du fichier, puis de l'ajouter à la liste de noms PageNames
        PageNames.push_back(PageName);
        PageNumberToName[PageNumber++] = PageName; // On associe au numéro de page le nom que l'on vient d'extraire
        archive_read_data_skip(ComicReader); // Passer au fichier suivant
    }
    archive_read_free(ComicReader);
    ComicReader = nullptr;

    extractedPages.resize(PageNames.size(), false); 
    return true;
}

bool ComicExtractor::extractPage(const std::string& PageName, const std::string& outputDir)
{
    if (ComicReader != nullptr) // Réinitialiser le Reader
    {
        archive_read_free(ComicReader); 
        std::cout<<ComicReader<<std::endl;
        ComicReader = nullptr;
    }

    ComicReader = archive_read_new(); 
    archive_read_support_format_all(ComicReader);
    archive_read_support_filter_all(ComicReader);

    int result = archive_read_open_filename(ComicReader, ComicPath.c_str(), 10240);
    if (result != ARCHIVE_OK)
    {
        std::cerr << "Erreur lors de l'ouverture de l'archive pour extraction : " << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    if (ComicWriter != nullptr) // Réinitialiser le Writer
    {
        archive_write_free(ComicWriter); 
        ComicWriter = nullptr;
    }
    ComicWriter = archive_write_disk_new();

    struct archive_entry* entry;
    while((result = archive_read_next_header(ComicReader, &entry)) == ARCHIVE_OK)
    {
        const char* CurrentPageName = archive_entry_pathname(entry);
        if(PageName == CurrentPageName)
        {
            std::string fullOutputPath = outputDir + "/" + PageName;
            std::filesystem::create_directories(outputDir);

            archive_entry_set_pathname(entry, fullOutputPath.c_str());
            result = archive_write_header(ComicWriter, entry);
            if(result != ARCHIVE_OK)
            {
                std::cerr<< "Erreur lors de l'extraction de la page."<<std::endl;
                return false;

            }
            const void* buff;
            size_t size;
            la_int64_t offset;
            while((result = archive_read_data_block(ComicReader, &buff, &size, &offset)) == ARCHIVE_OK)
            {
                archive_write_data_block(ComicWriter, buff, size, offset);
            }
            if (result != ARCHIVE_EOF) {
                std::cerr << "Erreur lors de la lecture des blocs de données." << std::endl;
                return false;
            }
            archive_write_finish_entry(ComicWriter);
            break;
        }
    }

    if (result != ARCHIVE_EOF && result != ARCHIVE_OK) {
        std::cerr << "Erreur lors de la lecture de l'archive : " 
                  << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    archive_read_free(ComicReader);
    ComicReader = nullptr;

    return true;
}

std::vector<std::string> ComicExtractor::listPages() 
{
    return PageNames;
}

bool ComicExtractor::extractPageByNumber(int PageNumber, const std::string& outputDir)
{
    auto it = PageNumberToName.find(PageNumber); // On récupère le nom de la page associée au numéro de page
    if( it == PageNumberToName.end())
    {
        std::cerr<<"Numéro de page invalide: "<< PageNumber<< std::endl;
        return false;
    }
    return extractPage(it->second, outputDir);
}

void ComicExtractor::extractAllPages()
{
    int numPages = extractedPages.size();
    int i;
    while(!allPagesExtracted)
    {
        i = 0;
        for(; i<numPages; i++)
        {
            if(currentPageChanged){ currentPageChanged = false; break; }    // Si la page a changé, on doit repartir de la nouvelle page
            if(!extractedPages[(currentPage+i)%numPages]){ // Si la page n'a pas encore été extraite
                extractPageByNumber((currentPage+i)%numPages, "../users_data"); // On extrait la page
                extractedPages[(currentPage+i)%numPages] = true;                // On dit qu'on l'a extraite
                QString currentPagePath("../users_data/");
                currentPagePath += PageNames[(currentPage+i)%numPages];
                emit newPageExtracted(QPixmap(currentPagePath), (currentPage+i)%numPages);  // On transmet la nouvelle page
                if(i == 0){ emit firstPageExtracted(); }  // Needed to automatically refresh the PageDisplay when the wanted page is extracted
            }
        }
        if(i == numPages){ allPagesExtracted = true; return; }
    }
    return;
}


bool ComicGenerator::generateFromDirectory(const std::string& inputDir, const std::string& outputDir)
{
    namespace fs = std::filesystem;

    // Vérification de l'existence du répertoire d'entrée
    if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) {
        std::cerr << "Répertoire d'entrée invalide: " << inputDir << std::endl;
        return false;
    }

    // Vérification ou création du répertoire de sortie
    if (!fs::exists(outputDir)) {
        std::cerr << "Répertoire de sortie invalide ou inexistant: " << outputDir << std::endl;
        return false;
    }

    // Construction du chemin du fichier d'archive
    std::string outputFilePath = outputDir + "/archive.cbz";  // Nom de l'archive à créer dans le répertoire de sortie

    // Création de l'archive
    struct archive* ComicWriter = archive_write_new();
    archive_write_set_format_zip(ComicWriter);

    if (archive_write_open_filename(ComicWriter, outputFilePath.c_str()) != ARCHIVE_OK) {
        std::cerr << "Erreur lors de la création de l'archive." << std::endl;
        return false;
    }

    // Parcours du répertoire d'entrée
    for (const auto& entry : fs::recursive_directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::cout << "Ajout du fichier : " << filePath << std::endl;

            struct archive_entry* entryInArchive = archive_entry_new();
            archive_entry_set_pathname(entryInArchive, entry.path().filename().c_str());  // Utiliser le nom du fichier seulement
            archive_entry_set_size(entryInArchive, fs::file_size(entry));
            archive_entry_set_filetype(entryInArchive, AE_IFREG);
            archive_entry_set_perm(entryInArchive, 0644);

            if (archive_write_header(ComicWriter, entryInArchive) != ARCHIVE_OK) {
                std::cerr << "Erreur lors de l'écriture de l'en-tête pour le fichier : "
                          << filePath << std::endl;
                archive_entry_free(entryInArchive);
                continue;
            }

            // Lecture et écriture du contenu du fichier dans l'archive
            std::ifstream fileStream(filePath, std::ios::binary);
            if (!fileStream) {
                std::cerr << "Erreur lors de l'ouverture du fichier : " << filePath << std::endl;
                archive_entry_free(entryInArchive);
                continue;
            }

            std::vector<char> buffer(fs::file_size(entry));
            fileStream.read(buffer.data(), buffer.size());
            archive_write_data(ComicWriter, buffer.data(), buffer.size());
            archive_entry_free(entryInArchive);
        }
    }

    // Fermeture de l'archive
    if (archive_write_close(ComicWriter) != ARCHIVE_OK) {
        std::cerr << "Erreur lors de la fermeture de l'archive : " << archive_error_string(ComicWriter) << std::endl;
        archive_write_free(ComicWriter);
        return false;
    }

    archive_write_free(ComicWriter);

    std::cout << "Archive créée avec succès à l'emplacement : " << outputFilePath << std::endl;
    return true;
}
