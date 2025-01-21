#include "MainWindow.hpp"
#include <QGridLayout>

#include <iostream>

/* Constructor */
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Comic Book Reader");
    
    m_bottomBar = new BottomBar(this);
    m_pageViewer = new PageDisplay(this);

    // Connecting all the signals that go at the exterior of MainWindow
    connect(m_bottomBar, &BottomBar::nextPagePressed, this, &MainWindow::nextPagePressed);
    connect(m_bottomBar, &BottomBar::previousPagePressed, this, &MainWindow::previousPagePressed);
    connect(m_bottomBar, &BottomBar::pageSliderValueChanged, this, &MainWindow::pageSliderValueChanged);
    // Connecting all the signals that are intern to MainWindow
    connect(m_bottomBar, &BottomBar::rotatePressed, m_pageViewer, &PageDisplay::rotate);
    connect(m_bottomBar, &BottomBar::zoomOutPressed, m_pageViewer, &PageDisplay::zoomOut);
    connect(m_bottomBar, &BottomBar::zoomInPressed, m_pageViewer, &PageDisplay::zoomIn);

    // Arrange the widgets within a QGridLayout
    QGridLayout *mainWindowGridLayout = new QGridLayout(this);
    mainWindowGridLayout->addWidget(m_bottomBar, 1,0);
    mainWindowGridLayout->addWidget(m_pageViewer, 0,0);
    setLayout(mainWindowGridLayout);
}

/* Update label et view on pageChanged */
void MainWindow::updatePageChanges(Image newPage, int newPageNumber, int maxPage)
{
    this->showImage(newPage);
    m_bottomBar->updatePageSlider(newPageNumber, maxPage);
    m_bottomBar->updatePageIndicator(newPageNumber, maxPage);
}