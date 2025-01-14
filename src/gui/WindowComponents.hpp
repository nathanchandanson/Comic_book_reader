#ifndef WINDOWCOMPONENTS_HPP
#define WINDOWCOMPONENTS_HPP
/*
 *  This file contains all the headers of all the different widgets that compose our main window
*/
#include "Datatypes.hpp"

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QIcon>
#include <QLabel>

/*
 *  Top bar
*/

/*
 *  Middle section
*/
class PageDisplay : public QWidget
{
    Q_OBJECT

private:
    QGraphicsScene *m_middleScene;
    QGraphicsView *m_middleView;

public: 
    PageDisplay(QWidget *parent = 0);

    void showImage(Image image);
};

/*
 *  Bottom bar
*/
class BottomBar : public QWidget
{
    Q_OBJECT

private: 
    QSlider *m_pageSlider;
    QLabel *m_currentPage;
    QPushButton *m_prevPageButton;
    QPushButton *m_nextPageButton;

public:
    BottomBar(QWidget *parent = 0);
};

#endif