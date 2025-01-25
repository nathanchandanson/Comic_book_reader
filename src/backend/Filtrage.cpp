#include <QPixmap>
#include <QImage>
// Il faut télécharger les librairies suivantes:
// sudo apt install tesseract-ocr
// sudo apt install libtesseract-dev
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QtGui>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "Filtrage.hpp"

QImage QpixmapToQImage(const QPixmap& pixmap)
{   
    return pixmap.toImage();
}

bool isTextDominant(const QImage& image)
{
    QImage temp = image.convertToFormat(QImage::Format_RGB888); // Cette conversion doit être faite pour pouvoir utiliser les outils de Tesseract
    printf("ok16\n");
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    temp.save(&buffer, "PNG");
    Pix* pix = pixReadMem(reinterpret_cast<const l_uint8*>(byteArray.constData()), byteArray.size());
    if (pix == nullptr) {
        std::cerr << "Erreur de lecture de l'image avec Leptonica" << std::endl;
        return false;
    }
    printf("ok21\n");
    tesseract::TessBaseAPI ocr;
    printf("ok22\n");
    ocr.Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY);
    ocr.SetImage(pix);
    printf("ok17\n");
    char* text = ocr.GetUTF8Text();
    std::string detectedText(text);
    ocr.End();
    pixDestroy(&pix);
    delete[] text;
    printf("ok18\n");
    double textDensity = detectedText.size() / static_cast<double>(image.width() * image.height());
    return textDensity > 0.05;
}

QImage sharpenImage(const QImage& image)
{
    printf("ok19\n");
    QGraphicsScene scene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    printf("ok20\n");
    QGraphicsBlurEffect* effect = new QGraphicsBlurEffect();
    effect->setBlurRadius(0.5);  // Réglage du rayon de flou
    item->setGraphicsEffect(effect);

    scene.addItem(item);
    scene.setSceneRect(image.rect());

    QImage result(image.size(), QImage::Format_ARGB32);
    QPainter painter(&result);
    scene.render(&painter);
    return result;
}


QImage enhanceColors(const QImage& image)
{
    QImage result = image.copy();
    for(int y = 0; y< result.height(); ++y)
    {
        for(int x = 0; x< result.width(); ++x)
        {
            QColor color = result.pixelColor(x,y);
            int red = std::min(color.red() * 1.2, 255.0);
            int green = std::min(color.green() * 1.22, 255.0);
            int blue = std::min(color.blue() * 1.2, 255.0);
            color.setRgb(red, green, blue);
            result.setPixelColor(x, y, color);
        }
    }
    return result;
}
