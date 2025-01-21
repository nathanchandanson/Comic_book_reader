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

/* Rotate */
void PageDisplay::rotate()
{
    m_middleView->rotate(90);
}

/* Zoom in */
void PageDisplay::zoomIn()
{
    m_middleView->scale(1.1,1.1);
}

/* Zoom out */
void PageDisplay::zoomOut()
{
    m_middleView->scale(0.9,0.9);
}


/*
 *  Bottom bar
*/
/* Constructeur */
BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent)
{
    // Create the slider
    m_pageSlider = new QSlider(Qt::Horizontal, this);
    m_pageSlider->setRange(0,0);
    m_pageSlider->setValue(0);

    // Create the page text zone
    m_currentPage = new QLabel(this);
    m_currentPage->setText(QString("0/0"));

    // Create the buttons
    m_prevPageButton = new QPushButton(this);
    m_nextPageButton = new QPushButton(this);
    m_rotateButton = new QPushButton(this);
    m_zoomOutButton = new QPushButton(this);
    m_zoomInButton = new QPushButton(this);

    // Create the button icons
    QIcon next_icon("../data/icons/next_icon.png");
    QIcon previous_icon("../data/icons/previous_icon.png");
    QIcon rotate_icon("../data/icons/rotate_icon.png");
    QIcon zoomOut_icon("../data/icons/zoomOut_icon.png");
    QIcon zoomIn_icon("../data/icons/zoomIn_icon.png");
    // Apply the icons to the buttons
    m_prevPageButton->setIcon(previous_icon);
    m_nextPageButton->setIcon(next_icon);
    m_rotateButton->setIcon(rotate_icon);
    m_zoomOutButton->setIcon(zoomOut_icon);
    m_zoomInButton->setIcon(zoomIn_icon);

    // Connect the signals of the components to the ones of BottomBar
    connect(m_nextPageButton, &QPushButton::clicked, this, &BottomBar::nextPagePressed);
    connect(m_prevPageButton, &QPushButton::clicked, this, &BottomBar::previousPagePressed);
    connect(m_rotateButton, &QPushButton::clicked, this, &BottomBar::rotatePressed);
    connect(m_zoomOutButton, &QPushButton::clicked, this, &BottomBar::zoomOutPressed);
    connect(m_zoomInButton, &QPushButton::clicked, this, &BottomBar::zoomInPressed);   
    connect(m_pageSlider, &QSlider::valueChanged, this, &BottomBar::pageSliderValueChanged);

    // Arrange the widgets within a QGridLayout
    QGridLayout *bottomBarGridLayout = new QGridLayout(this);
    bottomBarGridLayout->addWidget(m_pageSlider, 0,0);
    bottomBarGridLayout->addWidget(m_currentPage, 0,1);
    bottomBarGridLayout->addWidget(m_prevPageButton, 0,2);
    bottomBarGridLayout->addWidget(m_nextPageButton, 0,3);
    bottomBarGridLayout->addWidget(m_rotateButton, 0, 4);
    bottomBarGridLayout->addWidget(m_zoomOutButton, 0, 5);
    bottomBarGridLayout->addWidget(m_zoomInButton, 0, 6);
    setLayout(bottomBarGridLayout);
}

/* Update du slider */
void BottomBar::updatePageSlider(int page, int maxPage)
{
    m_pageSlider->setValue(page);
    m_pageSlider->setMaximum(maxPage-1);
}

/* Update du label */
void BottomBar::updatePageIndicator(int page, int maxPage)
{
    m_currentPage->setText(QString("%1/%2").arg(page+1).arg(maxPage));
}

