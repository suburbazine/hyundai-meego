#ifndef DHAVN_APPUSERMANUAL_DOCUMENTWIDGET_H
#define DHAVN_APPUSERMANUAL_DOCUMENTWIDGET_H

#include <QLabel>
#include <QRect>
#include <poppler-qt4.h>
#include <DHAVN_AppUserManual_Log_Def.h>
#include "DHAVN_AppUserManual_PDFController.h"
#include <DHAVN_AppFramework_AppEngineBase.h>

class AppUserManual_PDFController;

class AppUserManual_DocumentWidget : public QLabel
{
    Q_OBJECT
public:
    AppUserManual_DocumentWidget( AppUserManual_PDFController* pdf_Viewer, QWidget* parent = 0 );
    ~AppUserManual_DocumentWidget();
    void updatePageNumbers( int &currentPag, int &totalPages );
    void updatePDFTitle( QString& pdfTitle ) const;
    QDomDocument* getTOC();
    int totalNumPages() { return totalPages; }
    void showSearchPage();
    void stopSearch(bool);
    void setTouchLock( bool lock );
    void resetSearchPage();
    void setPageThread( int page);
    void setLanguage( int language );
    int setNextPageThread();
    int setPrevPageThread();
    int setGoToPageThread( int page );

public slots:
    bool setDocument( );
    void setpage( int page = -1 );
    void setDrs( bool status );
    void isFullScreen( bool status );
    void setNumericKeypad( bool status );
    void setScale( qreal scale );
    void setNextPage();
    void setPrevPage();
    void gotoPage( int pageNumber );
//    void searchForward(QString& searchText , bool bFirst );
    bool searchForward(QString& searchText , bool bFirst );
    bool searchBackward(QString& searchText );

protected:
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );

private:
    void showPage( int page = -1 );
    Poppler::Document *document();
    QMatrix matrix() const;
    qreal scale() const;
    //void searchF(QString& searchText , bool bFirst );
    bool searchF(QString& searchText , bool bFirst );
    bool searchB(QString& searchText );

private:
    Poppler::Document* pdf_doc;
    AppUserManual_PDFController* m_pPdfViewer;
    qreal scaleFactor;

    ECountryVariant m_nCountryVariant;
    LANGUAGE_T m_nLanguage;
    int currentDisplayPage;
    int currentPage;
    int totalPages;
    int mousePressXCoordinate;
    int mousePressYCoordinate;
    int mousePressX;
    int mousePressY;
    int mouseReleaseX;
    int mouseReleaseY;
    int deltaX;
    int deltaY;
    int deltaTempX;
    int deltaTempY;
    QRectF searchLocation;
    bool isMouseMove;
    int searchPage;
    int m_nStartPage;
    bool m_bResetPage;
    bool m_bStopSearch;
    bool m_bTouchLock;
    bool m_bDrs;
    bool m_bNumericKeypad;
    bool m_bFullScreen;
};

#endif // DHAVN_APPUSERMANUAL_DOCUMENTWIDGET_H
