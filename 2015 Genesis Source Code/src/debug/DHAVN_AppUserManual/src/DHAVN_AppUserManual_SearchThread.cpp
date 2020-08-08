#include<DHAVN_AppUserManual_SearchThread.h>

SearchThread::SearchThread(AppUserManual_PDFController* pdfcontroller, AppUserManual_DocumentWidget* pdfDoc, QObject *parent)
    : m_pdfcontroller(pdfcontroller), m_pPdfDoc(pdfDoc),  QThread(parent)
{
    //m_searchText(searchText) , m_bFirst(bFirst) ,
    m_searchText = "";
    m_bFirst = true;
    m_bBlock = false;
    m_bForward = true;
}

SearchThread::~SearchThread()
{
}

void SearchThread::run()
{
//    HS_LOG( ">>>>>>>>>>>>>>> run() " );
    bool stringFound = false;

    if( m_pPdfDoc ) {
        if ( m_bForward ) stringFound = m_pPdfDoc->searchForward( m_searchText , m_bFirst );
        else stringFound = m_pPdfDoc->searchBackward( m_searchText );
    }

    emit searchCompleted( stringFound );
//    HS_LOG( ">>>>>>>>>>>>>>> exit run()" );
//        updatePageNumbers();
}

void SearchThread::setSearchText( QString searchText , bool first , bool forward )
{
//    HS_LOG( ">>>>>>>>>>>>>>> setSearchText() searchText : " << searchText << " , first : " << first );
    m_searchText = searchText ;
    m_bFirst = first;
    m_bForward = forward;
}

