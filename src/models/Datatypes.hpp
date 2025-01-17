#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <vector>
#include <QPixmap>

/* Defining the Image type */
using Image = QPixmap;

/* Defining the Comic type */
class Comic
{
private:
    std::vector<Image> m_pages;
    int m_pageNumber;               // La première page est à l'index 0 de m_pages
    int m_maxPageNumber;

public:
    void setPageNumber(int page) { m_pageNumber = page; };
    void setMaxPageNumber(int maxPage){ m_maxPageNumber = maxPage; };
    void addPage(Image thePage, int thePageNumber) { m_pages.emplace(m_pages.begin() + thePageNumber, thePage); m_maxPageNumber++; };

    int getPageNumber() { return m_pageNumber; };
    int getMaxPageNumber() { return m_maxPageNumber; };
    Image getPage() { return m_pages[m_pageNumber]; };
};


#endif