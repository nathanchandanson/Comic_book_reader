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

    // Connecting all the signals to the components signals
    connect(m_bottomBar, &BottomBar::nextPagePressed, this, &MainWindow::nextPagePressed);
    connect(m_bottomBar, &BottomBar::previousPagePressed, this, &MainWindow::previousPagePressed);
    connect(m_bottomBar, &BottomBar::pageSliderValueChanged, this, &MainWindow::pageSliderValueChanged);

    // Arrange the widgets within a QGridLayout
    QGridLayout *mainWindowGridLayout = new QGridLayout(this);
    mainWindowGridLayout->addWidget(m_bottomBar, 1,0);
    mainWindowGridLayout->addWidget(m_pageViewer, 0,0);
    setLayout(mainWindowGridLayout);
}

/* Update label et view on pageChanged */
void MainWindow::updatePageChanges(Image newPage, int newPageNumber, int maxPage)
{
    std::cout<<newPageNumber<<","<<maxPage<<std::endl<<std::endl;
    this->showImage(newPage);
    m_bottomBar->updatePageSlider(newPageNumber, maxPage);
    m_bottomBar->updatePageIndicator(newPageNumber, maxPage);
}