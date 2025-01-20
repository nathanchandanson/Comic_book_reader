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
    void setMaxPageNumber(int maxPage){ m_maxPageNumber = maxPage; };
    void addPage(Image thePage, int thePageNumber) { m_pages.emplace(m_pages.begin() + thePageNumber, thePage); m_maxPageNumber++; };

    unsigned int getPageNumber() { return m_pageNumber; };
    unsigned int getMaxPageNumber() { return m_maxPageNumber; };
    Image getPage() { return m_pages[m_pageNumber]; };

signals:
    void pageChanged(Image newPage, int newPageNumber, int maxPage);

public slots:
    void setPageNumber(int page);
    void incrementPageNumber();
    void decrementPageNumber();
};


#endif