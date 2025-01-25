// Filtrage.h
#ifndef FILTRAGE_HPP
#define FILTRAGE_HPP

#include <QImage>

QImage QpixmapToQImage(const QPixmap& pixmap);
bool isTextDominant(const QImage& image);
QImage sharpenImage(const QImage& image);
QImage enhanceColors(const QImage& image);

#endif // FILTRAGE_H
