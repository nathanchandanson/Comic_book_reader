#include "WindowComponents.hpp"

#include <QGridLayout>

/*
 *  Top bar
*/

/*
 *  Middle section
*/
/* Constructeur */
PageDisplay::PageDisplay(QWidget *parent)
    : QWidget(parent)
{
    m_middleScene = new QGraphicsScene(this);
    m_middleView = new QGraphicsView(m_middleScene, this);
    
    // Ajouter m_graphicsView au layout de PageDisplay
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_middleView);
    setLayout(layout);
}

/* Affichage d'une page */
void PageDisplay::showImage(Image image)
{
    m_middleScene->addPixmap(image);
}

/*
 *  Bottom bar
*/
BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent)
{
    // Create the slider
    m_pageSlider = new QSlider(Qt::Horizontal, this);
    m_pageSlider->setRange(1,10);
    m_pageSlider->setValue(0);

    // Create the page text zone
    m_currentPage = new QLabel(this);
    m_currentPage->setText(QString("1/10"));

    // Create the buttons
    m_prevPageButton = new QPushButton(this);
    m_nextPageButton = new QPushButton(this);
    // Create the button icons
    QIcon next_icon("../data/icons/next_icon.png");
    QIcon previous_icon("../data/icons/previous_icon.png");
    // Apply the icons to the buttons
    m_prevPageButton->setIcon(previous_icon);
    m_nextPageButton->setIcon(next_icon);

    // Arrange the widgets within a QGridLayout
    QGridLayout *bottomBarGridLayout = new QGridLayout(this);
    bottomBarGridLayout->addWidget(m_pageSlider, 0,0);
    bottomBarGridLayout->addWidget(m_currentPage, 0,1);
    bottomBarGridLayout->addWidget(m_prevPageButton, 0,2);
    bottomBarGridLayout->addWidget(m_nextPageButton, 0,3);
    setLayout(bottomBarGridLayout);
}
