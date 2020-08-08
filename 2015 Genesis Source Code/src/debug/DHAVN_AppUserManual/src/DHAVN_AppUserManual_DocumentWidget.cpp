#include <QtGui>
#include <QtXml/QDomElement>
#include <QFile>
#include <QTextStream>
#include "DHAVN_AppUserManual_DocumentWidget.h"

#define SCALE_FACTOR 2//1.287
#define PAGE_DEFAULT_INDEX -1

AppUserManual_DocumentWidget :: AppUserManual_DocumentWidget( AppUserManual_PDFController* pdf_Viewer, QWidget* parent )
    : QLabel( parent )

{
    LW_LOG( "AppUserManual_DocumentWidget :: Document_AppUserManual Constructor Entry" );

    pdf_doc = 0;
    m_pPdfViewer = pdf_Viewer;
    currentPage = PAGE_DEFAULT_INDEX;
    currentDisplayPage = 1;
    scaleFactor = SCALE_FACTOR;
    mousePressXCoordinate = 0;
    mousePressYCoordinate = 0;
    mousePressX = 0;
    mousePressY = 0;
    mouseReleaseX = 0;
    mouseReleaseY = 0;
    deltaX = 0;
    deltaY = 0;
    deltaTempX = 0;
    deltaTempY = 0;
    searchPage = 1;
    m_nStartPage = 1;
    m_bResetPage = false;
    m_bStopSearch = false;
    m_bDrs = false;
    m_bNumericKeypad = false;
    m_bTouchLock = false;
    m_nCountryVariant = AppEngineBase::GetCountryVariant();
    m_nLanguage = AppEngineBase::GetLanguage();

    isMouseMove = false;
    m_bFullScreen = false;

    setAlignment( Qt::AlignCenter );

    setDocument();

    LW_LOG( "AppUserManual_DocumentWidget :: Document_AppUserManual Constructor Exit" );
}

AppUserManual_DocumentWidget :: ~AppUserManual_DocumentWidget()
{
    LW_LOG( "AppUserManual_DocumentWidget :: DHAVN_AppUserManual_Document Destructor Entry" );
    if( pdf_doc != NULL )
    {
        delete pdf_doc;
        pdf_doc = NULL;
    }

    if( m_pPdfViewer != NULL )
    {
        delete m_pPdfViewer;
        m_pPdfViewer = NULL;
    }
    currentPage = PAGE_DEFAULT_INDEX;
    currentDisplayPage = 1;
    LW_LOG( "AppUserManual_DocumentWidget :: DHAVN_AppUserManual_Document Destructor Exit" );
}

void AppUserManual_DocumentWidget::setLanguage( int language )
{
    LW_LOG( "cv: " << m_nCountryVariant << ", language: " << language );
    m_nLanguage = (LANGUAGE_T) language;
    setDocument();
}

Poppler::Document* AppUserManual_DocumentWidget :: document()
{
    if( pdf_doc != NULL )
        return pdf_doc;
    return NULL;
}

qreal AppUserManual_DocumentWidget :: scale() const
{
    return scaleFactor;
}

void AppUserManual_DocumentWidget :: showPage( int page )
{
    LW_LOG( ">>>>>> ShowPage() Entry" );

    if( page != PAGE_DEFAULT_INDEX && page != currentPage + 1 )
    {
        currentPage = page - 1;
    }

//    QImage page_Image = pdf_doc->page( currentPage )->renderToImage( scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY() );
    QImage page_Image = pdf_doc->page( currentPage )->renderToImage( scaleFactor* logicalDpiX(), scaleFactor * logicalDpiX() );

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = page_Image.copy(highlightRect);
        QPainter painter;
        painter.begin(&highlight);
        painter.fillRect(highlight.rect(), QColor(124, 189, 255, 150));
        painter.end();

        QPainter painter1;
        painter1.begin(&page_Image);
        painter1.drawImage(highlightRect, highlight);
        painter1.end();
    }

    setPixmap( QPixmap :: fromImage( page_Image ) );

//    m_pPdfViewer->sendCurrentPage( page );
   HS_LOG( "<<<<<< ShowPage() Exit - page: " << currentPage );
}

void AppUserManual_DocumentWidget :: showSearchPage( )
{
    LW_LOG( ">>>>>> showSearchPage() Entry" );

    if( searchPage != PAGE_DEFAULT_INDEX && searchPage != currentPage + 1 )
    {
        currentPage = searchPage - 1;
    }

    QImage page_Image = pdf_doc->page( currentPage )->renderToImage( scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY() );

    if (!searchLocation.isEmpty()) {
        QRect highlightRect = matrix().mapRect(searchLocation).toRect();
        highlightRect.adjust(-2, -2, 2, 2);
        QImage highlight = page_Image.copy(highlightRect);
        QPainter painter;
        painter.begin(&highlight);
        painter.fillRect(highlight.rect(), QColor(124, 189, 255, 150));
        painter.end();

        QPainter painter1;
        painter1.begin(&page_Image);
        painter1.drawImage(highlightRect, highlight);
        painter1.end();
    }

    setPixmap( QPixmap :: fromImage( page_Image ) );

//    m_pPdfViewer->sendCurrentPage( page );
    LW_LOG( "<<<<<< showSearchPage() Exit" );
}

bool AppUserManual_DocumentWidget :: setDocument( )
{
    LW_LOG( "setDocument Entry" );
    QString str;
    switch ( m_nCountryVariant ) {
    case 0:         // Korea
    {
        str = "/app/share/AppUserManual/E_Manual_KR_KO.pdf";
        /*
            switch( m_nLanguage )
            {
            case LANGUAGE_EN_US:
            case LANGUAGE_EN_GB:
                str = "/app/share/AppUserManual/E_Manual_KR_EN.pdf";
                break;
            case LANGUAGE_KO:
            default:
                str = "/app/share/AppUserManual/E_Manual_KR_KO.pdf";
                break;
            }
            */
    }
    break;
    case 1:     // N.A
    case 6:     // Canada
    {
            switch ( m_nLanguage ) {
            case LANGUAGE_KO:
                str = "/app/share/AppUserManual/E_Manual_NA_KO.pdf";
                break;
            case LANGUAGE_FR_NA:
                str = "/app/share/AppUserManual/E_Manual_NA_FR.pdf";
                break;
            case LANGUAGE_ES_NA:
                str = "/app/share/AppUserManual/E_Manual_NA_ES.pdf";
                break;
            case LANGUAGE_EN_US:
            default:
                str = "/app/share/AppUserManual/E_Manual_NA_EN.pdf";
                break;
            }
    }
    break;

    case 2: //China  - heyonil.shin
    {
        switch ( m_nLanguage )
        {
            case LANGUAGE_ZH_CN:
            case LANGUAGE_ZH_YUE:
                str = "/app/share/AppUserManual/E_Manual_CN_CN.pdf";
                break;
            case LANGUAGE_KO:
                str = "/app/share/AppUserManual/E_Manual_CN_KO.pdf";
                break;
            default:
                str = "/app/share/AppUserManual/E_Manual_CN_EN.pdf";
                break;
        }
    }
    break; //heyonil.shin

    case 3: // General
    {
        switch( m_nLanguage )
        {
        case LANGUAGE_EN_US:
        case LANGUAGE_EN_GB:
            str = "/app/share/AppUserManual/E_Manual_GE_EN.pdf";
            break;
        case LANGUAGE_KO:
        default:
            str = "/app/share/AppUserManual/E_Manual_GE_KO.pdf";
            break;
        }
     }
        break;
    case 4:     // MiddleEast
    {
        if ( AppEngineBase::GetSystemVariant() & 0x1 ) {
            switch ( m_nLanguage )
            {
            case LANGUAGE_EN_GB:
                str = "/app/share/AppUserManual/E_Manual_ME_EN.pdf";
                break;
            case LANGUAGE_FR:
                str = "/app/share/AppUserManual/E_Manual_ME_FR.pdf";
                break;
            case LANGUAGE_KO:
                str = "/app/share/AppUserManual/E_Manual_ME_KO.pdf";
                break;
            case LANGUAGE_AR:
            default:
                str = "/app/share/AppUserManual/E_Manual_ME_AR.pdf";
                break;
            }
        }
        else {
            switch ( m_nLanguage )
            {
            case LANGUAGE_EN_GB:
                str = "/app/share/AppUserManual/E_Manual_ME_AV_EN.pdf";
                break;
            case LANGUAGE_FR:
                str = "/app/share/AppUserManual/E_Manual_ME_AV_FR.pdf";
                break;
            case LANGUAGE_KO:
                str = "/app/share/AppUserManual/E_Manual_ME_AV_KO.pdf";
                break;
            case LANGUAGE_AR:
            default:
                str = "/app/share/AppUserManual/E_Manual_ME_AV_AR.pdf";
                break;
            }
        }
    }
    break;

    case 5:     // Europe
    case 7:     // Russia
    {
            switch ( m_nLanguage )
            {
            case LANGUAGE_FR:
                str = "/app/share/AppUserManual/E_Manual_EU_FR.pdf";
                break;
            case LANGUAGE_ES:
                str = "/app/share/AppUserManual/E_Manual_EU_ES.pdf";
                break;
            case LANGUAGE_RU:
                str = "/app/share/AppUserManual/E_Manual_EU_RU.pdf";
                break;
            case LANGUAGE_PT:
                str = "/app/share/AppUserManual/E_Manual_EU_PT.pdf";
                break;
            case LANGUAGE_DE:
                str = "/app/share/AppUserManual/E_Manual_EU_DE.pdf";
                break;
            case LANGUAGE_PL:
                str = "/app/share/AppUserManual/E_Manual_EU_PL.pdf";
                break;
            case LANGUAGE_IT:
                str = "/app/share/AppUserManual/E_Manual_EU_IT.pdf";
                break;
            case LANGUAGE_NL:
                str = "/app/share/AppUserManual/E_Manual_EU_NL.pdf";
                break;
            case LANGUAGE_SV:
                str = "/app/share/AppUserManual/E_Manual_EU_SV.pdf";
                break;
            case LANGUAGE_TR:
                str = "/app/share/AppUserManual/E_Manual_EU_TR.pdf";
                break;
            case LANGUAGE_CS:
                str = "/app/share/AppUserManual/E_Manual_EU_CS.pdf";
                break;
            case LANGUAGE_DA:
                str = "/app/share/AppUserManual/E_Manual_EU_DA.pdf";
                break;
            case LANGUAGE_SK:
                str = "/app/share/AppUserManual/E_Manual_EU_SK.pdf";
                break;
            case LANGUAGE_KO:
                str = "/app/share/AppUserManual/E_Manual_EU_KO.pdf";
                break;
            case LANGUAGE_EN_GB:
            default:
                str = "/app/share/AppUserManual/E_Manual_EU_EN.pdf";
                break;
            }
    }
    break;
    default:        // KR
        str = "/app/share/AppUserManual/E_Manual_KR_KO.pdf";
    }

    Poppler :: Document* prev_Doc = pdf_doc;
    pdf_doc = Poppler :: Document :: load( str );
    if( pdf_doc )
    {
        delete prev_Doc;
        prev_Doc = NULL;
        pdf_doc->setRenderHint( Poppler :: Document :: Antialiasing, true );
        pdf_doc->setRenderHint( Poppler :: Document :: TextAntialiasing, true );
        currentPage = PAGE_DEFAULT_INDEX;
        currentDisplayPage = 1;
        totalPages = pdf_doc->numPages();
    }

    LW_LOG( "setDocument Exit" );

    return pdf_doc != 0;
}

void AppUserManual_DocumentWidget :: setpage(int page)
{
    LW_LOG( ">>>>>> setPage Entry" );

//    if( page != (currentPage + 1) )
    {
        searchLocation = QRectF();
        showPage( page );
    }

    LW_LOG( "<<<<<< setPage Exit" );
}

void AppUserManual_DocumentWidget :: setPageThread(int page)
{
    LW_LOG( ">>>>>> setPageThread Entry" );

    showPage( page );

    LW_LOG( "<<<<<< setPageThread Exit" );
}

void AppUserManual_DocumentWidget :: setScale( qreal scale )
{
    LW_LOG( ">>>>>> setScale Entry" );

    if( scaleFactor != scale )
    {
        scaleFactor = scale;
        showPage();
    }

    LW_LOG( "<<<<<< setScale Exit" );
}

void AppUserManual_DocumentWidget :: setNextPage()
{
    if( currentDisplayPage < pdf_doc->numPages() )
    {
        ++currentDisplayPage;
        setpage(currentDisplayPage);
    }
    else
    {
        currentDisplayPage = 1;
        setpage(currentDisplayPage);
    }
}

void AppUserManual_DocumentWidget :: setPrevPage()
{
    if( currentDisplayPage == 1 )
        currentDisplayPage = pdf_doc->numPages();
    else
        --currentDisplayPage;

    setpage(currentDisplayPage);
}

int AppUserManual_DocumentWidget :: setGoToPageThread(int page)
{
    LW_LOG( ">>>>>> setGoToPageThread - page : " << page );
    currentDisplayPage += page;
    if ( currentDisplayPage >  pdf_doc->numPages() ) currentDisplayPage -= pdf_doc->numPages();
    else if ( currentDisplayPage < 1 ) currentDisplayPage += pdf_doc->numPages();
//    setpage(currentDisplayPage);

    return currentDisplayPage;
}

int AppUserManual_DocumentWidget :: setNextPageThread()
{
    LW_LOG( ">>>>>> setNextPageThread " );
    if( currentDisplayPage < pdf_doc->numPages() ) ++currentDisplayPage;
    else currentDisplayPage = 1;

    return currentDisplayPage;
}

int AppUserManual_DocumentWidget :: setPrevPageThread()
{
    LW_LOG( ">>>>>> setPrevPageThread " );
    if( currentDisplayPage == 1 ) currentDisplayPage = pdf_doc->numPages();
    else --currentDisplayPage;

    return currentDisplayPage;
}

void AppUserManual_DocumentWidget :: mousePressEvent( QMouseEvent *event )
{
    if ( m_bTouchLock ) return;
    LW_LOG( "AppUserManual_DocumentWidget ::  mousePressEvent " );

//    LW_LOG( "####TEST##### - [global] x: " << event->globalX() << " , y: " << event->globalY() << " / [pos] x: " << event->pos().x()  << " , y : " << event->pos().y() );
    isMouseMove = false;

    m_pPdfViewer->setFullscreenTimer();         // stop timer
    mousePressXCoordinate = event->globalX();
    mousePressYCoordinate = event->globalY();
    mousePressX = event->globalX();
    mousePressY = event->globalY();
}

void AppUserManual_DocumentWidget :: mouseMoveEvent( QMouseEvent *event )       // globalX/Y : 현재 화면에 보이는 좌표(LCD 좌표), pos().x/y: PDF 이미지의 좌표
{    
    if ( m_bTouchLock ) return;
    if ( m_bNumericKeypad ) return;       // 숫자 키패드 show 상태면 touch disable
    isMouseMove = true;

    deltaX = event->globalX() - mousePressXCoordinate;
    deltaY = event->globalY() - mousePressYCoordinate;

    deltaTempX = deltaX;
    deltaTempY = deltaY;
//    LW_LOG( "####dragWidget##### - [global] x: " << event->globalX() << " , y: " << event->globalY() << " / [pos] x: " << event->pos().x()  << " , y : " << event->pos().y() );
    mousePressXCoordinate = event->globalX();
    mousePressYCoordinate = event->globalY();

    if( m_pPdfViewer ) {
        if ( mousePressYCoordinate < 190 ) {
            // 전체화면 여부 check,  fullScreen인 경우 dragWiget 함수 호출
            if ( m_bFullScreen )  m_pPdfViewer->dragWidgetTouch( deltaX, deltaY );
        }
        else m_pPdfViewer->dragWidgetTouch( deltaX, deltaY );
    }
}

void AppUserManual_DocumentWidget :: mouseReleaseEvent( QMouseEvent *event )
{
    if ( m_bTouchLock ) return;
    LW_LOG( "AppUserManual_DocumentWidget ::  mouseReleaseEvent " );
    Q_UNUSED( event )

    mouseReleaseX = event->globalX();
    mouseReleaseY = event->globalY();

    if ( m_bNumericKeypad ) ;       // 숫자 키패드 show 상태면 touch disable
    else if ( !isMouseMove )        // fullScreen 전환
    {
        if (mouseReleaseY < 190 ) {
            if (m_pPdfViewer != NULL) m_pPdfViewer->setFullScreen(true);                         // statusbar, modearea
        }
        else {
            if (m_pPdfViewer != NULL) m_pPdfViewer->setFullScreen(false);                         // 그외 영역
        }
    }
    if( m_pPdfViewer ) {
        if ( !m_bFullScreen && mouseReleaseY < 190 ) ;       // statusbar, titlearea beep 미출력
        else m_pPdfViewer->playBeep();         // statusbar, titlearea 이외 영역 beep 출력
    }
    isMouseMove = false;
}

void AppUserManual_DocumentWidget :: updatePageNumbers(int &currentPag, int &totalPages)
{
    LW_LOG( "AppUserManual_DocumentWidget :: updatePageNumbers() - currentDisplayPage : " << currentDisplayPage );
    currentPag = currentDisplayPage;
    if( pdf_doc )
        totalPages = pdf_doc->numPages();
}

void AppUserManual_DocumentWidget :: updatePDFTitle(QString &pdfTitle) const
{
    if( pdf_doc )
        pdfTitle = pdf_doc->info("Title");
}

QDomDocument* AppUserManual_DocumentWidget :: getTOC()
{
    if( pdf_doc )
        return pdf_doc->toc();
    else
        return NULL;
}

void AppUserManual_DocumentWidget :: gotoPage(int pageNumber)
{
    currentDisplayPage = pageNumber;
    setpage(currentDisplayPage);
}

QMatrix AppUserManual_DocumentWidget :: matrix() const
{
    return QMatrix(scaleFactor * physicalDpiX() / 72.0, 0,
                   0, scaleFactor * physicalDpiY() / 72.0,
                   0, 0);
}

void AppUserManual_DocumentWidget::resetSearchPage()
{
    LW_LOG( "resetSearchPage() - currentPage: -1 ");
    currentPage = PAGE_DEFAULT_INDEX;
}

//void AppUserManual_DocumentWidget :: searchForward(QString& searchText , bool bFirst )
bool AppUserManual_DocumentWidget :: searchForward(QString& searchText , bool bFirst )
{
    LW_LOG( "searchForward : " << searchText << " , bFirst : " << bFirst );
//    searchF(searchText , bFirst );
    return searchF(searchText , bFirst );
}

void AppUserManual_DocumentWidget::setTouchLock( bool lock )
{
    LW_LOG( "DHAVN_AppUserManual_DocumentWidget :: setTouchLock - " << lock );
    m_bTouchLock = lock;
}

void AppUserManual_DocumentWidget::stopSearch(bool stopSearch )
{
    HS_LOG( "stopSearch");
    m_bStopSearch = stopSearch;
}

//void AppUserManual_DocumentWidget :: searchF(QString& searchText , bool bFirst )
bool AppUserManual_DocumentWidget :: searchF(QString& searchText , bool bFirst )
{
    HS_LOG( "bFirst: " << bFirst << ", currentPage: " << currentPage );
    if ( currentPage == PAGE_DEFAULT_INDEX )
        currentPage = 0;
    int page = currentPage;
    if ( bFirst ) {
//        m_nStartPage = page;
        m_bResetPage = false;
    }
    LW_LOG( "searchF 2- bFirst: " << bFirst << ", currentPage: " << currentPage << ", m_nStartPage : " << m_nStartPage );

    if( pdf_doc )
    {
        while (page < pdf_doc->numPages())
        {
            if (pdf_doc->page(page)->search(searchText, searchLocation,
                Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
            {
                if (!searchLocation.isNull())
                {
                    searchPage = page + 1 ;
                    currentDisplayPage = searchPage;
                    return true;
                }
            }
            page += 1;
//            if ( m_bResetPage && page > m_nStartPage ) return false;      // looping
            searchLocation = QRectF();
            if (m_bStopSearch) {        // threadQuit() 호출 상태일 경우 검색 종료
                HS_LOG( ">>>>>>  searchF - stopSearch" );
                m_bStopSearch = false;
                return false;
            }
        }

        page = 0;
        m_bResetPage = true;

        while (page < currentPage)
        {
            searchLocation = QRectF();

            if (pdf_doc->page(page)->search(searchText, searchLocation,
                Poppler::Page::NextResult, Poppler::Page::CaseInsensitive))
            {
                if (!searchLocation.isNull())
                {
                    searchPage = page + 1 ;
                    currentDisplayPage = searchPage;
                    return true;
                }
            }
            page += 1;
//            if ( m_bResetPage && page > m_nStartPage ) return false;      // looping
            if (m_bStopSearch) {        // threadQuit() 호출 상태일 경우 검색 종료
                HS_LOG( ">>>>>>  searchF - stopSearch2" );
                m_bStopSearch = false;
                return false;
            }
        }
    }

    LW_LOG( "SearchF - return false " );
    return false;
}

bool AppUserManual_DocumentWidget :: searchBackward(QString &searchText  )
{
    LW_LOG( "searchBackward" << searchText );
    return searchB( searchText );
}

bool AppUserManual_DocumentWidget :: searchB(QString &searchText )
{
    LW_LOG( "searchText" << searchText );

    QRectF oldLocation = searchLocation;

    int page = currentPage;
    if (oldLocation.isNull())
        page -= 1;

    if( pdf_doc )
    {
        while (page > PAGE_DEFAULT_INDEX) {

            QList<QRectF> locations;
            searchLocation = QRectF();

            while (pdf_doc->page(page)->search(searchText, searchLocation,
                Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) { //hyeonil.shin ITS 0230398
                //Poppler::Page::PreviousResult , Poppler::Page::CaseInsensitive)) {

                if (searchLocation != oldLocation)
                    locations.append(searchLocation);
                else
                    break;
            }

            int index = locations.indexOf(oldLocation);
            if (index == PAGE_DEFAULT_INDEX && !locations.isEmpty()) {
                searchLocation = locations.last();
                searchPage = page + 1 ;
                currentDisplayPage = searchPage;
                return true;
            } else if (index > 0) {
                searchLocation = locations[index - 1];
                searchPage = page + 1 ;
                currentDisplayPage = searchPage;
                return true;
            }

            oldLocation = QRectF();
            page -= 1;
            if (m_bStopSearch) {        // threadQuit() 호출 상태일 경우 검색 종료
                HS_LOG( ">>>>>>  searchB - stopSearch" );
                m_bStopSearch = false;
                return false;
            }
        }

//        if (currentPage == pdf_doc->numPages() - 1)
//            return false;

        oldLocation = QRectF();
        page = pdf_doc->numPages() - 1;

        while (page > currentPage) {

            QList<QRectF> locations;
            searchLocation = QRectF();

            while (pdf_doc->page(page)->search(searchText, searchLocation,
                Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) { //hyeonil.shin ITS 0230398
                //Poppler::Page::PreviousResult , Poppler::Page::CaseInsensitive)) {

                locations.append(searchLocation);
            }

            if (!locations.isEmpty()) {
                searchLocation = locations.last();
                searchPage = page + 1 ;
                currentDisplayPage = searchPage;
                return true;
            }
            page -= 1;
            if (m_bStopSearch) {        // threadQuit() 호출 상태일 경우 검색 종료
                HS_LOG( ">>>>>>  searchB - stopSearch2" );
                m_bStopSearch = false;
                return false;
            }
        }
    }
    return false;
}

void AppUserManual_DocumentWidget::isFullScreen(bool status)
{
    m_bFullScreen = status;
}

void AppUserManual_DocumentWidget::setNumericKeypad( bool status )
{
    m_bNumericKeypad = status;
    LW_LOG( "setNumericKeypad() - m_bNumericKeypad: " << m_bNumericKeypad );
}

void AppUserManual_DocumentWidget::setDrs( bool status )
{
    m_bDrs = status;
    LW_LOG( "setDrs() - m_bDrs: " << m_bDrs );
}
