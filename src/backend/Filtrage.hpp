// Filtrage.h
#ifndef FILTRAGE_HPP
#define FILTRAGE_HPP

#include <QImage>

bool isTextDominant(const QImage& image); // Fonction qui permet de déterminer si une image est davantage textuelle (True) ou graphique (False), en estimant la quantité de texte présente sur l'image
QImage sharpenImage(const QImage& image); // Filtre qui augmente la netteté pour les images davantages textuelles
QImage enhanceColors(const QImage& image); // Filtre qui intensifie les couleurs pour les images davantages graphiques

#endif // FILTRAGE_H
