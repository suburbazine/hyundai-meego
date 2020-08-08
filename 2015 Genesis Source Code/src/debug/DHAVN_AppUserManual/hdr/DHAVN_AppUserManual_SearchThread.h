#ifndef DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H
#define DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H

#include <DHAVN_AppUserManual_Log_Def.h>
#include "DHAVN_AppUserManual_PDFController.h"
#include "DHAVN_AppUserManual_DocumentWidget.h"
#include <QObject>
#include <QThread>

class AppUserManual_PDFController;
class AppUserManual_DocumentWidget;


//class ResetSettingThread : public QThread
class SearchThread : public QThread
{
    Q_OBJECT

public:
    SearchThread(QObject *parent = 0);
    SearchThread(AppUserManual_PDFController* pdfcontroller,  AppUserManual_DocumentWidget* m_pPdfDoc ,  QObject *parent = 0);
    ~SearchThread();

    virtual void run ();
    void setSearchText( QString searchText , bool first , bool forward );

signals:
    void searchCompleted(bool found);

public slots:

private:
    AppUserManual_PDFController* m_pdfcontroller;
    AppUserManual_DocumentWidget* m_pPdfDoc;
    QString m_searchText ;
    bool m_bFirst;
    bool m_bBlock;
    bool m_bForward;
};

#endif // DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H
