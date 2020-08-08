#ifndef DHAVN_FILEOPERATIONSHELPER_H
#define DHAVN_FILEOPERATIONSHELPER_H

#include <QTimer>
#include <Qt/qdeclarativeview.h> //added by edo.lee 2012.11.04 for statusbar
#include <QStringList>
#include <QtDBus/QDBusConnection>
#include <QTranslator>
#include <QtCore>
#include <QCoreApplication> //[KOR][ITS][177753][minor](aettie.ji)
#include "usbcontroller.h"


#define STATUSBAR_NEW   1  // added by edo.lee 2012.12.04 for new statusbar
// { modified  by edo.lee 2012.12.12 for statusbar
#if STATUSBAR_NEW
#include "DHAVN_AppFramework_Def.h"
#else
#include "DHAVN_AppFramework_StatusAdapter.h"
#endif
// } modified by edo.lee 

//enum DISPLAY_T
//{
//    DISPLAY_NONE,               /**< No screen assigned to app */
//    DISPLAY_FRONT,              /**< App running on front screen */
//    DISPLAY_REAR,               /**< App running on rear screen */
//    DISPLAY_FRONT_REAR,         /**< Apps running on both screens */
//    DISPLAY_AUTO                /**< For non VEXT suport */
//};

// modified by ravikanth 10-07-13 for ITS 179182
enum EOperationReturnValue
{
    eOperationReturnFileNone = 0,
    eOperationReturnFileCopyError,
    eOperationReturnFileRemoveError,
    eOperationReturnFileAlreadyExists,
    eOperationReturnFileGeneralError
};

// { added by Sergey for CR#14494
enum EOperationStatus
{
    eOperationNone,
    eOperationInProgress,
    eOperationPaused
};
// } added by Sergey for CR#14494.

// modified for ITS 0212431
enum EOperationMediaType
{
    eOperationMediaNone,
    eOperationMediaJukeBox,
    eOperationMediaUSB
};

class FileOperationsHelper : public QObject
{
    Q_OBJECT
public:
	#if (STATUSBAR_NEW == 0) // added by edo.lee 2012.12.12 for statusbar
    explicit FileOperationsHelper(StatusAdapter *pStatusAdapter, QObject *parent = 0);
    explicit FileOperationsHelper(StatusAdapter *pStatusAdapter,
                                  USBController *controller, QObject *parent = 0);
	#endif // added by edo.lee 2012.12.12 for statusbar
    // { added by edo.lee 2012.12.04 for statusbar
    explicit FileOperationsHelper(QDeclarativeView* pOSDview, QObject *parent = 0);
    explicit FileOperationsHelper(QDeclarativeView* pOSDview,
                                  USBController *controller, QObject *parent = 0);

	void setDeclarativeView(QDeclarativeView *pOSDView);
    // } added by edo.lee

    // { modified by wspark 2012.12.14 for copying according to the media type.
    //void copy(const QList<QString>& fileList, const QString& targetFolder);
    void copy(const QList<QString>& fileList, const QString& targetFolder, int typemode = 0, ETAVolumeType volumeType = eUSB);
    // } modified by wspark
    void move(const QList<QString>& fileList, const QString& targetFolder);
    int deleteFiles(const QList<QString>& fileList, int typemode = 0, ETAVolumeType volumeType = eJukeBox); // modified by ravikanth 21.06.13 for ITS 0174571
    void renameFolder( const QString & sourcePath, const QString & targetPath );
    void cancelOperation(bool isRemoved = false);
    void continueOperation();
    void setOverwriteMode(int mode, bool continueCurrentOperation = true); // modified by ravikanth 18.06.13 for ITS 0174610
    void pauseOperation();
    void renameFile( const QString & sourcePath, const QString & targetPath );
    void createFolder( const QString & path);
    void setTranslator( QTranslator * translator );
    EOperationReturnValue saveFileSynchronously(const QString& file, const QString& targetPath, bool replaceFile = false); // modified by ravikanth 10-07-13 for ITS 179182
    void setCopySpeed(int nSpeed); // added by kihyung 2013.10.08 for ITS 0193747 
    int currentOperation();
    void setMediaType( EOperationMediaType mediaType ) { m_operationMediaType = mediaType; } // modified for ITS 0212431
    
public slots:
    void setDisplay( int disp );

signals:
    void progress(int index, int total, int copyPercentage, QString strName = QString());
    void operationCopyComplete(int skipCount, int copyFilesCount); // modified by ravikanth 22-04-13
    void operationDeleteComplete(QString path);
    void signalDeleteCompleted();
    void operationMoveComplete();
    void copyRequest(QString fileName, int totalFilesForCopy); // modified by ravikanth 07-07-13 for ITS 0178184
    void copyError(int type);
    void capacityError(int filesCount, qint64 fileSize);
    void createFolderComplete(int event);
    void editFolderComplete(int event);
    void editFileComplete(int event);
    void fileMoved(QString fileName);
    void setMoveStatus(bool state);// add by eunhye 2012.09.21 for CR 9243
    void operationStatusChanged(EOperationStatus value); // added by Sergey for CR#14494
    void fileSystemChanged(); // added by kihyung 2013.2.17
    void copyOpertaionExited(); // modified by ravikanth 19-04-13
    void operationDeleteIncomplete(); // modified by ravikanth 21.06.13 for ITS 0174571

public:
    enum OperationType
    {
        operationUnknown,
        operationCopy,
        operationDelete,
        operationMove
    };

    enum ECompleteStatus
    {
        eCompleteNormal,
        eCompleteCancel,
        eCompleteError,
        eCompleteFileError // modified by ravikanth 08-08-13 for ITS 0183321 
    };

private:
    bool sendStatusToIBox(uint copyStatus, uint copyProgress = 0);
    void notifyOSD(const QString& messageId, bool allScreens = false); // modified for ITS 0212431
    void handleCopyError(int errorCode, QString fileName);
    void connectCopySignals();
    void connectMoveSignals();
    void connectDeleteSignals();
    void disconnectCopySignals();
    void disconnectMoveSignals();
    void disconnectDeleteSignals();
    void setOperationStatus(EOperationStatus); // added by Sergey for CR#14494
    USBController* GetFileInterface(); // added by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    void notifyOSDOnCopyStart(); // modified by ravikanth 25-07-13 for ITS 0181545
    void notifyOSDOnCopyEnd();

private slots:
    void onCopyComplete(int index, int skipCount, int copyFilesCount,  int modeType, int volumeType, int currentProgressModeType, int currentProgressVolumeType); // modified by ravikanth 22-04-13
    void onDeleteComplete(int index, int modeType, int volumeType);
    void onMoveComplete(int index, int mode);
    void onError(int errorCode, QString fileName);
    void onCopyProgress(int percent, qint64 size, int copyCount); // modified by ravikanth 18-07-13 for copy index and count display
    void onDeleteProgress(int percent, qint64 size, QString strName);
    void onMoveProgress(int percent, qint64 size);

    void notifyDeleteComplete();
    void notifyCopyCanceled(); //added by Michael.Kim 2014.03.06 for ISV 98041

private:
    USBController *m_pFileInterface; // modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
    OperationType operationType;
    QScopedPointer<QDBusInterface> mostInterface;
    QString targetFolder;
    ECompleteStatus completeStatus;
    int currentErrorCode; // added by Michael.Kim 2014.03.06 for ISV 98041
    OperationType currentOperationType; // added by Michael.Kim 2014.03.06 for ISV 98041
    int currentSize;
    int totalSize;
    int currentPercent;
    int filesCount;
	#if (STATUSBAR_NEW == 0) // added  by edo.lee 2012.12.12 for statusbar
    StatusAdapter *m_pStatusAdapter;
	#endif// added  by edo.lee 2012.12.12 for statusbar
    DISPLAY_T m_nDisplay;
    QScopedPointer<QTimer> m_timer;
    QScopedPointer<QTimer> m_timerOSDNotify; //added by Michael.Kim 2014.03.06 for ISV 98041
    QTranslator *m_pTranslator;
    EOperationStatus m_eOperationStatus; // added by Sergey for CR#14494
    QDeclarativeView* m_pOSDview; //added by edo.lee 2012.12.04 for statusbar
    bool m_isDeviceRemovedOnOperation;
    int m_currentModeType;
    int m_currentVolumeType;
    bool m_bOSDOnCopyStart;
    EOperationMediaType m_operationMediaType; // modified for ITS 0212431
};

#endif // DHAVN_FILEOPERATIONSHELPER_H
