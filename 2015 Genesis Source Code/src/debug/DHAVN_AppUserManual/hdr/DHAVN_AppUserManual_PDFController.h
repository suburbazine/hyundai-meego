#ifndef DHAVN_APPUSERMANUAL_PDFCONTROLLER_H
#define DHAVN_APPUSERMANUAL_PDFCONTROLLER_H

#include <QObject>
#include <QtDeclarative>
#include <QtGui>
#include <QDeclarativeItem>
#include <QtXml/QDomDocument>
#include <QScrollArea>
#include <DHAVN_AppUserManual_Log_Def.h>

class QScrollArea;
class QGraphicsProxyWidget;
class DocumentWidget;
class AppUserManual_DocumentWidget;
class SearchThread;
class PageThread;

class AppUserManual_PDFController : public QDeclarativeItem
{

Q_OBJECT

public:
AppUserManual_PDFController(QDeclarativeItem *parent =0);

~AppUserManual_PDFController();

Q_INVOKABLE void showPrevPage();
Q_INVOKABLE void showNextPage();
Q_INVOKABLE void updatePageNumbers();
Q_INVOKABLE void updateTitle();
Q_INVOKABLE void getTOC();
Q_INVOKABLE void setLanguage( int language );
Q_INVOKABLE void gotoPage(int pageNumber);
Q_INVOKABLE void getPageNumbers( int index );
Q_INVOKABLE void gotoPageNum( int pageNum );
Q_INVOKABLE void resetSearchPage();
Q_INVOKABLE void searchForward( QString searchText , bool bFirst );
Q_INVOKABLE void searchBackward( QString searchText );
Q_INVOKABLE void clearSearchLocation();
Q_INVOKABLE int zoomWidget( bool zoomIn );
Q_INVOKABLE int zoomWidgetCnt( int zoomCnt );
Q_INVOKABLE void clearZoomStatus();
Q_INVOKABLE void pinchZoomWidget( double  scale  );
Q_INVOKABLE void doubleTapZoom( );
Q_INVOKABLE void dragWidgetJog( int deltaX, int deltaY );
Q_INVOKABLE void exitZoom();
Q_INVOKABLE void setDrs(bool status);
Q_INVOKABLE void isFullScreen(bool status);
Q_INVOKABLE void setNumericKeypad(bool status);
//Q_INVOKABLE void searchPDF( QString searchPDFString );
Q_INVOKABLE void threadQuit();
Q_INVOKABLE void setTouchLock( bool lock );
Q_INVOKABLE bool checkSearching();
Q_INVOKABLE void exitSearch();

void setFullScreen(bool statusbar);
void setFullScreenByDrag();
void playBeep();
void dragWidgetTouch(int deltaX, int deltaY );
void dragNextPrevPage( bool dragLeft );
void callFullScreenTimer();
void searchStringNotFound();
void searchStringEnd();
void sendCurrentPage( int page );
void searchStringFound();
void setFullscreenTimer();

signals:
Q_SIGNALS:
    void showFullScreen(bool statusbar);
    void playAudioBeep();
    void showPageNumber( int currentPage, int totalPages );
    void showPDFTitle(QString pdfTitle);
    void titlesReceived( QStringList titleList);
    void contentsReceived( QStringList contentsList);
    void pageNumbersReceived( QList< int > pageNumberList );
    void titlePageNumReceived( QList< int > titlePageNum , int totalPages );
    void searchPDFListEmpty();
    void launchPDFScreen();
    void stopFullscreenTimer();
    void searchPDFListReceived( QStringList searchPDFStringList );
    void searchTextNotFound();
    void searchTextEnd();
    void searchTextFound();
    void setCurrentPage( int page );
    void setSearchPopupVisible();
    void setPDFSearchPopupVisible();
    void threadIsRunning( bool running );
    void resetFullScreenTimer();
    void setZoomStatus( int status );
    void callSearchForward( bool forward);

public slots:
    void slotUpdatePageNumbers(bool);
    void slotPageThreadCompleted(int);

private:
QGraphicsProxyWidget *m_pProxy;
AppUserManual_DocumentWidget* m_pPdfDoc;
SearchThread *m_pSearchThread;
PageThread * m_pPageThread;
int m_totalPages;
int m_currentPage;
int m_pageWidth;
int m_pageHeight;
bool m_bFirst;
bool m_bStopThread;
bool m_pStartNextPage;
bool m_bSearchRect;
int zoomStatus;
int m_pNextPrevPage;

QScrollArea* m_scrollArea;
QDomDocument* m_tableOfContents;
QStringList m_titleList;                                        // 좌측(title) string 목록
QStringList m_contentsList;                                        // 우측(contents) string 목록
QList< int > m_titlePageNumbersList;                        // 선택한 titlelist에 해당하는 content 목록(page들)
QHash< QString, QStringList > m_title_pageNumberList;       // 기존 존재하던거... 삭제!! ==> 다시 사용!!! 우측 목록들도 tagName 저장해야함
QList< int > m_titlePageNum;                        // m_title_pageNumberList 대신 추가. 각 title 시작 page
};


#endif // DHAVN_APPUSERMANUAL_PDFCONTROLLER_H
