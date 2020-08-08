#include "hdr/DHAVN_FileOperationsHelper.h"
#include "DHAVN_StatusControl_Def.h"
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QtDBus/QDBusReply>
#include "iplayerengine.h"
#include <QmlStatusBar/osdwriter.h> //modified by edo.lee 2013.05.04

#include <QDebug>


// From MOST LLD
#define COPY_READY_STATUS           0x00
#define COPYING_STATUS              0x01
#define COPY_COMPLETE_STATUS        0x02
#define COPY_LOCK_STATUS            0x03
#define COPY_ERROR_STATUS           0xFF
#define JUKEBOX_VOLUME              "/usr_data" // added by wspark 2012.12.06 for showing remaining bytes.
#define DEFAULT_SAVEFRAME_FILE1     "saveframe1" //added by kihyung 2013.08.07 for ITS 0166998
#define DEFAULT_SAVEFRAME_FILE2     "saveframe2" //added by kihyung 2013.08.07 for ITS 0166998

// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
#if (STATUSBAR_NEW==0) //added by edo.lee 2012.12.12 for statusbar
FileOperationsHelper::FileOperationsHelper(StatusAdapter *pStatusAdapter,QObject *parent) :
    QObject(parent)
  , m_pFileInterface(NULL)
  , operationType(operationUnknown)
  , currentOperationType(operationUnknown) // added by Michael.Kim 2014.03.28 for ISV 98041
  , completeStatus(eCompleteNormal)
  , currentSize(0)
  , totalSize(0)
  , currentPercent(0)
  , filesCount(0)
  , currentErrorCode(0) // added by Michael.Kim 2014.03.28 for ISV 98041
  , m_pStatusAdapter( pStatusAdapter )
  , m_nDisplay( DISPLAY_NONE )
  , m_pTranslator(0)
  , m_eOperationStatus(eOperationNone) // added by Sergey for CR#14494
  , m_isDeviceRemovedOnOperation(false)
{
    m_timer.reset( new QTimer( this ) );
    m_timer->setSingleShot( true );
    m_timer->setInterval( 1000 );

    connect( m_timer.data(), SIGNAL( timeout() ), this, SLOT( notifyDeleteComplete() ) );

    // {added by Michael.Kim 2014.03.06 for ISV 98041
    m_timerOSDNotify.reset( new QTimer( this ) );
    m_timerOSDNotify->setSingleShot( true );
    m_timerOSDNotify->setInterval( 2000 );
    connect( m_timerOSDNotify.data(), SIGNAL( timeout() ), this, SLOT( notifyCopyCanceled() ) );
    // }added by Michael.Kim 2014.03.06 for ISV 98041
}

FileOperationsHelper::FileOperationsHelper(StatusAdapter *pStatusAdapter,
                                           USBController *controller, QObject *parent) :
    QObject(parent)
  , m_pFileInterface(controller)
  , operationType(operationUnknown)
  , currentOperationType(operationUnknown) // added by Michael.Kim 2014.03.28 for ISV 98041
  , completeStatus(eCompleteNormal)
  , currentSize(0)
  , totalSize(0)
  , currentPercent(0)
  , filesCount(0)
  , currentErrorCode(0) // added by Michael.Kim 2014.03.28 for ISV 98041
  , m_pStatusAdapter( pStatusAdapter )
  , m_nDisplay( DISPLAY_NONE )
  , m_pTranslator(0)
  , m_eOperationStatus(eOperationNone) // added by Sergey for CR#14494
{
    if (this->m_pFileInterface != NULL)
    {
        connect(this->m_pFileInterface, SIGNAL(OnTotalFileDeleted(int,int,int)),
                SLOT(onDeleteComplete(int,int,int)));
        connect(this->m_pFileInterface, SIGNAL(OnTotalFileMoved(int,int)),
                SLOT(onMoveComplete(int,int)));
        connect(this->m_pFileInterface, SIGNAL(OnError(int, QString)),
                SLOT(onError(int, QString)));
        connect(this->m_pFileInterface, SIGNAL(OnPercentageAndSizeAndNameDeleted(int,qint64,QString)),
                SLOT(onDeleteProgress(int,qint64,QString)));
        connect(this->m_pFileInterface, SIGNAL(OnPercentageAndSizeMoved(int,qint64)),
                SLOT(onMoveProgress(int,qint64)));
        connect(this->m_pFileInterface, SIGNAL(OnFileMoved(QString)),
                SIGNAL(fileMoved(QString)));
        connect(this->m_pFileInterface, SIGNAL(OnCreateFolder(int)),
                SIGNAL(createFolderComplete(int)));
        connect(this->m_pFileInterface, SIGNAL(OnEditFolder(int)),
                SIGNAL(editFolderComplete(int)));
        connect(this->m_pFileInterface, SIGNAL(OnEditFile(int)),
                SIGNAL(editFileComplete(int)));
    }

    m_timer.reset( new QTimer( this ) );
    m_timer->setSingleShot( true );
    m_timer->setInterval( 1000 );

    connect( m_timer.data(), SIGNAL( timeout() ), this, SLOT( notifyDeleteComplete() ) );

    // {added by Michael.Kim 2014.03.06 for ISV 98041
    m_timerOSDNotify.reset( new QTimer( this ) );
    m_timerOSDNotify->setSingleShot( true );
    m_timerOSDNotify->setInterval( 2000 );
    connect( m_timerOSDNotify.data(), SIGNAL( timeout() ), this, SLOT( notifyCopyCanceled() ) );
    // }added by Michael.Kim 2014.03.06 for ISV 98041
    // qDebug () << "FileOperationsHelper::FileOperationsHelper,  m_pStatusAdapter = " << m_pStatusAdapter;
}
#endif // added  by edo.lee 2012.12.12 for statusbar
// { added by edo.lee 2012.12.04 for statusbar
FileOperationsHelper::FileOperationsHelper(QDeclarativeView *pOSDView,QObject *parent) :
	QObject(parent)
#if USE_USB_SINGLE_INSTANCE
  , fileInterface(USBController::GetInstance())
#else
  , m_pFileInterface(USBController::GetFirstInstance())
#endif
  , operationType(operationUnknown)
  , currentOperationType(operationUnknown) // added by Michael.Kim 2014.03.28 for ISV 98041
  , completeStatus(eCompleteNormal)
  , currentSize(0)
  , totalSize(0)
  , currentPercent(0)
  , filesCount(0)
  , currentErrorCode(0) // added by Michael.Kim 2014.03.28 for ISV 98041
  , m_pOSDview( pOSDView )
  , m_nDisplay( DISPLAY_NONE )
  , m_pTranslator(0)
  , m_eOperationStatus(eOperationNone) // added by Sergey for CR#14494
  , m_isDeviceRemovedOnOperation(false)
  , m_currentModeType(0)
  , m_currentVolumeType(0)
  , m_bOSDOnCopyStart(false)
  , m_operationMediaType(eOperationMediaNone) // modified for ITS 0212431
{
    m_timer.reset( new QTimer( this ) );
    m_timer->setSingleShot( true );
    m_timer->setInterval( 1000 );

    connect( m_timer.data(), SIGNAL( timeout() ), this, SLOT( notifyDeleteComplete() ) );

    // {added by Michael.Kim 2014.03.06 for ISV 98041
    m_timerOSDNotify.reset( new QTimer( this ) );
    m_timerOSDNotify->setSingleShot( true );
    m_timerOSDNotify->setInterval( 2000 );
    connect( m_timerOSDNotify.data(), SIGNAL( timeout() ), this, SLOT( notifyCopyCanceled() ) );
    // }added by Michael.Kim 2014.03.06 for ISV 98041
}

FileOperationsHelper::FileOperationsHelper(QDeclarativeView *pOSDView,
										   USBController *controller, QObject *parent) :
	QObject(parent)
  , m_pFileInterface(controller)
  , operationType(operationUnknown)
  , currentOperationType(operationUnknown) // added by Michael.Kim 2014.03.28 for ISV 98041
  , completeStatus(eCompleteNormal)
  , currentSize(0)
  , totalSize(0)
  , currentPercent(0)
  , filesCount(0)
  , currentErrorCode(0) // added by Michael.Kim 2014.03.28 for ISV 98041
  , m_pOSDview( pOSDView )
  , m_nDisplay( DISPLAY_NONE )
  , m_pTranslator(0)
  , m_eOperationStatus(eOperationNone) // added by Sergey for CR#14494
  , m_isDeviceRemovedOnOperation(false)
  , m_currentModeType(0)
  , m_currentVolumeType(0)
  , m_bOSDOnCopyStart(false)
  , m_operationMediaType(eOperationMediaNone) // modified for ITS 0212431
{
    if (this->m_pFileInterface != NULL)
    {
        connect(this->m_pFileInterface, SIGNAL(OnTotalFileDeleted(int,int,int)),
                SLOT(onDeleteComplete(int,int,int)));
        connect(this->m_pFileInterface, SIGNAL(OnTotalFileMoved(int,int)),
                SLOT(onMoveComplete(int,int)));
        connect(this->m_pFileInterface, SIGNAL(OnError(int, QString)),
                SLOT(onError(int, QString)));
        connect(this->m_pFileInterface, SIGNAL(OnPercentageAndSizeAndNameDeleted(int,qint64,QString)),
                SLOT(onDeleteProgress(int,qint64,QString)));
        connect(this->m_pFileInterface, SIGNAL(OnPercentageAndSizeMoved(int,qint64)),
                SLOT(onMoveProgress(int,qint64)));
        connect(this->m_pFileInterface, SIGNAL(OnFileMoved(QString)),
                SIGNAL(fileMoved(QString)));
        connect(this->m_pFileInterface, SIGNAL(OnCreateFolder(int)),
                SIGNAL(createFolderComplete(int)));
        connect(this->m_pFileInterface, SIGNAL(OnEditFolder(int)),
                SIGNAL(editFolderComplete(int)));
        connect(this->m_pFileInterface, SIGNAL(OnEditFile(int)),
                SIGNAL(editFileComplete(int)));
    }

    m_timer.reset( new QTimer( this ) );
    m_timer->setSingleShot( true );
    m_timer->setInterval( 1000 );

    connect( m_timer.data(), SIGNAL( timeout() ), this, SLOT( notifyDeleteComplete() ) );

    // {added by Michael.Kim 2014.03.06 for ISV 98041
    m_timerOSDNotify.reset( new QTimer( this ) );
    m_timerOSDNotify->setSingleShot( true );
    m_timerOSDNotify->setInterval( 2000 );
    connect( m_timerOSDNotify.data(), SIGNAL( timeout() ), this, SLOT( notifyCopyCanceled() ) );
    // {added by Michael.Kim 2014.03.06 for ISV 98041
    // qDebug () << "FileOperationsHelper::FileOperationsHelper,  m_pStatusAdapter = " << m_pStatusAdapter;

}

void FileOperationsHelper::setDeclarativeView(QDeclarativeView *pOSDView)
{
    m_pOSDview = pOSDView ;
}
// } added by edo.lee 

USBController*
FileOperationsHelper::GetFileInterface()
{
    if (!m_pFileInterface)
    {
#if USE_USB_SINGLE_INSTANCE
        m_pFileInterface = USBController::GetInstance();
#else
        m_pFileInterface = USBController::GetFirstInstance();
#endif
        if (m_pFileInterface)
        {
            connect(m_pFileInterface, SIGNAL(OnError(int, QString)), SLOT(onError(int, QString)));
            connect(m_pFileInterface, SIGNAL(OnCreateFolder(int)), SIGNAL(createFolderComplete(int)));
            connect(m_pFileInterface, SIGNAL(OnEditFolder(int)), SIGNAL(editFolderComplete(int)));
            connect(m_pFileInterface, SIGNAL(OnEditFile(int)), SIGNAL(editFileComplete(int)));
        }
    }

    return m_pFileInterface;
}

// { modified by wspark 2012.12.14 for copying according to the media type.
//void FileOperationsHelper::copy(const QList<QString> &fileList, const QString &targetFolder)
void FileOperationsHelper::copy(const QList<QString> &fileList, const QString &targetFolder, int typemode, ETAVolumeType volumeType)
// } modified by wspark
{
    qDebug () << "FileOperationsHelper::copy" << typemode << volumeType;
    if (this->mostInterface == NULL)
    {
        this->mostInterface.reset(new QDBusInterface("com.dhavn.mostmanager.qtdbus",
                                                     "/jukebox", "", QDBusConnection::sessionBus()));
        if (this->mostInterface->isValid())
        {
            qWarning() << "FileOperationsHelper::copy() unable to create MOST interface";
        }
    }

    cancelOperation();
    this->completeStatus = eCompleteNormal;

    QDir target;
    QString normalizedTragerDir = QUrl(targetFolder).path();

    if (!target.exists(normalizedTragerDir) && GetFileInterface())
    {
        GetFileInterface()->CreateFolder(normalizedTragerDir);
    }

    QStringList copyList;
    for (int i = 0; i < fileList.size(); i++)
    {
        // { modified by eugene.eseo 2013.03.25
        // copyList.append(QUrl(fileList.at(i)).path()); 
        copyList.append(QUrl(fileList.at(i)).toString(QUrl::RemoveScheme));
        // } modified by eugene.eseo 2013.03.25
    }
    if(copyList.size() == 0)
    {
        // qDebug () << "FileOperationsHelper::empty";
        emit copyError(eMP_CORRUPT_FILE_COPY_ERROR);
        return;
    }

    this->targetFolder = normalizedTragerDir;

    if (copyList.size() > 0)
    {
        sendStatusToIBox(COPY_READY_STATUS);
        // qDebug () << "FileOperationsHelper::copy() start copy";
        // { added by edo.lee 2012.12.04
#if STATUSBAR_NEW
        //m_pOSDview->setWindowTitle("@OSD:FR^MEDIA_INFO^WARN^JUKEBOX^JUKEBOX^STATUS_FILES_COPIED");
        qDebug()<<"STATUS_FILES_COPIED";
#else
        m_pStatusAdapter->setIndicatorState(STATUS_FILES_COPIED, 1);
#endif
        // } added by edo.lee
        this->filesCount = copyList.size();
        this->operationType = operationCopy;
        this->m_currentModeType = typemode;
        this->m_currentVolumeType = (int)volumeType;

        connectCopySignals();

        if (GetFileInterface() != NULL)
        {
            // GetFileInterface()->SetSystemState(SYSTEM_SLOW_COPY_SPEED); // removed by kihyung 2013.10.08 for ITS 0193747 
            // { modified by wspark 2012.12.14 for copying according to the media type.
            //GetFileInterface()->CopyFileList(copyList, normalizedTragerDir);
            if(typemode == 0)
            {
                GetFileInterface()->CopyFileList(copyList, normalizedTragerDir);
            }
            else if(typemode > 0)
            {
                GetFileInterface()->CopyFileList(copyList, normalizedTragerDir, typemode, volumeType);
            }
            // } modified by wspark

            this->totalSize = GetFileInterface()->TotalFileForCopy(); // modified by junggil 2012.07.06 for CR9513 : Wrong popup message while copying songs from USB to Jukebox
            //emit progress(0, this->totalSize, 0, "");
            emit progress(0, 0, 0, ""); // modified by ravikanth 28-08-13 for ITS 0186863
            notifyOSDOnCopyStart(); // modified by ravikanth 25-07-13 for ITS 0181545
            this->m_bOSDOnCopyStart = false; //added by Michael.Kim 2013.11.09 for ISV 93630
        }
    }
    // qDebug () << "FileOperationsHelper::size " << copyList.size();
}

void FileOperationsHelper::move(const QList<QString> &fileList, const QString &targetFolder)
{
    // qDebug () << "FileOperationsHelper::move";
    cancelOperation();
    this->completeStatus = eCompleteNormal;
    emit setMoveStatus(false);// add by eunhye 2012.09.21 for CR 9243

    QDir target;

    QString normalizedTragerDir = QUrl(targetFolder).path();
    if (!target.exists(normalizedTragerDir))
    {
        if (GetFileInterface() != NULL)
        {
            GetFileInterface()->CreateFolder(normalizedTragerDir);
        }
        //{added by edo.lee 2012.10.29 STATIC TEST 0134186
        else
        {
            // qDebug () << "FileOperationsHelper:: fileInterface is NULL";
            return;
        }
        //}added by edo.lee
    }

    QStringList moveList;
    for (int i = 0; i < fileList.size(); i++)
    {
        // { modified by jaden.lee 2012.08.09 CR12249
        QFileInfo info( QUrl(fileList.at(i)).path() );
        if (info.isFile() &&
                GetFileInterface() != NULL &&
                GetFileInterface()->m_pPlayer != NULL &&
                false == GetFileInterface()->m_pPlayer->IsFileSupported(QUrl(fileList.at(i)).path()) )
        {
            continue;
        }
        // } modified by jaden.lee
        moveList.append(QUrl(fileList.at(i)).path());
    }
    if(moveList.size() == 0)
    {
        // qDebug () << "FileOperationsHelper::empty";
        emit copyError(eMP_CORRUPT_FILE_COPY_ERROR);
        return;
    }

    this->targetFolder = normalizedTragerDir;

    if (moveList.size() > 0)
    {
        // qDebug () << "FileOperationsHelper::move() start move";
        // GetFileInterface()->SetSystemState(SYSTEM_NORMAL_COPY_SPEED); // removed by kihyung 2013.10.08 for ITS 0193747 
        emit progress(0, GetFileInterface()->TotalFileForCopy(), 0, "");
        this->operationType = operationMove;

        connectMoveSignals();
        GetFileInterface()->MoveFileList(moveList, normalizedTragerDir);
    }
}

// modified by ravikanth 21.06.13 for ITS 0174571
int FileOperationsHelper::deleteFiles(const QList<QString> &fileList, int typemode, ETAVolumeType volumeType)
{
    int ret = 0; // 0 = IFILE_FAILED (keep in sync with LGE-Controller enum)
    cancelOperation();
    qDebug () << "FileOperationsHelper::deleteFiles() start delete" << typemode << volumeType;;

    this->completeStatus = eCompleteNormal;

    QStringList deleteList;
    for (int i = 0; i < fileList.size(); i++)
    {
        // { modified by eugene.eseo 2013.06.14
        //deleteList.append(QUrl(fileList.at(i)).path());  
        QString removeFile = QUrl(fileList.at(i)).toString(QUrl::RemoveScheme);
        if(!removeFile.isEmpty())
            deleteList.append(removeFile);
        // } modified by eugene.eseo 2013.06.14
    }

    if (deleteList.size() > 0)
    {
        this->operationType = operationDelete;
        this->m_currentModeType = typemode;
        this->m_currentVolumeType = (int)volumeType;

        QFileInfo info(deleteList.at(0));
        this->targetFolder = info.dir().path();

        if (GetFileInterface() != NULL)
        {
            // moved progress after list set by ravikanth 26-07-13 for ITS 0180417
            connectDeleteSignals();
            ret = GetFileInterface()->DeleteFileList(deleteList, typemode, volumeType);
            this->totalSize = GetFileInterface()->TotalFileForDeletion();
            // qDebug () << "FileOperationsHelper::deleteFiles() total size" << this->totalSize;
            //emit progress(0, this->totalSize, 0, "");
            emit progress(0, 0, 0, ""); // modified by ravikanth 28-08-13 for ITS 0186863
        }
    }
    return ret;
}

void FileOperationsHelper::renameFolder( const QString & sourcePath, const QString & targetPath )
{
    cancelOperation();
    GetFileInterface()->EditedFolder( sourcePath, targetPath );
}

void FileOperationsHelper::cancelOperation(bool isRemoved)
{
    QString msg;
    //this->m_bOSDOnCopyStart = false; //removed by Michael.Kim 2013.11.09 for ISV 93630
    if (GetFileInterface() != NULL)
    {
        switch (this->operationType)
        {
        case operationCopy:
        case operationMove:
            GetFileInterface()->CancelCopyOperation();
            // { added by edo.lee 2012.12.04
            //    msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED");
            //	notifyOSD(msg);
            //	qDebug()<<"STATUS_FILES_COPIED";
            // } added by edo.lee
            
            break;
        case operationDelete:
            GetFileInterface()->CancelDeleteOperation();
            break;

        default:
            // qDebug () << "FileOperationsHelper::cancelOperation(): Unknown operation type";
            break;
        }
    }

    m_isDeviceRemovedOnOperation = isRemoved;

    this->operationType = operationUnknown;
    this->completeStatus = eCompleteCancel;

    setOperationStatus(eOperationNone); // added by Sergey for CR#14494

    this->currentPercent = 0;
    this->currentSize = 0;
    this->totalSize = 0;
}

bool FileOperationsHelper::sendStatusToIBox(uint copyStatus, uint copyProgress)
{
    QDBusReply<bool> reply;

    if (this->mostInterface == NULL)
    {
        return false;
    }

    // qDebug () << "FileOperationsHelper::sendStatusToIBox send CopyStatus status=" <<
    //             copyStatus << "progress" << copyProgress;

    reply = this->mostInterface->call("SetFileCopyStatus", copyStatus,
                                      copyProgress);
    return reply.value();
}

void FileOperationsHelper::onCopyComplete(int index, int skipCount, int copyFilesCount, int modeType, int volumeType, int currentProgressModeType, int currentProgressVolumeType)
{
    qDebug () << "FileOperationsHelper::onCopyComplete index=" << index << "skip count" << skipCount << filesCount; // modified by ravikanth 22-04-13
    qDebug () << "FileOperationsHelper::onCopyComplete m_currentModeType= " << m_currentModeType << " modeType= "<<modeType;
    qDebug () << "FileOperationsHelper::onCopyComplete m_currentVolumeType= " << m_currentVolumeType << " volumeType= "<<volumeType;
    qDebug () << "FileOperationsHelper::onCopyComplete currentProgressModeType= " << currentProgressModeType << " currentProgressVolumeType= "<<currentProgressVolumeType;
    this->m_bOSDOnCopyStart = false;
    if(modeType != currentProgressModeType || volumeType != currentProgressVolumeType)
    {
        qDebug () << "FileOperationsHelper::onCopyComplete - incorrect return from Controller";
        return;
    }
    notifyOSDOnCopyEnd(); // modified by ravikanth 25-07-13 for ITS 0181545

    if((m_currentModeType != modeType) || (m_currentVolumeType != volumeType))
    {
        qDebug () << "FileOperationsHelper::onCopyComplete - incorrect media return";
        return;
    }

    emit copyOpertaionExited(); // modified by ravikanth 19-04-13
    QString msg;
    disconnectCopySignals();
    // { added by edo.lee 2012.12.04
#if STATUSBAR_NEW
    //m_pOSDview->setWindowTitle("@OSD:FR^MEDIA_INFO^WARN^JUKEBOX^JUKEBOX^STATUS_FILES_COPIED");
    qDebug()<<"FileOperationsHelper::STATUS_FILES_COPIED";
#else
    m_pStatusAdapter->setIndicatorState(STATUS_FILES_COPIED, 0);
#endif
    // } added by edo.lee


    sendStatusToIBox(COPY_COMPLETE_STATUS, 100);
    if(index == 0) // when not success
    {
        skipCount = 0;
        copyFilesCount = 0;
    }
    switch (this->completeStatus)
    {
        case eCompleteNormal:
        case eCompleteFileError: // modified by ravikanth 08-08-13 for ITS 0183321
        {
            emit operationCopyComplete(skipCount, copyFilesCount); // modified by ravikanth 22-04-13
            //notifyOSD( "STR_OSD_COPY_COMPLETED" );
            //notifyOSD( "STR_MEDIA_MNG_COPY_COMPLETED" );//modified by edo.lee 2013.05.06
            //[KOR][ITS][177753][minor](aettie.ji)
            if((this->completeStatus != eCompleteFileError) && (index == 1))
            {
                msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_COPY_COMPLETED");
                notifyOSD(msg);
            }

            // {added by Michael.Kim 2014.03.06 for ISV 98041
            if((this->completeStatus == eCompleteFileError && m_isDeviceRemovedOnOperation == false) && ( currentErrorCode == eMP_CORRUPT_FILE_COPY_ERROR && this->currentOperationType == operationCopy))
            {
                if (m_operationMediaType == eOperationMediaNone) {
                    m_timerOSDNotify->start();
                }
            }
            // }added by Michael.Kim 2014.03.06 for ISV 98041

            // }added by Michael.Kim 2014.03.28 for ISV 98041
            if((this->completeStatus == eCompleteNormal && m_isDeviceRemovedOnOperation == false) && (currentErrorCode == eMP_CORRUPT_FILE_COPY_ERROR && this->currentOperationType <= operationCopy))
            {
                if (m_operationMediaType == eOperationMediaNone) {
                    m_timerOSDNotify->start();
                }
            }
            // }added by Michael.Kim 2014.03.28 for ISV 98041

            break;
        }

        case eCompleteCancel:
        {
            //notifyOSD("STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED");//added by edo.lee 2013.05.17
            //[KOR][ITS][177753][minor](aettie.ji)
            msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED");
            if(m_isDeviceRemovedOnOperation == false)
            {
                notifyOSD(msg, (m_operationMediaType == eOperationMediaJukeBox)); // modified for ITS 0212431
            }
            else
            {
                m_isDeviceRemovedOnOperation = false;
            }
            break;
        }

        case eCompleteError:
            break;

        default:
            break;
    }

    this->operationType = operationUnknown;
    this->m_currentModeType = 0; // invalid type
    this->m_currentVolumeType = 0;

    emit fileSystemChanged(); // added by kihyung 2013.2.17
}

void FileOperationsHelper::onDeleteComplete(int index, int modeType, int volumeType)
{
    // qDebug () << "FileOperationsHelper::onDeleteComplete index=" << index;
    qDebug () << "FileOperationsHelper::onDeleteComplete m_currentModeType= " << m_currentModeType << " modeType= "<<modeType;
    qDebug () << "FileOperationsHelper::onCopyComplete m_currentVolumeType= " << m_currentVolumeType << " volumeType= "<<volumeType;
    if((m_currentModeType != modeType) || (m_currentVolumeType != volumeType))
    {
        qDebug () << "FileOperationsHelper::onDeleteComplete - incorrect media return";
        return;
    }

    disconnectDeleteSignals();

    // modified by ravikanth 21.06.13 for ITS 0174571
    if(index != 0) // on success
    {
        emit operationDeleteComplete(this->targetFolder);

        switch( this->completeStatus )
        {
            case eCompleteNormal:
                m_timer->start();
                break;

            case eCompleteCancel:
                break;

            case eCompleteError:
                break;

            default:
                break;
        }
    }
    else
    {
        emit operationDeleteIncomplete();
    }

    this->operationType = operationUnknown;
    this->m_currentModeType = 0; // invalid type
    this->m_currentVolumeType = 0;

    // modified by ravikanth 21.06.13 for ITS 0174571
    if(index != 0) // on success
        emit fileSystemChanged(); // added by kihyung 2013.2.17
}

void FileOperationsHelper::onMoveComplete(int index, int mode)
{
    // qDebug () << "FileOperationsHelper::onMoveComplete index=" << index;
    QString msg;
//{ add by eunhye 2012.09.21 for CR 9243
    emit setMoveStatus(true);
    disconnectMoveSignals();
    switch (this->completeStatus)
    {
       case eCompleteNormal:
          emit operationMoveComplete();
          //notifyOSD( "STR_OSD_MOVING_COMPLITED" );
          //notifyOSD( "STR_MEDIA_MNG_MOVE_COMPLETED" ); //modified by edo.lee 2013.05.06
		  //[KOR][ITS][177753][minor](aettie.ji)
            msg = QCoreApplication::translate("main", "STR_MEDIA_MNG_MOVE_COMPLETED");
            notifyOSD(msg);
          break;
       case eCompleteCancel:
          break;
       case eCompleteError:
          break;
       default:
          break;
    }
//} add by eunhye 2012.09.21
    this->operationType = operationUnknown;
    this->m_currentModeType = 0; // invalid type
    this->m_currentVolumeType = 0;

    emit fileSystemChanged(); // added by kihyung 2013.2.17
}

void FileOperationsHelper::onError(int errorCode, QString fileName)
{
    qDebug () << "FileOperationsHelper::onError errorCode =" << errorCode << "operationType =" << operationType;
    qDebug () << "FileOperationsHelper::onError fileName =" << fileName;

    currentErrorCode = errorCode; // added by Michael.Kim 2014.03.06 for ISV 98041
    this->currentOperationType = this->operationType; // added by Michael.Kim 2014.03.06 for ISV 98041

    if (GetFileInterface() != NULL)
    {
        switch (errorCode)
        {
            case eMP_SAME_FOLDER_EXIST_INCREATE:
                qDebug () << "FileOperationsHelper::onError: error for same folder exist at create";
                break;
            case eMP_SAME_FOLDER_EXIST_INEDIT:
                qDebug () << "FileOperationsHelper::onError: error for same folder exist at edit";
                break;
            case eMP_SAME_FILE_EXIST_INCOPY:
                qDebug () << "FileOperationsHelper::onError: error for same file exist at copying destination";
                break;
            case eMP_LESS_SPACE_ERROR:
                qDebug () << "FileOperationsHelper::onError: less space error exist for copying";
                break;
            case eMP_CORRUPT_FILE_COPY_ERROR:
                qDebug () << "FileOperationsHelper::onError: corrupt file copy error";
                break;
            case eMP_UNSUPPORTED_FORMAT_COPY_ERROR:
                qDebug () << "FileOperationsHelper::onError: unsupported format copy error";
                break;
            case eMP_JUKEBOX_ERROR:
                qDebug () << "FileOperationsHelper::onError: JukeBox  error";
                break;
            default:
                break;
        }

        switch (this->operationType)
        {
            case operationCopy:
            case operationMove:
                handleCopyError(errorCode, fileName);
                break;
            default:
                break;
        }
    }
}

void FileOperationsHelper::onCopyProgress(int percent, qint64 size, int copyCount)
{
    // qDebug () << "FileOperationsHelper::onCopyProgress percent=" << percent << "size=" << size;

    setOperationStatus(eOperationInProgress); // added by Sergey for CR#14494

    sendStatusToIBox(COPYING_STATUS, percent);

    this->totalSize = GetFileInterface()->TotalFileForCopy();
    this->currentSize = size;
    this->currentPercent = percent;

    emit progress(copyCount, GetFileInterface()->TotalFileForCopy(), percent, ""); // modified by ravikanth 18-07-13 for copy index and count display

    if(!this->m_bOSDOnCopyStart)
    {
        notifyOSDOnCopyStart();
    }
}

void FileOperationsHelper::onDeleteProgress(int percent, qint64 size, QString strName)
{
    // qDebug () << "FileOperationsHelper::onDeleteProgress percent=" << percent << "size=" << size;

    setOperationStatus(eOperationInProgress); // added by Sergey for CR#14494

    this->totalSize = GetFileInterface()->TotalFileForDeletion();
    this->currentSize = size;
    this->currentPercent = percent;

    emit progress(size, GetFileInterface()->TotalFileForDeletion(), percent, strName);
}

void FileOperationsHelper::onMoveProgress(int percent, qint64 size)
{
    // qDebug () << "FileOperationsHelper::onMoveProgress percent=" << percent << "size=" << size;

    setOperationStatus(eOperationInProgress); // added by Sergey for CR#14494

    this->totalSize = GetFileInterface()->TotalFileForMove();
    this->currentSize = size;
    this->currentPercent = percent;

    emit progress(size, GetFileInterface()->TotalFileForMove(), percent, "");
}

void FileOperationsHelper::continueOperation()
{
    // qDebug () << "FileOperationsHelper::continueOperation()";

    if (GetFileInterface() != NULL)
    {
        switch (this->operationType)
        {
        case operationCopy:
        case operationMove:
            GetFileInterface()->ContinueCopying();
            break;
        default:
            // qDebug () << "FileOperationsHelper::continueOperation(): Unknown operation type";
            break;
        }
    }
}

void FileOperationsHelper::notifyOSD( const QString& messageId, bool allScreens )
{
    qDebug () << "FileOperationsHelper::notifyOSD status" << messageId<<m_nDisplay;
	//{modified by edo.lee 2012.10.29 STATIC TEST 0134185
    /*
    OSD_MEDIA_INFO osdInfo;
    memset(&osdInfo, 0, sizeof(OSD_MEDIA_INFO));

    osdInfo.eModeType = SB_OSD_Enum_Def::OSD_MEDIA_MODE_JUKEBOX;
    osdInfo.eBarType = SB_OSD_Enum_Def::OSD_INFO_BAR_TYPE;
    osdInfo.stInfoIcon.eType = SB_OSD_Enum_Def::OSD_INFO_SLOT_TYPE_NONE;
    if ( m_pTranslator )
    {
       m_pTranslator->translate("main",messageId.toAscii() ).toWCharArray(osdInfo.stText1.sText);
    }
    else
    {
       messageId.toWCharArray(osdInfo.stText1.sText);
    }

    qDebug() << "FileOperationsHelper::notifyOSD m_nDisplay = " << m_nDisplay;
    m_pStatusAdapter->SetOSDMediaInfo( osdInfo, m_nDisplay );
    */

// { modified by edo.lee 2012.12.04 for new statusbar    
	#if STATUSBAR_NEW
	QString s_disp;
   
   switch(m_nDisplay)
   {
   	case DISPLAY_FRONT:
		s_disp = allScreens ? "FR" : "F"; // modified for ITS 0212431
		break;
	case DISPLAY_REAR:
		s_disp = allScreens ? "FR" : "R"; // modified for ITS 0212431
		break;
	case DISPLAY_FRONT_REAR:	
        case DISPLAY_AUTO:
		s_disp = "FR";
		break;
	default:
        s_disp = "F";		
   }
	QString info = QString("@OSD:"+s_disp+"^MEDIA_INFO^WARN^JUKEBOX^");
	info += "JUKEBOX^";
     // info +=  m_pTranslator->translate("main",messageId.toAscii() );
	info += messageId;
     
	 qDebug() << "FileOperationsHelper::notifyOSD " << info;	
	//modified by edo.lee 2013.05.06
	//if(m_pOSDview)
	//    m_pOSDview->setWindowTitle(info);
	OSDWriter::SendOSD(info);
	//modified by edo.lee 2013.05.06
	#else	
// } modified by edo.lee 
    OSD_MEDIA_INFO *osdInfo = new OSD_MEDIA_INFO ;
    memset(osdInfo, 0, sizeof(OSD_MEDIA_INFO));

    osdInfo->eModeType = SB_OSD_Enum_Def::OSD_MEDIA_MODE_JUKEBOX;
    osdInfo->eBarType = SB_OSD_Enum_Def::OSD_INFO_BAR_TYPE;
    osdInfo->stInfoIcon.eType = SB_OSD_Enum_Def::OSD_INFO_SLOT_TYPE_NONE;
    if ( m_pTranslator )
    {
       m_pTranslator->translate("main",messageId.toAscii() ).toWCharArray(osdInfo->stText1.sText);
    }
    else
    {
       messageId.toWCharArray(osdInfo->stText1.sText);
    }

    // qDebug () << "FileOperationsHelper::notifyOSD m_nDisplay = " << m_nDisplay;
    m_pStatusAdapter->SetOSDMediaInfo( *osdInfo, m_nDisplay );
    delete osdInfo;
	//}modified by edo.lee
	#endif // added by edo.lee 2012.12.04 for new statusbar    
 
}

void FileOperationsHelper::handleCopyError(int errorCode, QString fileName)
{
    setOperationStatus(eOperationNone); // added by Sergey for CR#14494

    switch (errorCode)
    {
        case eMP_SAME_FILE_EXIST_INCOPY:
            emit copyRequest(fileName, GetFileInterface()->TotalFileForCopy()); // modified by ravikanth 07-07-13 for ITS 0178184
            break;
        case eMP_LESS_SPACE_ERROR:
            this->completeStatus = eCompleteError;
            // { modified by wspark 2012.12.06 for showing remaining bytes.
            emit capacityError(this->filesCount,
                               GetFileInterface()->GetTotalCopySize()); // modified by ravikanth 27-08-13 for ITS 186712
            //emit capacityError(this->filesCount,
            //                   GetFileInterface()->GetTotalDiscSpace(JUKEBOX_VOLUME)
            //                   - GetFileInterface()->GetTotalUsedDiscSpace(JUKEBOX_VOLUME));
            // } modified by wspark
            break;
        // { added by sungkim for CR 9616
        case eMP_CORRUPT_FILE_COPY_ERROR:
        case eMP_UNSUPPORTED_FORMAT_COPY_ERROR:
		// modified by ravikanth 20-08-13 for ITS 0183321, 0185271
            if(this->completeStatus != eCompleteError)
                this->completeStatus = eCompleteFileError;
        // } added by sungkim for CR 9616
		// modified by ravikanth 21.06.13 for ITS 0174790
        break;
        case eMP_USER_CANCEL_COPY_ERROR:
        {
		// user cancelled all copy requests by avoding copy for each item
         this->completeStatus = eCompleteCancel;
         emit copyError(errorCode);
         break;
        }
        default:
            emit copyError(errorCode);
    }
}

void FileOperationsHelper::connectCopySignals()
{
   this->connect(GetFileInterface(), SIGNAL(OnTotalFileCopied(int,int,int,int,int,int,int)), this, SLOT(onCopyComplete(int,int,int,int,int,int,int))); // modified by ravikanth 22-04-13
   this->connect(GetFileInterface(), SIGNAL(OnPercentageAndSizeCopied(int,qint64,int)),
                               this, SLOT(onCopyProgress(int,qint64,int))); // modified by ravikanth 18-07-13 for copy index and count display

    /* CR#10023
     * Need to make reconnect for this signal cause it is not received
     * in case of copying from USB to Jukebox due to unknown reason.
     * Seems it is MediaController issue, but root cause was not found */
    disconnect(GetFileInterface(), SIGNAL(OnError(int, QString)), this, SLOT(onError(int, QString)));
    connect(GetFileInterface(), SIGNAL(OnError(int, QString)), this, SLOT(onError(int, QString)));
}

void FileOperationsHelper::connectMoveSignals()
{
    connect(GetFileInterface(), SIGNAL(OnTotalFileMoved(int,int)), this, SLOT(onMoveComplete(int,int)));
    connect(GetFileInterface(), SIGNAL(OnPercentageAndSizeMoved(int,qint64)),
                          this, SLOT(onMoveProgress(int,qint64)));
    connect(GetFileInterface(), SIGNAL(OnFileMoved(QString)), SIGNAL(fileMoved(QString)));

    disconnect(GetFileInterface(), SIGNAL(OnError(int, QString)), this, SLOT(onError(int, QString)));
    connect(GetFileInterface(), SIGNAL(OnError(int, QString)), this, SLOT(onError(int, QString)));
}

void FileOperationsHelper::connectDeleteSignals()
{
    connect(GetFileInterface(), SIGNAL(OnTotalFileDeleted(int,int,int)), this, SLOT(onDeleteComplete(int,int,int)));
    connect(GetFileInterface(), SIGNAL(OnPercentageAndSizeAndNameDeleted(int, qint64, QString)),
                          this, SLOT(onDeleteProgress(int, qint64, QString)));
}

void FileOperationsHelper::disconnectCopySignals()
{
   this->disconnect(GetFileInterface(), SIGNAL(OnTotalFileCopied(int,int,int,int,int,int,int)), this, SLOT(onCopyComplete(int,int,int,int,int,int,int))); // modified by ravikanth 22-04-13
   this->disconnect(GetFileInterface(), SIGNAL(OnPercentageAndSizeCopied(int,qint64,int)),
                                  this, SLOT(onCopyProgress(int,qint64,int))); // modified by ravikanth 18-07-13 for copy index and count display
}

void FileOperationsHelper::disconnectMoveSignals()
{
   disconnect(GetFileInterface(), SIGNAL(OnTotalFileMoved(int,int)), this, SLOT(onMoveComplete(int,int)));
   disconnect(GetFileInterface(), SIGNAL(OnPercentageAndSizeMoved(int,qint64)),
                            this, SLOT(onMoveProgress(int,qint64)));
   disconnect(GetFileInterface(), SIGNAL(OnFileMoved(QString)), this, SIGNAL(fileMoved(QString)));
}

void FileOperationsHelper::disconnectDeleteSignals()
{
   disconnect(GetFileInterface(), SIGNAL(OnTotalFileDeleted(int,int,int)), this, SLOT(onDeleteComplete(int,int,int)));
   disconnect(GetFileInterface(), SIGNAL(OnPercentageAndSizeAndNameDeleted(int, qint64, QString)),
                            this, SLOT(onDeleteProgress(int, qint64, QString)));
}

void FileOperationsHelper::setOverwriteMode(int mode, bool continueCurrentOperation) // modified by ravikanth 18.06.13 for ITS 0174610
{
    if (GetFileInterface())
    {
        if (mode == CANCEL_COPY_OPERATION_MODE)
        {
            this->completeStatus = eCompleteCancel;
        }

        GetFileInterface()->SetOverWriteCurrentExistFileMode(mode);
        if(continueCurrentOperation) // modified by ravikanth 18.06.13 for ITS 0174610
            continueOperation();
    }
}

void FileOperationsHelper::pauseOperation()
{
    // qDebug () << "FileOperationsHelper::pauseOperation()";

    if (GetFileInterface() != NULL)
    {
        switch (this->operationType)
        {
        case operationCopy:
        case operationMove:
            GetFileInterface()->PauseCopying();
            setOperationStatus(eOperationPaused); // added by Sergey for CR#14494
            break;
        default:
            // qDebug () << "FileOperationsHelper::pauseOperation(): Unknown operation type";
            break;
        }
    }
}

void FileOperationsHelper::renameFile( const QString & sourcePath, const QString & targetPath )
{
    // qDebug () << "FileOperationsHelper::renameFile() rename file" << sourcePath << "to" << targetPath;
    //{modified by edo.lee STATIC TEST 0136386
    /*
    if( fileInterface !=  NULL &&
        fileInterface->m_pPlayer != NULL &&
        false == fileInterface->m_pPlayer->IsFileSupported( sourcePath ) )
    {
       emit copyError(eMP_CORRUPT_FILE_COPY_ERROR);
    }
    else
    {
       cancelOperation();
       fileInterface->EditedFile(sourcePath, targetPath);
    }
    */
    if (GetFileInterface() != NULL)
    {
        if (GetFileInterface()->m_pPlayer != NULL &&
           (false == GetFileInterface()->m_pPlayer->IsFileSupported( sourcePath )))
        {
            emit copyError(eMP_CORRUPT_FILE_COPY_ERROR);
        }
        else
        {
            cancelOperation();
            GetFileInterface()->EditedFile(sourcePath, targetPath);
        }
    }
    //}modified by edo.lee
}

void FileOperationsHelper::createFolder( const QString & path)
{
    cancelOperation();

    if (GetFileInterface() != NULL)
    {
        GetFileInterface()->CreateFolder(path);
    }
}
// } modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot

void FileOperationsHelper::setTranslator( QTranslator * translator )
{
   m_pTranslator = translator;
}

void FileOperationsHelper::notifyDeleteComplete()
{
    //notifyOSD("STR_MEDIA_MNG_DELETE_COMPLETED");
	//[KOR][ITS][177753][minor](aettie.ji)
    QString msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_DELETE_COMPLETED");
    notifyOSD(msg);

    emit signalDeleteCompleted();
}

// {added by Michael.Kim 2014.03.06 for ISV 98041
void FileOperationsHelper::notifyCopyCanceled()
{
     qDebug () << "FileOperationsHelper::notifyCopyCanceled: Called";

     QString  msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED");
     notifyOSD(msg);
}
// }added by Michael.Kim 2014.03.06 for ISV 98041

// { added by Sergey for CR#14494
void FileOperationsHelper::setOperationStatus(EOperationStatus value)
{
    // qDebug () << "FileOperationsHelper::setOperationStatus value =" << value;

    m_eOperationStatus = value;
    emit operationStatusChanged(value);
}
// } added by Sergey for CR#14494.

// modified by ravikanth 10-07-13 for ITS 179182
// This is a synchronous copy, use this to copy only one file of small size to avoid blocking thread
// Use this function for copying one file and not multiple files and when there is any chance of lge controller busy
EOperationReturnValue FileOperationsHelper::saveFileSynchronously(const QString& file,const QString& targetPath, bool replaceFile)
{
    // { modified by kihyung 2013.08.14 for ITS 0184288
    QDir saveFolder(targetPath);
    if(!saveFolder.exists())
    {
        if(!saveFolder.mkdir(targetPath))
        {
            return eOperationReturnFileGeneralError;
        }
    }
    
    QString tempFile(file);
    if(tempFile.startsWith("file://"))
    {
        tempFile = tempFile.mid(7);
    }
    QFile srcfile(tempFile);

    int     nSize       = tempFile.size();
    int     nIndex      = tempFile.lastIndexOf(".");
    QString strFormat   = tempFile.mid(nIndex, nSize - nIndex).toLower();


    QString dstFileName1 = targetPath + QString("/") + QString(DEFAULT_SAVEFRAME_FILE1) + strFormat;
    QString dstFileName2 = targetPath + QString("/") + QString(DEFAULT_SAVEFRAME_FILE2) + strFormat;
    QString dstFileName;

    if(QFile::exists(dstFileName1) == true) {
        QFile::remove(dstFileName1);
        dstFileName = dstFileName2;
    }
    else {
        QFile::remove(dstFileName2);
        dstFileName = dstFileName1;
    }

    if(QFile::exists(dstFileName))
    {
        if(replaceFile)
        {
            if(!QFile::remove(dstFileName))
            {
                return eOperationReturnFileRemoveError;
            }
        }
        else
        {
            return eOperationReturnFileAlreadyExists;
        }
    }

    if(!srcfile.copy(dstFileName))
    {
        return eOperationReturnFileCopyError;
    }
	
    QString msg =  QCoreApplication::translate("main", "STR_MEDIA_MNG_COPY_COMPLETED");
    notifyOSD( msg );
    
    return eOperationReturnFileNone;
    // } modified by kihyung 2013.08.14 for ITS 0184288    
}

// added by kihyung 2013.10.08 for ITS 0193747 
void FileOperationsHelper::setCopySpeed(int nSpeed)
{
    m_pFileInterface->SetSystemState(nSpeed);
}

// modified by ravikanth 25-07-13 for ITS 0181545
void FileOperationsHelper::notifyOSDOnCopyStart()
{
    this->m_bOSDOnCopyStart = true;
    OSDWriter::SendOSD("@STA^COPY^1");
}

void FileOperationsHelper::notifyOSDOnCopyEnd()
{
    OSDWriter::SendOSD("@STA^COPY^0");
}

int FileOperationsHelper::currentOperation()
{
    return this->operationType;
}

// modified for ITS 0212431
void FileOperationsHelper::setDisplay( int disp )
{
    m_nDisplay = ( DISPLAY_T )disp;
    if( m_nDisplay == DISPLAY_FRONT_REAR )
    {
        setMediaType(eOperationMediaNone);
    }
}
