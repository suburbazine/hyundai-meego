#include<DHAVN_AppUserManual_NextPrevPageThread.h>

PageThread::PageThread(AppUserManual_PDFController* pdfcontroller, AppUserManual_DocumentWidget* pdfDoc, QObject *parent)
    : m_pdfcontroller(pdfcontroller), m_pPdfDoc(pdfDoc),  QThread(parent)
{
    //m_searchText(searchText) , m_bFirst(bFirst) ,
    m_bNext = true;
    m_nPage = 0;
}

PageThread::~PageThread()
{
}

void PageThread::run()
{
    int  curPage;
    if( m_pPdfDoc ) {
        if ( m_nPage != 0 ) curPage = m_pPdfDoc->setGoToPageThread(m_nPage);
        else if ( m_bNext ) curPage = m_pPdfDoc->setNextPageThread();
        else curPage = m_pPdfDoc->setPrevPageThread();
        emit signalPageThreadCompleted( curPage );
//        m_pdfcontroller->updatePageNumbers();
    }
}

void PageThread::setNext( bool next , int page )
{
    m_nPage = page;
    m_bNext = next ;
}

