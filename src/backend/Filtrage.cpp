// Il faut télécharger les librairies suivantes:
// sudo apt install tesseract-ocr
// sudo apt install libtesseract-dev

#include "Filtrage.hpp"

#include <QPixmap>
#include <QImage>

#include <tesseract/baseapi.h> // Utilisée pour faire la reconnaissance de texte sur les images
#include <leptonica/allheaders.h> // Utilisée par Tesseract pour manipuler les données d'image

// Pour les filtres:
#include <QtGui>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect> 
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <iostream>





bool isTextDominant(const QImage& image) // 
{
    QImage temp = image.convertToFormat(QImage::Format_RGB888); // 8bits par canal de couleur (R,G,B) pour chaque pixel
    // Cette conversion est nécéssaire pour effectuer le traitement avec la librairie Tesseract (pour la reconnaissance optique)

    QByteArray byteArray; // Pour stocker les données binaires de l'image
    QBuffer buffer(&byteArray); // Le buffer sert à faciliter la lecture et l'écritue 
    temp.save(&buffer, "PNG"); // Stockage dans le buffer (PNG car format compact et compressé qui préserve la qualité de l'image)

    Pix* pix = pixReadMem(reinterpret_cast<const l_uint8*>(byteArray.constData()), byteArray.size());
    // "reinterpret_cast" pour convertir les données brutes au format accepté par "pixReadMem"
    // "pixReadMem" crée un objet Pix pour représenter l'image (de la librairie Leptonica)

    if (pix == nullptr) {
        std::cerr << "Erreur de lecture de l'image avec Leptonica" << std::endl;
        return false;
    }

    tesseract::TessBaseAPI ocr; // Traitement OCR (Reconnaissance Optique de Caractères), permet de récupérer les méthodes nécéssaires à la reconnaissance du texte
    ocr.Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY); // Ici on a choisi l'anglais, mais on aurait pu rajouter la langue en argument de la fonction pour pouvoir adapter la détection du texte en fonction de la langue du Comic
    // On utilise un réseau de neurones LSTM pour effectuer la reconnaissance
    ocr.SetImage(pix);
    char* text = ocr.GetUTF8Text();
    std::string detectedText(text);
    
    ocr.End();
    pixDestroy(&pix);
    delete[] text;
    // Pour néttoyer les allocations

    double textDensity = detectedText.size() / static_cast<double>(image.width() * image.height());
    // Nombre de caractères détectés / La taille de l'image
    return textDensity > 0.05; // On a choisi cette valeure de facon très approximative
}

QImage sharpenImage(const QImage& image)
{
    QGraphicsScene scene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image)); // Ici la conversion en QPixmap est nécéssaire pour utiliser la fonction "QGraphicsPixmapItem"
    QGraphicsBlurEffect* effect = new QGraphicsBlurEffect(); // Applique un effet de flou gaussien
    effect->setBlurRadius(0.5);  // Définit l'intensité du flou (ici 0,5 correspond à un flou léger)
    item->setGraphicsEffect(effect); // Application du flou

    scene.addItem(item);
    scene.setSceneRect(image.rect());

    QImage result(image.size(), QImage::Format_ARGB32); // Nouvelle Image créée
    QPainter painter(&result);
    scene.render(&painter);
    return result;
}


QImage enhanceColors(const QImage& image)
{
    QImage result = image.copy(); // On ne veut pas modifier directement l'image source (on veut garder une version originale)
    for(int y = 0; y< result.height(); ++y)
    {
        for(int x = 0; x< result.width(); ++x)
        {
            QColor color = result.pixelColor(x,y); // Pour chaque pixel, on intensifie chaque couleur 
            int red = std::min(color.red() * 1.2, 255.0);
            int green = std::min(color.green() * 1.2, 255.0);
            int blue = std::min(color.blue() * 1.2, 255.0);
            color.setRgb(red, green, blue); 
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}
