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

public slots:
    void showImage(Image image);
    void rotate();
    void zoomIn();
    void zoomOut();
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
    QPushButton *m_rotateButton;
    QPushButton *m_zoomOutButton;
    QPushButton *m_zoomInButton;

public:
    BottomBar(QWidget *parent = 0);

signals:
    void nextPagePressed();
    void previousPagePressed();
    void zoomInPressed();
    void zoomOutPressed();
    void rotatePressed();
    void pageSliderValueChanged(int page);


public slots:
    void updatePageSlider(int page, int maxPage);
    void updatePageIndicator(int page, int maxPage);
};

#endif