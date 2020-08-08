#include <QGraphicsProxyWidget>
#include "DHAVN_AppUserManual_PDFController.h"
#include "DHAVN_AppUserManual_DocumentWidget.h"
#include "DHAVN_AppUserManual_SearchThread.h"
#include "DHAVN_AppUserManual_NextPrevPageThread.h"

#define PDF_X_COORDINATE 0
#define PDF_Y_COORDINATE 0
#define PDF_WIDTH 1280
#define PDF_HEIGHT 720
#define PAGE_WIDTH 1280//1046
#define PAGE_HEIGHT 720
#define ZOOM_DELTA  100
#define ZOOM 0
#define MAX_ZOOM 1
#define MIN_ZOOM 2
//#define X2      1256
//#define X3      1508
//#define X4      1810
//#define X5      2172
//#define X6      2605
//#define X7      3127
//#define X8      4184
#define X2      1536//1256
#define X3      1843//1508
#define X4      2212//1810
#define X5      2654//2172
#define X6      3185//2605
#define X7      3822//3127
#define X8      5120//4184
#define Y2      864
#define Y3      1037
#define Y4      1244
#define Y5      1493
#define Y6      1792
#define Y7      2150
#define Y8      2880

AppUserManual_PDFController::AppUserManual_PDFController(QDeclarativeItem *parent)
    : QDeclarativeItem(parent) , m_pSearchThread(NULL), m_pPageThread(NULL)
{
    LW_LOG( ">>>>>> PDF Controller Constructor Entry" );

//    if( !m_pPdfDoc )
    {
        m_pPdfDoc = new AppUserManual_DocumentWidget(this);
        m_pPdfDoc->setMinimumSize( PAGE_WIDTH, PAGE_HEIGHT );
        m_pPdfDoc->setScaledContents( true );
    }
    m_bFirst = true;
    m_bStopThread = false;
    m_pStartNextPage = false;
    m_bSearchRect = false;
    m_pSearchThread = new SearchThread( this , m_pPdfDoc );
    m_pPageThread = new PageThread( this , m_pPdfDoc );
    connect ( m_pSearchThread, SIGNAL(searchCompleted(bool)), this, SLOT(slotUpdatePageNumbers(bool)));
    connect ( m_pPageThread, SIGNAL(signalPageThreadCompleted(int)), this, SLOT(slotPageThreadCompleted(int)));
//    connect ( m_pSearchThread, SIGNAL(finished()), this, SLOT(initThread()));

//    if( !m_scrollArea )
    {
        m_scrollArea = new QScrollArea();
        m_scrollArea->setBackgroundRole( QPalette::Dark );
        m_scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        m_scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        m_scrollArea->setAlignment( Qt::AlignCenter );
        m_scrollArea->setWidgetResizable( false );
        m_scrollArea->setGeometry( PDF_X_COORDINATE, PDF_Y_COORDINATE, PDF_WIDTH, PDF_HEIGHT );
        m_scrollArea->setWidget( m_pPdfDoc );
    }

//    if( !m_pProxy )
    {
        m_pProxy = new QGraphicsProxyWidget(this);
        m_pProxy->setPalette(QPalette( QColor( Qt::GlobalColor( Qt::transparent ))));
        m_pProxy->setWidget( m_scrollArea );
    }


    m_pageWidth = PAGE_WIDTH;
    m_pageHeight = PAGE_HEIGHT;
    zoomStatus = MIN_ZOOM;

    m_totalPages = 0;
    m_currentPage = 0;
    m_tableOfContents = NULL;
    m_pNextPrevPage = 0;

    LW_LOG( "<<<<<< PDF Controller Constructor Exit" );
}

AppUserManual_PDFController::~AppUserManual_PDFController()
{
    LW_LOG( "~PDF COntroller" );
    if(m_titleList.count() > 0)
        m_titleList.clear();

    if(m_contentsList.count() > 0)
        m_contentsList.clear();

    if(m_titlePageNum.count() > 0)
        m_titlePageNum.clear();

    if( m_title_pageNumberList.count() > 0 )
        m_title_pageNumberList.clear();

    if( m_titlePageNumbersList.count() > 0 )
        m_titlePageNumbersList.clear();

    if(m_tableOfContents)
    {
        delete m_tableOfContents;
        m_tableOfContents = NULL;
    }

    if( m_scrollArea )
    {
        delete m_scrollArea;
        m_scrollArea = NULL;
    }

    if(m_pProxy)
    {
        delete m_pProxy;
        m_pProxy = NULL;
    }

    if(m_pSearchThread != NULL)
    {
        if( m_pSearchThread->isFinished() )
        {
            disconnect ( m_pSearchThread, SIGNAL(searchCompleted(bool)), this, SLOT(slotUpdatePageNumbers(bool)));
            delete m_pSearchThread;
            m_pSearchThread = NULL;
        }
    }
}

void AppUserManual_PDFController::setLanguage( int language )
{
    LW_LOG( "setLanguage: " << language );
    m_pPdfDoc->setLanguage( language );
}

void AppUserManual_PDFController::sendCurrentPage( int page )
{
	Q_UNUSED(page);
    LW_LOG( "sendCurrentPage: " << page );
//    emit showPageNumber( m_currentPage, m_totalPages );
//    emit setCurrentPage(page);
}

void AppUserManual_PDFController::searchStringFound()
{
    HS_LOG( "searchStringFound" );
    m_bSearchRect = true;
    emit searchTextFound();
    updatePageNumbers();
}

void AppUserManual_PDFController::searchStringNotFound()
{
    HS_LOG( "searchStringNotFound" );
    m_bSearchRect = false;
    emit searchTextNotFound();
}

void AppUserManual_PDFController::searchStringEnd()
{
    HS_LOG( "searchStringEnd" );
    m_bSearchRect = false;
    emit searchTextEnd();
    clearSearchLocation();
}

void AppUserManual_PDFController::exitSearch()
{
    HS_LOG( "exitSearch" );
    m_bSearchRect = false;
}

void AppUserManual_PDFController :: slotPageThreadCompleted(int page)
{
    LW_LOG( "slotPageThreadCompleted()" );
    m_pPdfDoc->setPageThread(page);
    m_pPdfDoc->updatePageNumbers( m_currentPage, m_totalPages );
    emit showPageNumber( m_currentPage, m_totalPages );
    m_pStartNextPage = false;
    if ( m_pNextPrevPage != 0 ) {
        showNextPage();
    }
}

void AppUserManual_PDFController :: showNextPage()
{
    LW_LOG( "showNextPage()" );

    if(m_pPageThread != NULL)
    {
        if( m_pPageThread->isRunning() || m_pStartNextPage ) {
            LW_LOG( "thread isRunning " );
            m_pNextPrevPage++;
            return;
        }
    }
    if(m_pPageThread == NULL) m_pPageThread = new PageThread( this , m_pPdfDoc );

    // Screen Thread Start
    if( !(m_pPageThread->isRunning()) || !m_pStartNextPage )
    {
        LW_LOG( "thread start " );
        m_pStartNextPage = true;
        if ( m_pNextPrevPage != 0 ) {
            m_pPageThread->setNext(true , m_pNextPrevPage );
            m_pNextPrevPage = 0;
        }
        else m_pPageThread->setNext(true , 0 );
        m_pPageThread->start(QThread::HighPriority);
    }

    /* TODO : thread.cpp run() 내에 여기.. 이거.. 해야할 일 넣기 */
//    if( m_pPdfDoc )
//        m_pPdfDoc->searchForward( searchText , bFirst );
//    updatePageNumbers();


//    if( m_pPdfDoc )
//    {
//        m_pPdfDoc->setNextPage();
//        m_pPdfDoc->updatePageNumbers( m_currentPage, m_totalPages );
//        emit showPageNumber( m_currentPage, m_totalPages );
//    }
}

void AppUserManual_PDFController :: showPrevPage()
{
    LW_LOG( "showPrevPage()" );

    if(m_pPageThread != NULL)
    {
        if( m_pPageThread->isRunning() || m_pStartNextPage ) {
            LW_LOG( "thread isRunning " );
            m_pNextPrevPage--;
            return;
        }
    }
    if(m_pPageThread == NULL) m_pPageThread = new PageThread( this , m_pPdfDoc );

    // Screen Thread Start
    if( !(m_pPageThread->isRunning()) || !m_pStartNextPage )
    {
        LW_LOG( "thread start " );
        m_pStartNextPage = true;
        if ( m_pNextPrevPage != 0 ) {
            m_pPageThread->setNext(false , m_pNextPrevPage );
            m_pNextPrevPage = 0;
        }
        else m_pPageThread->setNext(false , 0 );
        m_pPageThread->start(QThread::HighPriority);
    }

//    LW_LOG( "AppUserManual_PDFController :: Show Prev Page" );
//
//    if( m_pPdfDoc )
//    {
//        m_pPdfDoc->setPrevPage();
//        m_pPdfDoc->updatePageNumbers( m_currentPage, m_totalPages );
//        emit showPageNumber( m_currentPage, m_totalPages );
//    }
}

void AppUserManual_PDFController::playBeep()
{
    LW_LOG( "playBeep()" );
    emit playAudioBeep();
}

void AppUserManual_PDFController :: setFullScreenByDrag()
{
    LW_LOG( "setFullScreenByDrag()" );
    if ( zoomStatus == MIN_ZOOM ) {
        setFullScreen(false);
    }
}

void AppUserManual_PDFController :: setFullScreen(bool statusbar)
{
    LW_LOG( "setFullScreen() - statusbar : " << statusbar );
    emit showFullScreen(statusbar);
}

void AppUserManual_PDFController :: updatePageNumbers()
{
    LW_LOG( "<Before> m_currentPage : " <<   m_currentPage );

    if( m_pPdfDoc )
    {
        m_pPdfDoc->updatePageNumbers( m_currentPage, m_totalPages );
        emit showPageNumber( m_currentPage, m_totalPages );
    }
    LW_LOG( "<After> m_currentPage : " <<   m_currentPage );
}

void AppUserManual_PDFController :: updateTitle()
{
    LW_LOG( "updateTitle" );
    if( m_pPdfDoc )
    {
        QString pdf_Title;
        m_pPdfDoc->updatePDFTitle(pdf_Title);
        emit showPDFTitle(pdf_Title);
    }
}

void AppUserManual_PDFController :: getTOC()
{
    LW_LOG( "totalPage : " << m_totalPages );
    if ( m_tableOfContents ) delete m_tableOfContents;      // memory leak
    if( m_pPdfDoc )
    {
        m_tableOfContents = m_pPdfDoc->getTOC();
        if( m_titleList.count() > 0 )
        {
            m_titleList.clear();
        }
        if( m_title_pageNumberList.count() > 0 )
        {
            m_title_pageNumberList.clear();
        }

        int cnt = 1;
        if( m_titlePageNum.count() > 0 )
            m_titlePageNum.clear();
        if( m_tableOfContents != NULL)
        {
            QDomNode n  = m_tableOfContents->firstChild();

            while (!n.isNull())
            {
                QDomElement element  = n.toElement();

                // getting the Titles of the various parts of PDF
                if(!element.isNull())
                {
                    m_titleList.append( element.tagName() );
                    m_titlePageNum.append(cnt);
                    LW_LOG("element.tagName: "<< element.tagName());
                    if( m_contentsList.count() > 0 )
                    {
                        m_contentsList.clear();
                    }
                    QDomNode child  = n.firstChild();
                    while ( !child.isNull() )
                    {
                        QDomElement childElement = child.toElement();
                        if ( !childElement.isNull() ) {
                            LW_LOG("child tagName: " << childElement.tagName() );
                            m_contentsList.append(childElement.tagName());
                        }
                        child = child.nextSibling();
                        cnt++;
                    }
                    m_title_pageNumberList.insert( element.tagName(), m_contentsList );
                }
                n = n.nextSibling();
            }
        }
        emit titlePageNumReceived( m_titlePageNum , m_pPdfDoc->totalNumPages() );
        emit titlesReceived( m_titleList );         // 각 title string
    }
}

void AppUserManual_PDFController :: gotoPage(int pageNumber)
{
    HS_LOG( "GoTo Page" << pageNumber );
    if( m_pPdfDoc )
        m_pPdfDoc->gotoPage( pageNumber );
}

void AppUserManual_PDFController :: getPageNumbers( int index )     // 우측 목록 정보 불러옴
{
    if( m_contentsList.count() > 0 )
        m_contentsList.clear();
    if( m_titlePageNumbersList.count() > 0 )
        m_titlePageNumbersList.clear();

    m_contentsList = m_title_pageNumberList.value(m_titleList.value(index));
    if ( m_contentsList.count() == 0 ) {
        m_contentsList.append(m_titleList.value(index));
    }
    LW_LOG( "index:" << index );
    emit contentsReceived( m_contentsList );         // 각 contents string

    int startPage = m_titlePageNum.operator [](index) ;
    for ( int i = 0; i < m_contentsList.count(); i++ ) {        // content의 string(북마크) 수만큼 page를 m_titlePageNumbersList에 저장
        m_titlePageNumbersList.append( startPage + i );
    }
//    emit pageNumbersReceived( m_titlePageNumbersList );
}

void AppUserManual_PDFController :: gotoPageNum( int pageNum )      // content 목록의 index 전달
{
    if( m_pPdfDoc )
        m_pPdfDoc->gotoPage( m_titlePageNumbersList.operator[]( pageNum ) );
}

void AppUserManual_PDFController :: slotUpdatePageNumbers( bool found )
{
    LW_LOG( "slotUpdatePageNumbers  "   );
    emit threadIsRunning( false );
    if ( m_bStopThread ) return;
    if ( found ) {      // 검색 결과 있을 경우
        m_pPdfDoc->showSearchPage();
        searchStringFound();
        if ( m_bFirst )
            emit setSearchPopupVisible( );      // searchview popup hide
    }
    else if ( m_bFirst ){   // 검색 결과 하나도 없을 경우
        searchStringNotFound();
        emit setSearchPopupVisible( );          // searchview popup hide
    }
    else {          // 검색 결과 더이상 없을 경우(next)
        searchStringEnd();
    }
    emit setPDFSearchPopupVisible();    // pdfscreen popup hide

    if( m_pPdfDoc )
    {
        m_pPdfDoc->updatePageNumbers( m_currentPage, m_totalPages );
        emit showPageNumber( m_currentPage, m_totalPages );
    }
//    if(m_pSearchThread != NULL)
//    {
//        if( m_pSearchThread->isFinished() )
//        {
//            disconnect ( m_pSearchThread, SIGNAL(finished()), this, SLOT(initScreenThread()));
//            delete m_pSearchThread;
//            m_pSearchThread = NULL;
//        }
//    }
    LW_LOG( "<After> m_currentPage : " <<   m_currentPage );
}

bool AppUserManual_PDFController::checkSearching()
{
    LW_LOG( "checkSearching()" );
    if(m_pSearchThread != NULL)
    {
        if( m_pSearchThread->isRunning())
            return true;
    }
    return false;
}

void AppUserManual_PDFController::resetSearchPage()
{
    m_pPdfDoc->resetSearchPage();
}

void AppUserManual_PDFController :: searchForward( QString searchText , bool bFirst )
{
    LW_LOG( "searchForward : " << searchText );

    m_bStopThread = false;
    m_pPdfDoc->stopSearch(m_bStopThread);
    if(m_pSearchThread != NULL)
    {
        if( m_pSearchThread->isRunning())
            return;
    }
    m_bFirst = bFirst;
    if(m_pSearchThread == NULL) m_pSearchThread = new SearchThread( this , m_pPdfDoc );
    m_pSearchThread->setSearchText( searchText , bFirst , true );

    // Screen Thread Start
	if( !(m_pSearchThread->isRunning()) )
	{
		emit threadIsRunning( true );
		usleep(5000);
		m_pSearchThread->start(QThread::HighPriority);
	}

    /* TODO : thread.cpp run() 내에 여기.. 이거.. 해야할 일 넣기 */
//    if( m_pPdfDoc )
//        m_pPdfDoc->searchForward( searchText , bFirst );
//    updatePageNumbers();
}

//void AppUserManual_PDFController::initThread()
//{
//    LW_LOG( "AppUserManual_PDFController :: initThread"  );
//    if(m_pSearchThread != NULL)
//    {
//        if( m_pSearchThread->isFinished() )
//        {
//            disconnect ( m_pSearchThread, SIGNAL(searchCompleted(bool)), this, SLOT(slotUpdatePageNumbers(bool)));
//            delete m_pSearchThread;
//            m_pSearchThread = NULL;
//        }
//    }
//}

void AppUserManual_PDFController::setTouchLock( bool lock )
{
    if( m_pPdfDoc )
        m_pPdfDoc->setTouchLock(lock);
}

void AppUserManual_PDFController::threadQuit()
{
    if(m_pSearchThread != NULL)
    {
        m_bStopThread = true;
        m_pPdfDoc->stopSearch(m_bStopThread);
        LW_LOG( "threadQuit() " );
    }
}

void AppUserManual_PDFController :: searchBackward( QString searchText  )
{
    LW_LOG( "searchBackward : " << searchText );

    m_bStopThread = false;
    m_pPdfDoc->stopSearch(m_bStopThread);
    if(m_pSearchThread != NULL)
    {
        if( m_pSearchThread->isRunning())
            return;
    }
    if(m_pSearchThread == NULL) m_pSearchThread = new SearchThread( this , m_pPdfDoc );
    m_bFirst = false;       // searchBackward는 bFirst가 항상 false
    m_pSearchThread->setSearchText( searchText , m_bFirst , false );

    // Screen Thread Start
    if( !(m_pSearchThread->isRunning()) )
    {
        emit threadIsRunning( true );
        usleep(5000);
        m_pSearchThread->start(QThread::HighPriority);
    }
//    if( m_pPdfDoc )
//        m_pPdfDoc->searchBackward( searchText );
}

void AppUserManual_PDFController :: clearSearchLocation()
{
    if( m_pPdfDoc )
        m_pPdfDoc->setpage();
    m_bSearchRect = false;
}

void AppUserManual_PDFController :: doubleTapZoom( )
{
    LW_LOG( "doubleTapZoom() " );
    m_pageWidth = PAGE_WIDTH * 2;
    m_pageHeight = PAGE_HEIGHT * 2;
    zoomStatus = ZOOM;

    if( m_pPdfDoc )
    {
        m_pPdfDoc->resize( m_pageWidth, m_pageHeight );
        m_pPdfDoc->update();
    }
}


void AppUserManual_PDFController :: pinchZoomWidget(  double  scale )
{
    HS_LOG( "pinchZoomWidget - scale : " << scale );

    int tmpWidth = m_pageWidth;
    int tmpHeight = m_pageHeight;

    if ( scale > 1 ) {
        m_pageWidth +=150;
        m_pageHeight = m_pageWidth*2.3/4;
    }
    else {
        m_pageWidth -= 90;
        m_pageHeight = m_pageWidth*2.3/4;
    }

    if ( m_pageWidth <= PAGE_WIDTH ) {
        zoomStatus = MIN_ZOOM;
        m_pageWidth = PAGE_WIDTH;
        m_pageHeight = PAGE_HEIGHT;
    }
    else if ( m_pageWidth >= PAGE_WIDTH*4 ) {
        zoomStatus = MAX_ZOOM;
        m_pageWidth = PAGE_WIDTH * 4;
        m_pageHeight = PAGE_HEIGHT * 4;
    }
    else zoomStatus = ZOOM;


    /*
    int tmpWidth = m_pageWidth;
    int tmpHeight = m_pageHeight;
    if ( scale < 0 ) {
        if ( zoomStatus == MIN_ZOOM ) return;
        scale = scale * -1;
        m_pageWidth = m_pageWidth - (m_pageWidth * scale * 2);
        m_pageHeight = m_pageHeight - (m_pageHeight * scale * 2);

        if ( m_pageWidth <= PAGE_WIDTH ) {
            zoomStatus = MIN_ZOOM;
            m_pageWidth = PAGE_WIDTH;
            m_pageHeight = PAGE_HEIGHT;
        }
        else zoomStatus = ZOOM;
    }
    else {
        m_pageWidth = m_pageWidth + (m_pageWidth * scale);
        m_pageHeight = m_pageHeight + (m_pageHeight * scale);

        if ( m_pageWidth >= PAGE_WIDTH*4 ) {
            zoomStatus = MAX_ZOOM;
            m_pageWidth = PAGE_WIDTH * 4;
            m_pageHeight = PAGE_HEIGHT * 4;
        }
        else zoomStatus = ZOOM;
    }
*/
    if( m_pPdfDoc )
    {
        m_pPdfDoc->resize( m_pageWidth, m_pageHeight );
        m_pPdfDoc->update();
        int hv = m_scrollArea->horizontalScrollBar()->value() ;
        int vv = m_scrollArea->verticalScrollBar()->value() ;
        m_scrollArea->horizontalScrollBar()->setValue(hv+(m_pageWidth - tmpWidth )/2);
        m_scrollArea->verticalScrollBar()->setValue(vv+(m_pageHeight - tmpHeight )/2);
    }
    emit setZoomStatus( zoomStatus );
}

int AppUserManual_PDFController :: zoomWidgetCnt( int zoomCnt )
{
    /*
#define ZOOM 0
#define MAX_ZOOM 1
#define MIN_ZOOM 2
      */
    int tmpWidth = m_pageWidth;
    int tmpHeight = m_pageHeight;
    if( zoomCnt > 0 )                    // 확대
    {
        if ( m_pageWidth >= X7 ) {
            m_pageWidth = X8;
            m_pageHeight = Y8;
        }
        else if ( m_pageWidth >= X6 ) {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }
        else if ( m_pageWidth >= X5 ) {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case 2:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }
        else if ( m_pageWidth >= X4 ) {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case 2:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case 3:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }
        else if ( m_pageWidth >= X3 ) {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case 2:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case 3:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case 4:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }
        else if ( m_pageWidth >= X2 ) {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case 2:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case 3:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case 4:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case 5:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }
        else {
            switch ( zoomCnt ) {
            case 1:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            case 2:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case 3:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case 4:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case 5:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case 6:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            default:
                m_pageWidth = X8;
                m_pageHeight = Y8;
                break;
            }
        }

        if ( m_pageWidth >= X8 ) {
            zoomStatus = MAX_ZOOM;
            m_pageWidth = X8;
            m_pageHeight = Y8;
        }
        else zoomStatus = ZOOM;
    }
    else
    {
        ///////////////////////////////////////
        //
        //             width               height
        // X1:      1046                720
        // X2:      1256                864
        // X3:      1508                1037
        // X4:      1810                1244
        // X5:      2172                1493
        // X6:      2605                1792
        // X7:      3127                2150
        // X8:      4184                2580         // max
        //
        ///////////////////////////////////////
        if ( m_pageWidth <= X2 ) {
            m_pageWidth = PAGE_WIDTH;
            m_pageHeight = PAGE_HEIGHT;
        }
        else if ( m_pageWidth <= X3 ) {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }
        else if ( m_pageWidth <= X4 ) {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case -2:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }
        else if ( m_pageWidth <= X5 ) {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case -2:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case -3:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }
        else if ( m_pageWidth <= X6 ) {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case -2:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case -3:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case -4:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }
        else if ( m_pageWidth <= X7 ) {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case -2:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case -3:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case -4:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case -5:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }
        else {
            switch ( zoomCnt ) {
            case -1:
                m_pageWidth = X7;
                m_pageHeight = Y7;
                break;
            case -2:
                m_pageWidth = X6;
                m_pageHeight = Y6;
                break;
            case -3:
                m_pageWidth = X5;
                m_pageHeight = Y5;
                break;
            case -4:
                m_pageWidth = X4;
                m_pageHeight = Y4;
                break;
            case -5:
                m_pageWidth = X3;
                m_pageHeight = Y3;
                break;
            case -6:
                m_pageWidth = X2;
                m_pageHeight = Y2;
                break;
            default:
                m_pageWidth = PAGE_WIDTH;
                m_pageHeight = PAGE_HEIGHT;
                break;
            }
        }

        if ( m_pageWidth <= PAGE_WIDTH ) {
            zoomStatus = MIN_ZOOM;
            m_pageWidth = PAGE_WIDTH;
            m_pageHeight = PAGE_HEIGHT;
        }
        else zoomStatus = ZOOM;
    }

    LW_LOG( "zoomWidget  - " << m_pageWidth << " / " << m_pageHeight );

    if( m_pPdfDoc )
    {
        LW_LOG( "zoomWidget scroll - horizon: " << m_scrollArea->horizontalScrollBar()->value() << " , vertical: " << m_scrollArea->verticalScrollBar()->value() );
        int hv = m_scrollArea->horizontalScrollBar()->value() ;
        int vv = m_scrollArea->verticalScrollBar()->value() ;
        m_pPdfDoc->resize( m_pageWidth, m_pageHeight );
        m_scrollArea->horizontalScrollBar()->setValue(hv+(m_pageWidth - tmpWidth )/2);
        m_scrollArea->verticalScrollBar()->setValue(vv+(m_pageHeight - tmpHeight )/2);
        m_pPdfDoc->update();
    }

    return zoomStatus;
}

int AppUserManual_PDFController :: zoomWidget( bool zoomIn )
{
    /*
#define ZOOM 0
#define MAX_ZOOM 1
#define MIN_ZOOM 2
      */
    int tmpWidth = m_pageWidth;
    int tmpHeight = m_pageHeight;
    if( zoomIn > 0 )                    // 확대
    {
        if ( m_pageWidth >= X7 ) {
                m_pageWidth = X8;
                m_pageHeight = Y8;
        }
        else if ( m_pageWidth >= X5 ) {
                m_pageWidth = X7;
                m_pageHeight = Y7;
        }
        else if ( m_pageWidth >= X3 ) {
                m_pageWidth = X5;
                m_pageHeight = Y5;
        }
        else {
                m_pageWidth = X3;
                m_pageHeight = Y3;
        }

        if ( m_pageWidth >= X8 ) {
            zoomStatus = MAX_ZOOM;
            m_pageWidth = X8;
            m_pageHeight = Y8;
        }
        else zoomStatus = ZOOM;
    }
    else
    {
        ///////////////////////////////////////
        //
        //             width               height
        // X1:      1046                720
        // X2:      1256                864
        // X3:      1508                1037
        // X4:      1810                1244
        // X5:      2172                1493
        // X6:      2605                1792
        // X7:      3127                2150
        // X8:      4184                2580         // max
        //
        ///////////////////////////////////////
        if ( m_pageWidth <= X3 ) {
            m_pageWidth = PAGE_WIDTH;
            m_pageHeight = PAGE_HEIGHT;
        }
        else if ( m_pageWidth <= X5) {
                m_pageWidth = X3;
                m_pageHeight = Y3;
        }
        else if ( m_pageWidth <= X7 ) {
                m_pageWidth = X5;
                m_pageHeight = Y5;
        }
        else {
                m_pageWidth = X7;
                m_pageHeight = Y7;
        }

        if ( m_pageWidth <= PAGE_WIDTH ) {
            zoomStatus = MIN_ZOOM;
            m_pageWidth = PAGE_WIDTH;
            m_pageHeight = PAGE_HEIGHT;
        }
        else zoomStatus = ZOOM;
    }

    LW_LOG( "zoomWidget  - " << m_pageWidth << " / " << m_pageHeight );

    if( m_pPdfDoc )
    {
        LW_LOG( "zoomWidget scroll - horizon: " << m_scrollArea->horizontalScrollBar()->value() << " , vertical: " << m_scrollArea->verticalScrollBar()->value() );
        int hv = m_scrollArea->horizontalScrollBar()->value() ;
        int vv = m_scrollArea->verticalScrollBar()->value() ;
        m_pPdfDoc->resize( m_pageWidth, m_pageHeight );
        m_scrollArea->horizontalScrollBar()->setValue(hv+(m_pageWidth - tmpWidth )/2);
        m_scrollArea->verticalScrollBar()->setValue(vv+(m_pageHeight - tmpHeight )/2);
        m_pPdfDoc->update();
    }

    return zoomStatus;
}

void AppUserManual_PDFController :: clearZoomStatus()
{
    LW_LOG( "clearZoomStatus" );
    zoomStatus = MIN_ZOOM;
}

void AppUserManual_PDFController::callFullScreenTimer()
{
    emit resetFullScreenTimer();
}

void AppUserManual_PDFController :: dragNextPrevPage( bool dragLeft )
{
    LW_LOG( "dragNextPrevPage() -  dragLeft: " << dragLeft );
    if ( m_pageHeight <= PAGE_HEIGHT ) {
        if ( m_bSearchRect ) {
            emit callSearchForward( dragLeft );
        }
        else {
            if ( dragLeft ) showNextPage();
            else showPrevPage();
        }
    }
    else
    {
        LW_LOG( "dragNextPrevPage() - !minZoom" );
    }
}

void AppUserManual_PDFController::setFullscreenTimer()          // stop timer
{
    LW_LOG( "setFullscreenTimer()" );
    emit stopFullscreenTimer();
}

void AppUserManual_PDFController :: dragWidgetTouch( int deltaX, int deltaY )
{
//    if ( ( deltaX > -5 && deltaX < 5) || (deltaY > -5 && deltaY < 5) ) return;
    LW_LOG( "dragWidgetTouch() x: "  << deltaX << ", y: " << deltaY );
    if ( m_pageHeight <= PAGE_HEIGHT ) {
        LW_LOG( "AppUserManual_PDFController :: here!!!!!!!!!!" );
    }
    else if( m_scrollArea )
    {
        m_scrollArea->horizontalScrollBar()->setValue( m_scrollArea->horizontalScrollBar()->value() - deltaX );
        m_scrollArea->verticalScrollBar()->setValue( m_scrollArea->verticalScrollBar()->value() - deltaY );
        m_scrollArea->update();
    }
}

void AppUserManual_PDFController::isFullScreen(bool status)
{
    m_pPdfDoc->isFullScreen(status);
}

void AppUserManual_PDFController :: dragWidgetJog( int deltaX, int deltaY )
{
    LW_LOG( "dragWidgetJog() x: "  << deltaX << ", y: " << deltaY );
    if ( m_pageHeight <= PAGE_HEIGHT ) {
        LW_LOG( "dragWidgetJog() :: here!!!!!!!!!!" );
    }
    else if( m_scrollArea )
    {
        m_scrollArea->horizontalScrollBar()->setValue( m_scrollArea->horizontalScrollBar()->value() - deltaX );
        m_scrollArea->verticalScrollBar()->setValue( m_scrollArea->verticalScrollBar()->value() - deltaY );
        m_scrollArea->update();
    }
}

void AppUserManual_PDFController :: exitZoom()
{
    LW_LOG( "exitZoom() " );
    m_pageWidth = PAGE_WIDTH;
    m_pageHeight = PAGE_HEIGHT;

    if( m_pPdfDoc )
    {
        m_pPdfDoc->resize( m_pageWidth, m_pageHeight );
        m_pPdfDoc->update();
    }
}

void AppUserManual_PDFController::setDrs( bool status )
{
    LW_LOG( "setDrs(" << status << ")" );
    m_pPdfDoc->setDrs(status);
}

void AppUserManual_PDFController::setNumericKeypad( bool status )
{
    LW_LOG( "setNumericKeypad(" << status << ")" );
    m_pPdfDoc->setNumericKeypad(status);
}

//void AppUserManual_PDFController :: searchPDF( QString searchPDFString )
//{
//    LW_LOG( "AppUserManual_PDFController :: searchPDF" << searchPDFString );
//    if( m_titleList.count() > 0 )
//    {
//        QStringList searchPDFList = m_titleList.filter( searchPDFString );
//        int listCount = searchPDFList.count();
//
//        if( listCount == 1 )
//        {
//            gotoPage( m_title_pageNumberList.value(searchPDFList.at(0)) );
//            emit launchPDFScreen();
//        }
//        else if( listCount == 0 )
//        {
//            emit searchPDFListEmpty();
//        }
//        else if( listCount > 1 )
//        {
//            emit searchPDFListReceived( searchPDFList );
//        }
//    }
//}
