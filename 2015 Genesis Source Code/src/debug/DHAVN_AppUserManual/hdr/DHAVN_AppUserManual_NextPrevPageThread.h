#include <DHAVN_AppUserManual_Log_Def.h>
#include "DHAVN_AppUserManual_PDFController.h"
#include "DHAVN_AppUserManual_DocumentWidget.h"
#include <QObject>
#include <QThread>

class AppUserManual_PDFController;
class AppUserManual_DocumentWidget;


//class ResetSettingThread : public QThread
class PageThread : public QThread
{
    Q_OBJECT

public:
    PageThread(QObject *parent = 0);
    PageThread(AppUserManual_PDFController* pdfcontroller,  AppUserManual_DocumentWidget* m_pPdfDoc ,  QObject *parent = 0);
    void setNext(bool next , int page );
    ~PageThread();

    virtual void run ();

signals:
    void signalPageThreadCompleted(int);

public slots:

private:
    AppUserManual_PDFController* m_pdfcontroller;
    AppUserManual_DocumentWidget* m_pPdfDoc;
    bool m_bNext;
    int m_nPage;
};
