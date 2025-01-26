#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <vector>
#include <QObject>
#include <QPixmap>

/* Defining the Image type */
using Image = QPixmap;

/* Defining the Comic type */
class Comic : public QObject
{
    Q_OBJECT

private:
    std::vector<Image> m_pages;
    unsigned int m_pageNumber;               // La première page est à l'index 0 de m_pages
    unsigned int m_maxPageNumber;

public:
    Comic(): m_pageNumber(0), m_maxPageNumber(0)
    {};
    Comic(int numberOfPages): m_pageNumber(0), m_maxPageNumber(numberOfPages)
    { m_pages.resize(m_maxPageNumber); };
    void setMaxPageNumber(int maxPage){ m_maxPageNumber = maxPage; };

    unsigned int getPageNumber() { return m_pageNumber; };
    unsigned int getMaxPageNumber() { return m_maxPageNumber; };
    Image getPage() { return m_pages[m_pageNumber]; };

signals:
    void pageChanged(Image newPage, int newPageNumber, int maxPage);

public slots:
    void setPageNumber(int page);
    void incrementPageNumber();
    void decrementPageNumber();
    void addPage(Image thePage, int thePageNumber) { m_pages[thePageNumber] = thePage; };
    void refreshPage() { emit pageChanged(m_pages[m_pageNumber], m_pageNumber, m_maxPageNumber); }
};


#endif