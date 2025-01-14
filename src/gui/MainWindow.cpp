#include "MainWindow.hpp"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Comic Book Reader");
    
    m_bottomBar = new BottomBar(this);
    m_pageViewer = new PageDisplay(this);

    // Arrange the widgets within a QGridLayout
    QGridLayout *mainWindowGridLayout = new QGridLayout(this);
    mainWindowGridLayout->addWidget(m_bottomBar, 1,0);
    mainWindowGridLayout->addWidget(m_pageViewer, 0,0);
    setLayout(mainWindowGridLayout);
}