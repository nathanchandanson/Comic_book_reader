#include "Archive.hpp"

#include <iostream> // Généralement utilisé pour faire des vérifications, signaler les pages existentes dans le comic, ou bien pour donner les messages d'erreur
#include <fstream> // Utilisé pour 
#include <archive.h> // Utilisées pour manipuler les archives
#include <archive_entry.h>
#include <filesystem> // Utilisé pour la manipulation des répertoires (création ou vérification d'existence)

ComicExtractor::ComicExtractor(const std::string& ComicPath): ComicPath(ComicPath), ComicReader(nullptr), ComicWriter(nullptr), currentPageChanged(false), allPagesExtracted(false)
{}
ComicExtractor::~ComicExtractor()
{
    if(ComicReader != nullptr){archive_read_free(ComicReader);}
    if(ComicWriter != nullptr){archive_write_free(ComicWriter);}
}

bool ComicExtractor::openComic() // Permet d'ouvrir le Comic, obtenir le nom des pages sans les décompresser. Attention, cette méthode ne peut pas être utilisée de facon asynchrone, pour l'extraction de deux Comic en parallèle, car on réinitialise le nom et les numéros des pages dans la méthode.
{
    if (ComicReader != nullptr)
    {
        archive_read_free(ComicReader); // Libérer la mémoire si déjà initialisé
        ComicReader = nullptr;
        
    }
    // On utilise les fonctions de libarchive pour effectuer la lecture et l'écriture des fichiers compressés

    ComicReader= archive_read_new(); // On initialise un objet de lecture d'archive
    archive_read_support_format_all(ComicReader); // Cette fonction active la prise en charge de tous les formats d'archives(zip, tar...)
    archive_read_support_filter_all(ComicReader); // Pareil mais pour les types de compression(gzip, bzip2...)

    int result= archive_read_open_filename(ComicReader, ComicPath.c_str(), 10240); // Objet qui permet d'accéder à la lecture d'archive ComicReader, qui doit contenir les métadonnées et les données d'accès
    // 10240 est "apparament" le meilleur compromis entre performance et mémoire pour la taille du tampon
    // c_str() est utilisée pour pouvoir passer ComicPath de std::string à const char*, pour être compatible avec le type d'arguments pris par la fonction archive_read_open_filename
    if(result != ARCHIVE_OK)
    {
        std::cerr << "Erreur lors de l'ouverture de l'archive : " << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    struct archive_entry* entry; // Cet objet permet d'accéder à un seul fichier dans l'archive concernée (il contient les métadonnées du fichier et les données d'accès)
    
    PageNames.clear(); // Réinitialise le nom des pages (pour un nouveau Comic)
    PageNumberToName.clear(); // Réinitialise le dictionnaire qui associe le nom de la page à son numéro 

    int PageNumber = 0; // On commence la numérotation des pages à 0
    while(archive_read_next_header(ComicReader, &entry) == ARCHIVE_OK) // On passe d'un fichier (entry) à un autre dans l'archive (lue par ComicReader)
    {
        const char* PageName = archive_entry_pathname(entry); // On récupère le nom de l'entrée (fichier) concerné
        PageNames.push_back(PageName); // On ajoute ce nom à la liste des noms de pages
        PageNumberToName[PageNumber++] = PageName; // On associe à la page son numéro
        archive_read_data_skip(ComicReader); // Dans cette méthode, on ignore le contenu des fichiers, on récupère seulement le nom des pages
    }
    archive_read_free(ComicReader);
    ComicReader = nullptr;

    extractedPages.resize(PageNames.size(), false); 
    return true;
}

bool ComicExtractor::extractPage(const std::string& PageName, const std::string& outputDir) // Permet d'extraire une page spécifique dont on connait le nom
{
    if (ComicReader != nullptr) // On réinitalise le lecteur d'archive pour éviter tout conflit de mémoire
    {
        archive_read_free(ComicReader); 
        std::cout<<ComicReader<<std::endl;
        ComicReader = nullptr;
    }

    ComicReader = archive_read_new(); // On initialise le ComicReader de la même façon que dans la méthode précédente
    archive_read_support_format_all(ComicReader);
    archive_read_support_filter_all(ComicReader);

    int result = archive_read_open_filename(ComicReader, ComicPath.c_str(), 10240);
    if (result != ARCHIVE_OK)
    {
        std::cerr << "Erreur lors de l'ouverture de l'archive pour extraction : " << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    if (ComicWriter != nullptr) // Ici, on initialise le Writer car on ne stocke pas les données dans une liste (comme dans openComic), mais sur le disque
    {
        archive_write_free(ComicWriter); 
        ComicWriter = nullptr;
    }
    ComicWriter = archive_write_disk_new();

    struct archive_entry* entry;
    while((result = archive_read_next_header(ComicReader, &entry)) == ARCHIVE_OK) // On passe dans cette boucle d'une page (fichier) à une autre, mais on veut s'arrêter à une page spécifique pour récupérer son contenu
    {
        const char* CurrentPageName = archive_entry_pathname(entry);
        if(PageName == CurrentPageName) // On ne s'arrête que si la page correspond au nom de page voulu
        {
            std::string fullOutputPath = outputDir + "/" + PageName; // On extrait la page dans un répertoire, dans lequel la classe Comic ira se servir pour remplir son tableau d'images
            std::filesystem::create_directories(outputDir); // Au cas où le repertoire n'existe pas déjà

            archive_entry_set_pathname(entry, fullOutputPath.c_str()); // Cela garantit que les données de la page sont extraits au bon endroit
            // à nouveau, c_str() sert à convertir le chemin du format std::string au format const char*
            result = archive_write_header(ComicWriter, entry); // On fait l'extraction des métadonnées dans un premier temps
            if(result != ARCHIVE_OK)
            {
                std::cerr<< "Erreur lors de l'extraction de la page."<<std::endl;
                return false;

            }

            // L'extraction des données se fait par bloc, avec "buff" qui pointe vers un bloc de taille "size" en mémoire. 
            // "offset" permet de savoir à quel bloc de l'extraction on est
            const void* buff;
            size_t size;
            la_int64_t offset;
            while((result = archive_read_data_block(ComicReader, &buff, &size, &offset)) == ARCHIVE_OK) // On extrait l'image par blocs de données jusqu'à ce que toutes les données soient extraites
            {
                archive_write_data_block(ComicWriter, buff, size, offset);
            }
            if (result != ARCHIVE_EOF) 
            {
                std::cerr << "Erreur lors de la lecture des blocs de données." << std::endl;
                return false;
            }
            archive_write_finish_entry(ComicWriter); // Cette fonction de libarchive permet de terminer "proprement" l'extraction de la page
            break;
        }
    }

    if (result != ARCHIVE_EOF && result != ARCHIVE_OK) 
    {
        std::cerr << "Erreur lors de la lecture de l'archive : " 
                  << archive_error_string(ComicReader) << std::endl;
        return false;
    }

    archive_read_free(ComicReader); // On libère le ComicReader 
    ComicReader = nullptr;

    return true;
}

std::vector<std::string> ComicExtractor::listPages() // Permet de lister le nom des pages présentes dans la BD
{
    return PageNames;
}

bool ComicExtractor::extractPageByNumber(int PageNumber, const std::string& outputDir) // Reprend la méthode extractPage, en allant chercher la page correspondant au numéro dans le dictionnaire PageNumberToName
{
    auto it = PageNumberToName.find(PageNumber); // On récupère le nom de la page associée au numéro de page donné en argument
    if( it == PageNumberToName.end()) // end() est utilisé comme marqueur de fin pour le conteneur map, cela veut alors dire qu'il n'y a aucune page correspondant au numéro 
    {
        std::cerr<<"Numéro de page invalide: "<< PageNumber<< std::endl;
        return false;
    }
    return extractPage(it->second, outputDir); // On applique alors la méthode extractPage en utilisant le nom de la page associée au numéro. "it->second" permet d'accéder au nom (std::string) de la page associée au numéro (int)
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

    if (!fs::exists(inputDir) || !fs::is_directory(inputDir)) // On vérifie si le répertoire d'entrée donné en argument est esiste bien et s'il est bien valide
    {
        std::cerr << "Répertoire d'entrée invalide: " << inputDir << std::endl;
        return false;
    }

    if (!fs::exists(outputDir)) // On vérifie si le répertoire de sortie existe
    {
        std::cerr << "Répertoire de sortie invalide ou inexistant: " << outputDir << std::endl;
        return false;
    }

    std::string outputFilePath = outputDir + "/archive.cbz";  // On donne le chemin et on nomme le nouveau fichier d'archive

    struct archive* ComicWriter = archive_write_new(); // Comme précédemment, on créer un ComicWriter afin de stocker l'archive sur le disque
    archive_write_set_format_zip(ComicWriter); // On effectue l'archivage au format zip

    if (archive_write_open_filename(ComicWriter, outputFilePath.c_str()) != ARCHIVE_OK) 
    {
        std::cerr << "Erreur lors de la création de l'archive." << std::endl;
        return false;
    }

    // Parcours du répertoire d'entrée
    for (const auto& entry : fs::directory_iterator(inputDir)) // On parcours le répertoire d'entrée pour récupérer les différentes pages que l'on veut archiver
    {
        if (entry.is_regular_file()) // On procède à l'archivage si l'image est un fichier "ordinaire", c'est-à-dire pas un répertoire (ou autre...)
        {
            std::string filePath = entry.path().string(); // On convertit le chemin de la page du format "std::filesystem::path" en "std::string"
            std::cout << "Ajout du fichier : " << filePath << std::endl;

            struct archive_entry* entryInArchive = archive_entry_new(); // On créer le fichier archive de la page
            archive_entry_set_pathname(entryInArchive, entry.path().filename().c_str()); 
            archive_entry_set_size(entryInArchive, fs::file_size(entry));
            archive_entry_set_filetype(entryInArchive, AE_IFREG); // Cela spécifie un fichier réfulier
            archive_entry_set_perm(entryInArchive, 0644);

            if (archive_write_header(ComicWriter, entryInArchive) != ARCHIVE_OK) 
            {
                std::cerr << "Erreur lors de l'écriture de l'en-tête pour le fichier : "
                          << filePath << std::endl;
                archive_entry_free(entryInArchive);
                continue;
            }

            
            std::ifstream fileStream(filePath, std::ios::binary); // Ouverture du fichier source (en mode binaire) pour lire son contenu
            if (!fileStream) 
            {
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

    
    if (archive_write_close(ComicWriter) != ARCHIVE_OK) // On "referme" l'archive
    {
        std::cerr << "Erreur lors de la fermeture de l'archive : " << archive_error_string(ComicWriter) << std::endl;
        archive_write_free(ComicWriter);
        return false;
    }

    archive_write_free(ComicWriter); 

    std::cout << "Archive créée avec succès à l'emplacement : " << outputFilePath << std::endl;
    return true;
}
