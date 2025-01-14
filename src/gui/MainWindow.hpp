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
    BottomBar *m_bottomBar;

    PageDisplay *m_pageViewer;

public: 
    MainWindow(QWidget *parent = 0);

    void showImage(Image image) { m_pageViewer->showImage(image); }
};

#endif