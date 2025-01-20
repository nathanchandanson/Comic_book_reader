#include "Datatypes.hpp"

/* Defining the Comic methods */
/* Changing the page */
void Comic::setPageNumber(int page)
{
    if(m_pageNumber == page || page >= m_maxPageNumber || page < 0){ return; }

    m_pageNumber = page;
    emit pageChanged(m_pages[m_pageNumber], m_pageNumber, m_maxPageNumber);
}

/* Incrementing the page */
void Comic::incrementPageNumber()
{
    setPageNumber(m_pageNumber + 1);
}

/* Decrementing the page number */
void Comic::decrementPageNumber()
{
    setPageNumber(m_pageNumber - 1);
}