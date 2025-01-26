#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
/*
 *  This file contains all the headers of the object that represents our main window
*/
#include "WindowComponents.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    // Composants de la fenêtre
    BottomBar *m_bottomBar;
    PageDisplay *m_pageViewer;

public: 
    MainWindow(QWidget *parent = 0);

    void showImage(Image image) { m_pageViewer->showImage(image); }

signals:
    void nextPagePressed();
    void previousPagePressed();
    void pageSliderValueChanged(int page);

public slots:
    void updatePageChanges(Image newPage, int newPageNumber, int maxPage);  // Pour process tous les changements quand une page change
};

#endif