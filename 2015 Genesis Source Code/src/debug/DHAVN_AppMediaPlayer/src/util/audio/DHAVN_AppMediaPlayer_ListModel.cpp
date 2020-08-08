#include "util/audio/DHAVN_AppMediaPlayer_ListModel.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h"
#include "engine/DHAVN_AppMediaPlayer_EngineBase.h"
#include <QDeclarativeView> // added by edo.lee 2012.12.08 for statusbar
#include <DHAVN_AppFramework_AppEngineQML_Def.h> // added by edo.lee 2012.12.04 for statusbar

//const int ALBUM_ICON_SIZE = 78; //removed by junam 2012.10.12 for CR14040
#define EXTM3U        "#EXTM3U"
#define EXTINF        "#EXTINF"
#define GB_SIZE       (1024L * 1024L * 1024L) // added by ruindmby 2012.08.22 for CR 12454
#define MB_SIZE       (1024L * 1024L)         // added by ruindmby 2012.09.26 for CR#11543

#define USB_ODCA_COUNT 2// added by junam 2012.10.12 for CR14040
#define PINYIN_TABLE_SIZE 20992 // added by jaehwan 2014.01.03 for pinyin sorting.
// {removed by junam 2013.11.18 for deprecated code
//const wchar_t m_categoryKoreaTable[19] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141,
//                                           0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149,
//                                           0x314A, 0x314B, 0x314C, 0x314D, 0x314E };
// } removed by junam

enum
{
    REQUID_LIST_DEVICE_ID           = 3000,
    REQUID_LIST_ALL_FILES           = REQUID_LIST_DEVICE_ID + 1,
    //{added by junam 2014.03.09 for ISV74498
    REQUID_LIST_DYNAIC_FETCH_ENTER   = REQUID_LIST_ALL_FILES + MP::ALL_DEVICES + 1, //enterning dynamic fetch list
    REQUID_LIST_TRACK_CHANGE, //On End of Media or Next track button.
    //}added by junam
    REQUID_LIST_DATABASE_UPDATE,  //added by junam 2013.05.09 for database update
    REQUID_LIST_QUICK_SCROLL, //added by junam 2013.07.25 for ITS_EU_180117
    REQUID_LIST_JUKEBOX_CATEGORY_AVAILABLE,   // added by sungha.choi 2013.08.12 for ITS 182674
    REQUID_LIST_USB1_CATEGORY_AVAILABLE,   // added by sungha.choi 2013.08.12 for ITS 182674_3rd
    REQUID_LIST_USB2_CATEGORY_AVAILABLE,   // added by sungha.choi 2013.08.12 for ITS 182674_3rd
    REQUID_LIST_COPY_OPERATION,    // added by sungha.choi 2013.09.22 for ITS 0189462
    REQUID_LIST_MAXIMUM
};

#define GET_TA_REQUID(X,Y) (X + Y)

// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
CListModel::CListModel(AppMediaPlayer_Controller_Audio &aController, QMap<QString, QString> & imageMap, QObject *parent)  //changed by junam 2013.10.28 for 2nd depth coverart
    : m_AudioController(aController)
    ,m_imageMap(imageMap) //added by junam 2013.10.28 for 2nd depth coverart
    ,m_filesCount(-1) ,m_countInfo(0), m_nTotalItemCount(0)     // added by junam 2012.07.15 for CR 10719
    //,m_trimDownMetaInfoTimer(this) //added by junam 2012.09.21 for CR13485
    ,m_requestCoverTimer(this) //added by junam 2012.10.12 for CR14040
    ,m_OperationCompleteforTA(false) // added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
    ,m_isCopyAll(false) // added by wspark 2012.11.27 for ISV64438
    ,m_currentIndex(0) // added by eugene.seo 2013.01.07 for scroll issue
    ,m_popupType(-1) //added by yongkyun.lee 2012.12.07 for file info Popup update
    ,m_folderHistoryStack(-1) //added by aettie 2013.04.10 for QA 41
    ,m_currentPlayingPath("") //added by aettie 2013.04.10 for QA 41
    ,m_skipFileCount(0) // modified by ravikanth 22-04-13
    ,m_copyFileCount(0) // modified by ravikanth 22-04-13
    ,m_currentReplaceFileName("") // modified by ravikanth 07-07-13 for ITS 0178185
    ,m_currentReplaceCopyCount(0) // modified by ravikanth 07-07-13 for ITS 0178184
    ,copyFromMainPlayer(false) //[KOR][ITS][179014][comment](aettie.ji)
    ,m_bCategoryTabAvailableJukeBox(false)   // { added by sungha.choi 2013.08.05 for ITS 182674
    ,m_bCategoryTabAvailableUSB1(false)
    ,m_bCategoryTabAvailableUSB2(false)   // } added by sungha.choi 2013.08.05 for ITS 182674
    ,m_categoryTabAvailableTimer(this) //added by junam 2013.11.01 for category disabled state
    ,m_bUpdateListOnEditExit(false)
{
    Q_UNUSED(parent);
    m_pListTrackerAbstractor = new CTrackerAbstractor();
	// { added by jaehwan.lee 2013.10.03
    m_pListTrackerAbstractor->SetLanguage(m_AudioController.GetEngineMain().GetLanguage()); 
	// }

	// { added by edo.lee 2012.12.04 for statusbar
    m_pFileOperationsHelper = new FileOperationsHelper(new QDeclarativeView());
	// } added by edo.lee 

    if (m_pListTrackerAbstractor)
    {
        connect(m_pListTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)),
                this, SLOT(ListRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)));

        connect(m_pListTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)),
                this, SLOT(partialRequestComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int, uint)));

        // { added by kihyung 2013.2.17
        connect(m_pListTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)),
                                    this, SLOT(ListRequestCompleteWithVolumeType(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)));
        // } added by kihyung 2013.2.17

        m_pListTrackerAbstractor->setDeviceId(eJukeBox, NULL, "/usr_data/media");
        m_pListTrackerAbstractor->SetCountryVariant(GetCountryVariant()); // added by Dmitry 28.05.13
    }

   // m_pFileOperationsHelper = new FileOperationsHelper(m_AudioController.GetEngineMain().GetStatusAdapter()); remove by edo.lee 2012.12.11
    if (m_pFileOperationsHelper)
    {
        m_pFileOperationsHelper->setDisplay(DISPLAY_NONE);

        connect(m_pFileOperationsHelper, SIGNAL(progress(int, int, int, QString)),
                                   this, SLOT(onProgress(int, int, int, QString)));

        connect(m_pFileOperationsHelper, SIGNAL(operationDeleteComplete(QString)),
                                   this, SLOT(onOperationComplete()));

        connect(m_pFileOperationsHelper, SIGNAL(operationDeleteIncomplete()),
                                   this, SLOT(onOperationIncomplete())); // modified by ravikanth 21.06.13 for ITS 0174571

        connect(m_pFileOperationsHelper, SIGNAL(operationCopyComplete(int,int)),
                                   this, SLOT(onOperationComplete(int,int))); // modified by ravikanth 22-04-13

        connect(m_pFileOperationsHelper, SIGNAL(operationMoveComplete()),
                                   this, SLOT(onOperationComplete()));

        connect(m_pFileOperationsHelper, SIGNAL(copyRequest(QString,int)),
                                   this, SLOT(replaceFileRequest(QString,int))); // modified by ravikanth 07-07-13 for ITS 0178184

        connect(m_pFileOperationsHelper, SIGNAL(capacityError(int, qint64)),
                                   this, SLOT(capacityError(int, qint64)));

        connect(m_pFileOperationsHelper, SIGNAL(fileMoved(QString)),
                                   this, SLOT(onFileMoved(QString)));

        connect(m_pFileOperationsHelper, SIGNAL(copyError(int)),
                                   this, SLOT(HandleError(int)));

        connect(m_pFileOperationsHelper, SIGNAL(fileSystemChanged()), 
                                   this, SLOT(onFileSystemChanged()));

        connect(m_pFileOperationsHelper, SIGNAL(copyOpertaionExited()),
                                   this, SLOT(onCopyOpertaionExit())); // modified by ravikanth 19-04-13


        QTranslator& translator = m_AudioController.GetEngineMain().GetTranslator();
        m_pFileOperationsHelper->setTranslator(&translator);
    }

    // modified by kihyung 2013.09.13 for ISV 89552
    connect(&m_AudioController, SIGNAL(currPlayingItemPosChanged(QString, QString, QString)), this, SLOT(setCurrPlayingItemPos(QString, QString, QString)));

	
    connect(&m_AudioController, SIGNAL(operationDisplay(DISPLAY_T)),
							  this, SLOT(onOperationDisplay(DISPLAY_T))); //added by edo.lee 2013.03.22

    QHash<int, QByteArray> roles = roleNames();
    roles.insert( imageRole,  QByteArray( "itemViewImage" ) );
    roles.insert( titleRole,  QByteArray( "itemViewTitle" ) );
    roles.insert( urlRole,    QByteArray( "itemURL" ) );
    roles.insert( firstCharRole, QByteArray( "firstChar" ) );
    roles.insert( checkBoxMarked, QByteArray( "isCheckBoxMarked" ) );
    roles.insert( selectableRole, QByteArray( "isSelectable" ) );
    roles.insert( imageVisibleRole, QByteArray( "isImageVisible" ) ); // added by junam 2012.10.12 for CR14040
    //roles.insert( trackNumber, QByteArray( "itemTrackNumber" ) ); // added by aettie.ji 2012.12.21 for new ux
    setRoleNames(roles);

    m_volumeType = eJukeBox;
    m_fileOperationData.operation = LVEnums::OPERATION_NONE;
    m_isDisplayTip = false;
    m_displayMode = MP::UNDEFINED;
    actuallyDeletedFiles.clear();

    resetRequestData();
    resetPartialFetchData();
    resetFileOperationData();
    resetQuickEditData();

    m_isFocusChange = true; //added by dongjin 2012.11.16 for CR14033
    m_bCopyMode = false; // added by junam 2012.9.26 for disable file in copy mode
    m_currentCategoryState = UNDEFINED_STATE;
    m_requestCoverTimer.setSingleShot(true);
    connect(&m_requestCoverTimer, SIGNAL(timeout()), &m_AudioController, SLOT(onRequestCoverArtForListView()));
    isCopyCompletedMode = false; // added by eugene 2012.11.29 for CR 16076
    isCopyingMode = false; // added by eugene 2013.01.10 for ISV 69609
    CheckBoxSelectable_count = 0; //added by yungi 2013.03.06 for New UX FileCount
    m_currentFocuseIndex = -1; //added by junam 2013.06.28
    m_moving = false; //added by junam 2013.07.15 for ISV_NA_87444
    m_currentDeletedFileCount = 0;

    //{added by junam 2013.11.01 for category disabled state
    m_categoryTabAvailableTimer.setInterval(30000);
    connect(&m_categoryTabAvailableTimer, SIGNAL(timeout()), this, SLOT(onCategoryTabAvailableTimeOver()));
    //}added by junam
    //{ added by jaehwan 2014.01.08 for pinyin sorting
    if (AppEngineBase::GetLanguage() == LANGUAGE_ZH_CMN ) // added by jaehwan.lee 2014.2.21 to load pinyin table only for chiness.
    {
        MP_HIGH<<"LOAD PINYIN table - START" << LOG_ENDL;
        CTrackerAbstractor::LoadPinyinTable();
        MP_MEDIUM<<"LOAD PINYIN table - END" << LOG_ENDL;
    }
    // } added by jaehwan
}
// } modified by eugeny.novikov 2012.12.10

CListModel::~CListModel()
{
    if (m_pListTrackerAbstractor)
    {
        disconnect(m_pListTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
        disconnect(m_pListTrackerAbstractor, SIGNAL(RequesteComplete(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int)));
        delete m_pListTrackerAbstractor;
    }

    if (m_pFileOperationsHelper)
    {
        disconnect(m_pFileOperationsHelper, 0, 0, 0);
        delete m_pFileOperationsHelper;
    }
}

// { modified by Michael.Kim 2014.02.04 for ITS 223011
// { added by eugeny.novikov 2012.12.10 for CR 16564 fastboot
USBController*
CListModel::GetUSBController()
{
    USBController *ptr = m_AudioController.GetControllers().GetUSBController();
    return ptr;
}
// } added by eugeny.novikov 2012.12.10
// } modified by Michael.Kim 2014.02.04 for ITS 223011

bool
CListModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
    if (!index.isValid() || index.row() >= rowCount()) // modified by ravikanth 27.06.13 for ITS 0175963
    {
        return false;
    }

    switch (role)
    {
        case checkBoxMarked:
		    // { changed by junam 2012.07.15 for CR 10719
			// modified by ravikanth 27.06.13 for ITS 0175963
            //if ( index.row() < m_iBlockOffset || index.row() >=  m_iBlockOffset +  m_listMetaInfo.isItemCheckBoxMarked.count() )
            //    return false;
            //Q_ASSERT(m_listMetaInfo.isItemCheckBoxMarked.size() > index.row() - m_iBlockOffset);
            // m_listMetaInfo.isItemCheckBoxMarked[index.row() - m_iBlockOffset] = value.toBool();
            if ( index.row() < 0 || index.row() >= m_listMetaInfo.isItemCheckBoxMarked.count() ) // modified by ravikanth 27.06.13
                return false;
            m_listMetaInfo.isItemCheckBoxMarked[index.row()] = value.toBool();
            emit dataChanged(index, index);
            return true;

        case titleRole:
            if ( setMetaInfo(index.row(), m_listMetaInfo.itemTitle, value.toString()))
            {
                emit dataChanged(index, index);
            }
			// } changed by junam
            return false;
        case imageRole:
        {
            //{changed by junam 2013.10.28 for 2nd depth coverart
            //if ( setMetaInfo(index.row(), m_listMetaInfo.imageSource, value.toString()))
            QString itemSource = getMetaInfo( index.row(), m_listMetaInfo.itemSource);
            if(!itemSource.isEmpty())
            {
                m_imageMap.insert(itemSource, value.toString());
                emit dataChanged(index, index);
                return true;
            }
            //}changed by junam
            return false;
        }
        default:
            return false;
    }
    return false;
}

void
CListModel::toggleItemCheckbox(int itemIndex)
{
    if (itemIndex >= 0 && itemIndex < rowCount())
    {
		// modified by ravikanth 27.06.13 for ITS 0175963
        bool isChecked =  ! m_listMetaInfo.isItemCheckBoxMarked.at(itemIndex); //! getMetaInfo( itemIndex, m_listMetaInfo.isItemCheckBoxMarked); // chage to use getInfo by junam 2012.07.15 for CR 10719
     // { modified by yungi 2013.03.06 for New UX FileCount
        if (isChecked)
        {
            //m_fileOperationData.checkedIdxList.append(itemIndex + m_iBlockOffset);
            m_fileOperationData.checkedIdxList.append(itemIndex);
            CheckBoxSelectable_count++;
        }
        else
        {
            //m_fileOperationData.checkedIdxList.removeOne(itemIndex + m_iBlockOffset);
            m_fileOperationData.checkedIdxList.removeOne(itemIndex);
            CheckBoxSelectable_count--;
        }
     // } modified by yungi 2013.03.06 for New UX FileCount
        setData(this->index(itemIndex), QVariant::fromValue(isChecked), checkBoxMarked);

        switch (m_fileOperationData.category)
        {						
            case SONG_DEPTH_0:
            case ALBUM_DEPTH_1:
            //case ARTIST_DEPTH_1: //remove by jaehwan 2013.11.04
            case ARTIST_DEPTH_2: // added by jaehwan 2013.11.04
			case GENRE_DEPTH_1:	// added by eugene.seo 2013.05.20 list query change
			//case ARTIST_DEPTH_2: // removed by eugene.seo 2013.05.20 we don't use any more
            //case GENRE_DEPTH_3: // removed by eugene.seo 2013.05.20 we don't use any more
                if (isChecked)
                {
                    // { modified by wspark 2013.04.12
                    //m_fileOperationData.fileURLs.append(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));// chage to use getInfo by junam 2012.07.15 for CR 10719

                    if(m_fileOperationData.category == SONG_DEPTH_0)
                    {
                        m_fileOperationData.fileURLs.append(getMetaInfo( itemIndex, m_listMetaInfo.itemSource));
                    }
                    else
                    {
                        m_fileOperationData.fileURLs.append(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));
                    }
                    // } modified by wspark

                    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        m_fileOperationData.trackTitles.append(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                }
                else
                {
                    // { modified by wspark 2013.04.12
                    //m_fileOperationData.fileURLs.removeOne(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));// chage to use getInfo by junam 2012.07.15 for CR 10719
                    if(m_fileOperationData.category == SONG_DEPTH_0)
                    {
                        m_fileOperationData.fileURLs.removeOne(getMetaInfo( itemIndex, m_listMetaInfo.itemSource));
                    }
                    else
                    {
                        m_fileOperationData.fileURLs.removeOne(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));
                    }
                    // } modified by wspark

                    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        m_fileOperationData.trackTitles.removeOne(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                }
                break;

            case FOLDER_DEPTH_0:
            case FOLDER_DEPTH_1:
                if (m_listMetaInfo.isFolder.at(itemIndex))
                {
                    if (isChecked)
                        m_fileOperationData.itemNames.append(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                    else
                        m_fileOperationData.itemNames.removeOne(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                }
                else
                {
                    if (isChecked)
                    {
                        m_fileOperationData.fileURLs.append(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));// chage to use getInfo by junam 2012.07.15 for CR 10719

                        if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        {
                            // { modified by changjin 2012.09.14 for 13127
                            //fileOperationData.trackTitles.append(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                            if(getMetaInfo( itemIndex, m_listMetaInfo.itemName).startsWith(JUKEBOX_ROOT))
                            {
                                m_fileOperationData.trackTitles.append(getExtractSongTitle(getMetaInfo(itemIndex, m_listMetaInfo.itemName)));
                            }
                            else
                            {
                                m_fileOperationData.trackTitles.append(getMetaInfo( itemIndex, m_listMetaInfo.itemName));
                            }
                            // } modified by changjin 2012.09.14
						}
                    }
                    else
                    {
                        m_fileOperationData.fileURLs.removeOne(FilePath_Prefix + getMetaInfo( itemIndex, m_listMetaInfo.itemSource));// chage to use getInfo by junam 2012.07.15 for CR 10719

                        if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                            m_fileOperationData.trackTitles.removeOne(getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                    }
                }
                break;

            /* For the views with Songs content we save file URLs directly */
            default:
                if (isChecked)
                    m_fileOperationData.itemNames.append( getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                else
                    m_fileOperationData.itemNames.removeOne( getMetaInfo( itemIndex, m_listMetaInfo.itemName));// chage to use getInfo by junam 2012.07.15 for CR 10719
                break;
        }
    }
}

// modified by ravikanth 26-06-13 for ITS 0175972
bool CListModel::isCheckBoxMarked(int index)
{
    if (index >= 0 && index < rowCount())
    {
        return m_listMetaInfo.isItemCheckBoxMarked.at(index); // modified by ravikanth 27.06.13 for ITS 0175963
    }
    else
    {
        return false;
    }
}

bool
CListModel::isAnyoneMarked()
{
    return m_listMetaInfo.isItemCheckBoxMarked.contains(true);
}

void
CListModel::enableAllCheckBoxes(bool isEnabled)
{
    MP_LOG << "isEnabled =" << isEnabled << LOG_ENDL;

    for (int idx = 0; idx < rowCount(); idx++)
    {
        setData(this->index(idx), QVariant::fromValue(isEnabled), checkBoxMarked);
    }

    if (isEnabled)
    {
        m_fileOperationData.checkedIdxList.clear();

        int totalCount = (m_fileOperationData.category == SONG_DEPTH_0 ||
                          m_fileOperationData.category == ALBUM_DEPTH_0 ||
                          m_fileOperationData.category == ARTIST_DEPTH_0)  ? m_nTotalItemCount : rowCount();

        for (int i = 0; i < totalCount; i++)
        {
            m_fileOperationData.checkedIdxList.append(i);
        }
        setFileURLCount(totalCount); // added by lssanh 2013.03.23 NoCR copyall cancel
    }
    else
    {
        resetFileOperationData();
    }
}

void CListModel::resetFileOperationData()
{
    MP_LOG << LOG_ENDL;
    m_fileOperationData.bCreatingUrlList = false;
    m_fileOperationData.fileURLs.clear();
    m_fileOperationData.itemNames.clear();
    m_fileOperationData.extraParams.clear();
    m_fileOperationData.trackTitles.clear();
    m_fileOperationData.destPath = "";
    m_fileOperationData.plPath = "";
    m_fileOperationData.checkedIdxList.clear();
    m_fileOperationData.category = calcCategoryState();
}

void CListModel::resetQuickEditData()
{
    MP_LOG << LOG_ENDL;
    m_quickEdit.category = UNDEFINED_STATE;
    m_quickEdit.index = -1;
    m_quickEdit.itemURL = "";
    m_quickEdit.itemTitle = "";
    m_quickEdit.operation = LVEnums::OPERATION_NONE;
}

void CListModel::backupFileOperationData()
{
    m_fileOperationData.category = calcCategoryState();

    switch (m_fileOperationData.category)
    {
        case ARTIST_DEPTH_1:
        case GENRE_DEPTH_1:
        //case GENRE_DEPTH_2: // removed by eugene.seo 2013.05.20 we don't use any more
            if (!m_lastRequestStack.isEmpty())
            {
                m_fileOperationData.extraParams.append(m_lastRequestStack.at(0));

                if (m_fileOperationData.category == GENRE_DEPTH_2 &&
                    m_lastRequestStack.count() > 1)
                {
                    m_fileOperationData.extraParams.append(m_lastRequestStack.at(1));
                }
            }
            break;

        default:
            MP_LOG << "No need to backup data for this category" << LOG_ENDL;
            break;
    }
}

void CListModel::startFileOperation(QString destPath, QString plPath)
{
    MP_LOG << "destPath =" << destPath << LOG_ENDL;
    MP_LOG << "plPath =" << plPath << LOG_ENDL;
    MP_LOG << "operation =" << m_fileOperationData.operation << LOG_ENDL;

    m_fileOperationData.destPath = destPath;
    m_fileOperationData.plPath = plPath;

    if (!isFileOperationValid())
    {
        MP_LOG << "Warning! Incorrect data for file operation!" << LOG_ENDL;
        resetFileOperationData();
        return;
    }

    createUrlListFromMarkedItems();

    if (!m_fileOperationData.bCreatingUrlList)
    {
        invokePendingFileOperation();
    }
    //added by yongkyun.lee 2012.11.07 for Miner controll
    MinerControler(true);
    //added by yongkyun 
}

bool CListModel::isFileOperationValid()
{
    // { modified by eugene 2012.11.29 for CR 16076
    // { modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
	int device = m_AudioController.playerMode();
	/*
	int device;
	
	if(isCopyCompletedMode == true) 
		device = MP::JUKEBOX;  	
	else
	    device = m_AudioController.playerMode();
         */
        // } modified by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
	// } modified by eugene 2012.11.29 for CR 16076

    switch (m_fileOperationData.operation)
    {
        case LVEnums::OPERATION_COPY:
            return ((device == MP::USB1 || device == MP::USB2) &&
                     !m_fileOperationData.destPath.isEmpty());

        case LVEnums::OPERATION_MOVE:
            return (device == MP::JUKEBOX && !m_fileOperationData.destPath.isEmpty());

        case LVEnums::OPERATION_DELETE:
            return (device == MP::JUKEBOX || m_displayMode == MP::JUKEBOX); // added by ruindmby 2012.09.26 for CR#11543

        case LVEnums::OPERATION_ADD_TO_PLAYLIST:
            return (device == MP::JUKEBOX || device == MP::USB1 || device == MP::USB2);

        default:
            return false;
    }
}

void CListModel::handleSelectAllItems()
{
    MP_LOG << LOG_ENDL;

    m_fileOperationData.itemNames.clear();
    m_fileOperationData.fileURLs.clear();
    m_fileOperationData.trackTitles.clear();
    enableAllCheckBoxes(true);

    m_fileOperationData.bCreatingUrlList = true;

    switch (m_fileOperationData.category)
    {
        case SONG_DEPTH_0:
            MP_LOG << "Sending eGetAllSongsFromVolumeQuery_Id" << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsFromVolumeQuery_Id, REQUID_LIST_COPY_OPERATION);    // modified by sungha.choi 2013.09.22 for ITS 0189462
            break;

        case ARTIST_DEPTH_0:
            MP_LOG << "Sending eGetAllMusicArtistsFromVolumeQuery_Id" << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsFromVolumeQuery_Id, REQUID_LIST_COPY_OPERATION);    // modified by sungha.choi 2013.09.22 for ITS 0189462
            break;

        case ALBUM_DEPTH_0:
            MP_LOG << "Sending eGetAllAlbumsFromVolumeQuery_Id" << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsFromVolumeQuery_Id, REQUID_LIST_COPY_OPERATION);    // modified by sungha.choi 2013.09.22 for ITS 0189462
            break;

        case ARTIST_DEPTH_1:
        case GENRE_DEPTH_0:
        case GENRE_DEPTH_1:
        //case GENRE_DEPTH_2: // removed by eugene.seo 2013.05.20 we don't use any more
            m_fileOperationData.itemNames.append(m_listMetaInfo.itemName);
            m_fileOperationData.fileURLs.append(m_listMetaInfo.itemSource); // added by eugene.seo 2013.05.20 list query change
            m_fileOperationData.bCreatingUrlList = false;
            break;
        
        case ALBUM_DEPTH_1:
        case ARTIST_DEPTH_2: // removed by eugene.seo 2013.05.20 we don't use any more //added by jaehwan 2013.11.04
        //case GENRE_DEPTH_3: // removed by eugene.seo 2013.05.20 we don't use any more
        //case PLAYLIST_DEPTH_0: // removed by eugene.seo 2013.05.20 we don't use any more
        //case PLAYLIST_DEPTH_1: // removed by eugene.seo 2013.05.20 we don't use any more
            m_fileOperationData.fileURLs.append(m_listMetaInfo.itemSource);
            m_fileOperationData.trackTitles.append(m_listMetaInfo.itemTitle);
            m_fileOperationData.bCreatingUrlList = false;
            break;

        case FOLDER_DEPTH_0:
        case FOLDER_DEPTH_1:
            for (int i = 0; i < m_listMetaInfo.isFolder.count(); i++)
            {
                if (m_listMetaInfo.isFolder[i])
                {
                    m_fileOperationData.itemNames.append(m_listMetaInfo.itemName.at(i));
                }
                else
                {
                    m_fileOperationData.fileURLs.append(FilePath_Prefix + m_listMetaInfo.itemSource.at(i)); // modified by Dmitry 28.05.13

                    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        m_fileOperationData.trackTitles.append(m_listMetaInfo.itemTitle.at(i));
                }
            }
            break;

        case UNDEFINED_STATE:
        default:
            resetFileOperationData();
            break;
    }
	// { modified by ravikanth 28-04-13 to remove copy cancel confirm popup
    if( ( m_fileOperationData.category != SONG_DEPTH_0 ) && ( m_fileOperationData.category != ARTIST_DEPTH_0 )
            && ( m_fileOperationData.category != ALBUM_DEPTH_0 ) )
    {
        emit contentRequestAllComplete();
    }
	// } modified by ravikanth 28-04-13
}

/* In this function we assume that all data is ready to continue file operation */
void CListModel::createUrlListFromMarkedItems()
{
    MP_LOG << "itemNames.count() =" << m_fileOperationData.itemNames.count() << LOG_ENDL;
    MP_LOG << "fileURLs.count() =" << m_fileOperationData.fileURLs.count() << LOG_ENDL;
    MP_LOG << "trackTitles.count() =" << m_fileOperationData.trackTitles.count() << LOG_ENDL;

    // foreach (QString str, m_fileOperationData.itemNames)
    //     MP_LOG << "itemNames =" << str << LOG_ENDL;

    // foreach (QString str, m_fileOperationData.fileURLs)
    //     MP_LOG << "fileURLs =" << str << LOG_ENDL;

    m_fileOperationData.bCreatingUrlList = true;
    m_volumeType = AudioUtils::VolumeType((MP::DEVICE_T)m_displayMode);

    if (m_volumeType == eDefaultVolume)
    {
        MP_LOG << "Error: Unsupported VolumeType!" << LOG_ENDL;
        return;
    }

    CATEGORY_STATE_T category = m_fileOperationData.category;
    switch (category)
    {
        case ARTIST_DEPTH_0:
        case ALBUM_DEPTH_0:
        {
            if (!m_fileOperationData.itemNames.isEmpty())
            {
                // { Modified by sangmin.seol ITS_0189462 Change query for copy all in artist, genre, album category depth 0
                if( CheckBoxSelectable_count == m_nTotalItemCount )// { Modified by sangmin.seol ITS_0209610 Consider delete-all & copy-all case in ARTIST_DEPTH_0, ALBUM_DEPTH_0
                {
                    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsFromVolumeQuery_Id, REQUID_LIST_COPY_OPERATION);
                }
                else
                {
                    ETrackerAbstarctor_Query_Ids queryId =
                            (category == ARTIST_DEPTH_0) ? eGetMultipleArtistSongsFromVolumeQuery_Id
                                                         : eGetMultipleAlbumSongsFromVolumeQuery_Id;

                    m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames,
                                                          m_volumeType, queryId);
                }
                // } Modified by sangmin.seol ITS_0189462 Change query for copy all in artist, genre, album category depth 0
            }
            break;
        }

        case GENRE_DEPTH_0:
        {
            if (!m_fileOperationData.itemNames.isEmpty())
            {
                // { Modified by sangmin.seol ITS_0189462 Change query for copy all in artist, genre, album category depth 0
                if( CheckBoxSelectable_count == m_nTotalItemCount ) // { Modified by sangmin.seol ITS_0209610 Consider delete-all & copy-all case in GENRE_DEPTH_0
                {
                    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsFromVolumeQuery_Id, REQUID_LIST_COPY_OPERATION);
                }
                else
                {
                    MP_LOG << "Sending eGetMultipleGenreSongsFromVolumeQuery_Id" << LOG_ENDL;
                    m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames, m_volumeType,
                                                         eGetMultipleGenreSongsFromVolumeQuery_Id);
                }
                // { Modified by sangmin.seol ITS_0189462 Change query for copy all in artist, genre, album category depth 0
            }
            break;
        }

        //{ added by jaehwan 2013.11.04 for copy issue in artist depth 1
        case ARTIST_DEPTH_1:
        {
            if (!m_fileOperationData.itemNames.isEmpty())
            {
                ETrackerAbstarctor_Query_Ids queryId = eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id;

                m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames,
                                                      m_fileOperationData.extraParams,
                                                      m_volumeType, queryId);
            }
            break;
        } //} adde by jeahwan

		/* // { removed by eugene.seo 2013.05.20 we don't use any more
        case ARTIST_DEPTH_1:
        case GENRE_DEPTH_1:
        case GENRE_DEPTH_2:
        {
            if (!m_fileOperationData.itemNames.isEmpty())
            {
                ETrackerAbstarctor_Query_Ids queryId =
                        (category == ARTIST_DEPTH_1) ? eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id :
                        (category == GENRE_DEPTH_1)  ? eGetMultipleArtistSongsByGenreFromVolumeQuery_Id
                                                     : eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id;

                // foreach (QString str, m_fileOperationData.extraParams)
                //     MP_LOG << "extraParams =" << str << LOG_ENDL;

                m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames,
                                                      m_fileOperationData.extraParams,
                                                      m_volumeType, queryId);
            }
            break;
        } // } removed by eugene.seo 2013.05.20 we don't use any more
		*/
			
        case FOLDER_DEPTH_0:
        case FOLDER_DEPTH_1:
            if (!m_fileOperationData.itemNames.isEmpty() || !m_fileOperationData.fileURLs.isEmpty()) //Changed by Alexey Edelev 2012.09.11.CR 13020
            {
                //{Changed by Alexey Edelev 2012.09.11.CR 13020
                QStringList copyMoveList = m_fileOperationData.itemNames + m_fileOperationData.fileURLs;
                copyMoveList.removeDuplicates();
                //}Changed by Alexey Edelev 2012.09.11.CR 13020
                // { modified by ruinivbe 2012.08.31 CR 12885
                if (m_fileOperationData.operation == LVEnums::OPERATION_COPY)
                {
                    //emit signalShowPopup(LVEnums::POPUP_TYPE_COPYING, false, false); // deleted by wspark 2012.12.18 for ISV65071
                    MP_LOG << "FileOperationHelper copy" << LOG_ENDL;
                    // { modified by wspark 2012.12.14 for copying according to the media type.
                    //m_pFileOperationsHelper->copy(copyMoveList, //Changed by Alexey Edelev 2012.09.11.CR 13020
                    //                              m_fileOperationData.destPath);
                    m_pFileOperationsHelper->copy(copyMoveList, //Changed by Alexey Edelev 2012.09.11.CR 13020
                                                  m_fileOperationData.destPath, AUDIO_FILE);
                    isCopyingMode = true; // modified by ravikanth 18.06.13 for ITS 0174790

                    // } modified by wspark
                    // { added by wspark 2012.12.18 for ISV65071
                    emit hidePopup(true); // modified by eugene.seo 2013.04.10
                    // { added by eugene.seo 2013.05.29
                    if(m_volumeType == eUSB_FRONT)
                        setCopyInProgress(true, 1);
                    else if(m_volumeType == eUSB_REAR)
                        setCopyInProgress(true, 2);
                    else
                        setCopyInProgress(true, 0);
                    // } added by eugene.seo 2013.05.29
                    emit signalShowPopup(LVEnums::POPUP_TYPE_COPYING, false, false);
                    // } added by wspark
                }
                else if (m_fileOperationData.operation == LVEnums::OPERATION_MOVE)
                {
                    emit signalShowPopup(LVEnums::POPUP_TYPE_MOVING, false, false);
                    MP_LOG << "FileOperationHelper move" << LOG_ENDL;
                    //{Changed by Alexey Edelev 2012.09.11.CR 13020
                    MP_LOG << "Current playing path: " << m_AudioController.GetCurrentFilePath() << LOG_ENDL;
                    if(copyMoveList.contains(m_AudioController.GetCurrentFilePath()))
                    {
                        m_AudioController.Stop();
                    }
                    //}Changed by Alexey Edelev 2012.09.11.CR 13020
                    m_pFileOperationsHelper->move(copyMoveList, //Changed by Alexey Edelev 2012.09.11.CR 13020
                                                  m_fileOperationData.destPath);
                }
                else if (m_fileOperationData.operation == LVEnums::OPERATION_DELETE)    // added by sungha.choi 2013.09.25 for ITS 0181308
                {
                    if(!m_fileOperationData.itemNames.isEmpty())
                    {
                        for (int i = 0; i < m_fileOperationData.itemNames.count(); i++)
                        {
                            m_fileOperationData.itemNames[i].prepend(FilePath_Prefix);
                            MP_LOG << "New folder path" << m_fileOperationData.itemNames[i] << LOG_ENDL;
                        }
                    }
                    m_fileOperationData.bCreatingUrlList = false;
                }
                else
                {
                    // { modified by wspark 2012.09.13 for CR13116
                    // below code is just for Folders. No need if there are only files.
                    /*
                    for (int i = 0; i < m_fileOperationData.itemNames.count(); i++)
                    {
                        m_fileOperationData.itemNames[i].prepend(FilePath_Prefix);
                        MP_LOG << "New folder path" << m_fileOperationData.itemNames[i] << LOG_ENDL;
                    }

                    MP_LOG << "Sending eGetMultipleFolderSongsQuery_Id" << LOG_ENDL;
                    m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames,
                                                          eGetMultipleFolderSongsQuery_Id);
                    */

                    if(!m_fileOperationData.itemNames.isEmpty())
                    {
	                    for (int i = 0; i < m_fileOperationData.itemNames.count(); i++)
	                    {
	                        m_fileOperationData.itemNames[i].prepend(FilePath_Prefix);
	                        MP_LOG << "New folder path" << m_fileOperationData.itemNames[i] << LOG_ENDL;
	                    }

	                    MP_LOG << "Sending eGetMultipleFolderSongsQuery_Id" << LOG_ENDL;
	                    m_pListTrackerAbstractor->RequestData(m_fileOperationData.itemNames,
	                                                          eGetMultipleFolderSongsQuery_Id);
                    }
                    else
                        m_fileOperationData.bCreatingUrlList = false;
                    // } modified by wspark
                }
                // } modified by ruinivbe
            }
            else
            {
                m_fileOperationData.bCreatingUrlList = false;
            }
            break;

        default:
            m_fileOperationData.bCreatingUrlList = false;
            break;
    }
}

// { add by yongkyun.lee@lge.com    2012.10.05 for CR 13484 (New UX: music(LGE) # MP3 File info POPUP
void CListModel::FileInfoPopUp()
{
    QString fileName,folderName, bitRate,fileFormat,CreateDate;

    emit hidePopup(true); // modified by eugene.seo 2013.04.10

    // { added by junam 2012.12.17 for CR15426
    if (m_AudioController.GetMediaMode() == MP::IPOD1 ||
            m_AudioController.GetMediaMode() == MP::IPOD2)
    {
        fileName   = "";
        folderName = "";
    }
    else // } added by junam
    {
        fileName   = m_AudioController.GetCurrentFileName();
        folderName = m_AudioController.GetCurrentFolderName();
    }

    bitRate    = m_AudioController.GetCurrentBitRate();
    fileFormat = m_AudioController.GetCurrentFormat();
    CreateDate = m_AudioController.GetCurrentCreateDate();
    MP_LOG << "[FileInfo]fileName   =" << fileName << LOG_ENDL;
    MP_LOG << "[FileInfo]folderName =" << folderName << LOG_ENDL;
    MP_LOG << "[FileInfo]bitRate    =" << bitRate << LOG_ENDL;
    MP_LOG << "[FileInfo]fileFormat =" << fileFormat << LOG_ENDL;
    MP_LOG << "[FileInfo]CreateDate =" << CreateDate << LOG_ENDL;

    // { added by  yongkyun.lee 2012.11.13 for ISV 62705
    //emit signalFileInfo( fileName, folderName , bitRate , fileFormat , CreateDate);
    emit signalFileInfo( folderName ,fileName,  bitRate , fileFormat , CreateDate);
    // { added by  yongkyun.lee
    emit signalShowPopup(LVEnums::POPUP_TYPE_DETAIL_FILE_INFO, false, false);

    return;
}
// } add by yongkyun.lee@lge.com

void CListModel::invokePendingFileOperation()
{
    MP_LOG << "itemNames.count() =" << m_fileOperationData.itemNames.count() << LOG_ENDL;
    MP_LOG << "fileURLs.count() =" << m_fileOperationData.fileURLs.count() << LOG_ENDL;
    MP_LOG << "trackTitles.count() =" << m_fileOperationData.trackTitles.count() << LOG_ENDL;
    MP_LOG << "operation =" << m_fileOperationData.operation << LOG_ENDL;

    // { modified by kihyung 2012.07.04 for CR 11277
    // if (m_fileOperationData.fileURLs.isEmpty())
    if (m_fileOperationData.fileURLs.isEmpty() && m_fileOperationData.itemNames.isEmpty())
    // } modified by kihyung
    {
        MP_LOG << "Warning! There is no files to copy!" << LOG_ENDL;
        return;
    }

    switch (m_fileOperationData.operation)
    {
        case LVEnums::OPERATION_COPY:
            //emit signalShowPopup(LVEnums::POPUP_TYPE_COPYING, false, false); // deleted by wspark 2012.12.18 for ISV65071
            // { modified by wspark 2012.12.14 for copying according to the media type.
            //m_pFileOperationsHelper->copy(m_fileOperationData.fileURLs,
            //                              m_fileOperationData.destPath);
            m_pFileOperationsHelper->copy(m_fileOperationData.fileURLs,
                                          m_fileOperationData.destPath, AUDIO_FILE);
			isCopyingMode = true; // added by eugene 2013.01.10 for ISV 69609
            // } modified by wspark
            // { added by wspark 2012.12.18 for ISV65071
            emit hidePopup(true); // modified by eugene.seo 2013.04.10
            // { added by eugene.seo 2013.05.29
            if(m_volumeType == eUSB_FRONT)
                setCopyInProgress(true, 1);
            else if(m_volumeType == eUSB_REAR)
                setCopyInProgress(true, 2);
            else
                setCopyInProgress(true, 0);
            // } added by eugene.seo 2013.05.29
            emit signalShowPopup(LVEnums::POPUP_TYPE_COPYING, false, false);
            // } added by wspark
            break;

        case LVEnums::OPERATION_MOVE:
            emit signalShowPopup(LVEnums::POPUP_TYPE_MOVING, false, false);
            m_pFileOperationsHelper->move(m_fileOperationData.fileURLs,
                                          m_fileOperationData.destPath);
            break;

        case LVEnums::OPERATION_DELETE:
            // { modified by kihyung 2012.07.04 for CR 11277
            // emit signalShowPopup(LVEnums::POPUP_TYPE_DELETING, false, false);
            // m_pFileOperationsHelper->deleteFiles(m_fileOperationData.fileURLs, AUDIO_FILE);
            {
                QList<QString> fileList;
                // { modified by wspark 2012.09.13 for CR13116
                // Below code is added only for deleting Folder Structure
                // So No need in other category.
                // itemNames : folder name to delete.
                /*
                for(int i = 0; i < m_fileOperationData.itemNames.count(); i++) {
                    MP_LOG << "wspark itemNames = " << m_fileOperationData.itemNames.at(i) << LOG_ENDL;
                    fileList.append(m_fileOperationData.itemNames.at(i));
                }
                */
                if(m_fileOperationData.category == FOLDER_DEPTH_0 || m_fileOperationData.category == FOLDER_DEPTH_1)
                {
                    for(int i = 0; i < m_fileOperationData.itemNames.count(); i++) {
                        fileList.append(m_fileOperationData.itemNames.at(i));
                    }
                }
                // } modified by wspark
                for(int i = 0; i < m_fileOperationData.fileURLs.count(); i++) {
                    fileList.append(m_fileOperationData.fileURLs.at(i));
                }

                if(fileList.count() > 0) {
                    emit signalShowPopup(LVEnums::POPUP_TYPE_DELETING, false, false);
                    m_currentDeletedFileCount = fileList.count();
                    m_pFileOperationsHelper->deleteFiles(fileList, AUDIO_FILE); // modified by ravikanth 02-09-13
                }
            }
            // } modified by kihyung
            break;

        case LVEnums::OPERATION_ADD_TO_PLAYLIST:
        {
            emit hidePopup(true); // modified by eugene.seo 2013.04.10
            playlistAdd(m_fileOperationData.plPath, m_fileOperationData.fileURLs,
                        m_fileOperationData.trackTitles);

            QString plName = m_fileOperationData.plPath.mid(m_fileOperationData.plPath.lastIndexOf("/") + 1);
            plName = plName.left(plName.indexOf("."));
            // { removed by junggil 2012.08.31 for CR13013 Name of the playlist is showing outside of popup
            //plName = plName.mid(0, 20);
            //plName.append("...");
            // } removed by junggil
            m_fileOperationData.operation = LVEnums::OPERATION_NONE; // modified by junggil 2012.10.26 for CR14229 Regression : Jukebox_Audio->Add To Playlist->List->Menu->Display Only "Cancel" Item
            emit finishEditMode();
            emit signalShowPopup(LVEnums::POPUP_TYPE_PL_ADD_COMPLETE, plName, false);
            break;
        }

        default:
            break;
    }
}

//added by yongkyun.lee 2012.11.07 for Miner controll
void CListModel::cancelOperation()
{
    isCopyingMode = false;
    m_pFileOperationsHelper->cancelOperation();
    MinerControler(false); // 0 is jukebox
}
//added by yongkyun

void CListModel::cancelFileOperation()
{
    MP_LOG << "operation =" << m_fileOperationData.operation << LOG_ENDL;
    // setCopyInProgress(false); // modified by ravikanth 19-04-13
    // { deleted by lssanh 2013.02.22 ISV73569
    //isCopyingMode = false; // added by eugene 2013.01.10 for ISV 69609
    //m_pFileOperationsHelper->setOverwriteMode(CANCEL_COPY_OPERATION_MODE);
    // } deleted by lssanh 2013.02.22 ISV73569
    //modified  by yongkyun.lee 2012.11.07 for Miner controll
    //m_pFileOperationsHelper->cancelOperation();
    //cancelOperation();
    //modified  by yongkyun.lee 2012.11.07 for Miner controll

    //emit hidePopup(true); // modified by eugene.seo 2013.04.10

    // { modified by eugene 2013.01.10 for ISV 69609
    // if (m_fileOperationData.operation == LVEnums::OPERATION_COPY)
    if (isCopyingMode)
	// } modified by eugene 2013.01.10 for ISV 69609
    {
        cancelOperation(); // added by lssanh 2013.02.22 ISV73569
        emit hidePopup(true); // modified by eugene.seo 2013.04.10
        emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
    }
    else
    {
        cancelOperation(); // added by lssanh 2013.02.22 ISV73569
        emit hidePopup(true); // modified by eugene.seo 2013.04.10
        if (m_fileOperationData.operation == LVEnums::OPERATION_MOVE)
        {
            updateActivePlaylist();
            emit signalShowPopup((int)LVEnums::POPUP_TYPE_MOVE_CANCELED, false, false);
        }
        else if (m_fileOperationData.operation == LVEnums::OPERATION_DELETE)
        {
            updateActivePlaylist();
        }
    }
    //isCopyingMode = false; // added by lssanh 2013.02.22 ISV73569
    m_fileOperationData.operation = LVEnums::OPERATION_NONE;
    emit cancelEditMode();
}

// { added by lssanh 2013.02.07 2USB
void CListModel::cancelOperationChangeSource(bool isRemoved)
{
    MP_LOG << (m_fileOperationData.operation == LVEnums::OPERATION_COPY) << LOG_ENDL;
    if (m_fileOperationData.operation == LVEnums::OPERATION_COPY)
    {
       m_pFileOperationsHelper->cancelOperation(isRemoved);
       m_fileOperationData.operation = LVEnums::OPERATION_NONE;
       // { added by lssanh 2013.03.26 ISV77213
       emit hidePopup(true); // modified by eugene.seo 2013.04.10
	   // modified by ravikanth 14.06.2013 for ITS 0174012
       //if(GetOSDDisplay() != DISPLAY_REAR) // added by eugene.seo 2013.06.08
       if( (MP::USB1 == ( MP::DEVICE_T ) m_AudioController.playerMode()
               ||  MP::USB2 == ( MP::DEVICE_T ) m_AudioController.playerMode()) && isCopyingMode) // modified by ravikanth on 12-09-13 for ITS 0189469, 0189876
           emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
       isCopyingMode = false;
       emit cancelEditMode();
       // } added by lssanh 2013.03.26 ISV77213
    }
    else if(m_AudioController.GetListViewModel()->isFileOperationInProgress())
    {
        m_pFileOperationsHelper->cancelOperation();
        m_fileOperationData.operation = LVEnums::OPERATION_NONE;
        isCopyingMode = false;
    }
}
// } added by lssanh 2013.02.07 2USB

bool CListModel::showEditPopup()
{
    static bool isFirstTime = true;
    bool ret = isFirstTime;

    if (isFirstTime)
    {
        emit signalShowPopup((int)LVEnums::POPUP_TYPE_EDIT_MODE, false, false);
        isFirstTime = false;
    }

    return ret;
}

void CListModel::tipPopup()
{
    // { removed by kihyung 2013.05.22
    /*
    MP_LOG << LOG_ENDL;
    if (!m_isDisplayTip)
    {
        emit signalShowPopup((int)LVEnums::POPUP_TYPE_TIP_ADD_PLAYLIST, false, false);
        m_isDisplayTip = true;
        AppMediaPlayer_SettingsStorage::GetInstance()->SaveData(&m_AudioController);
    }
    */
    // } removed by kihyung 2013.05.22    
}

//{modified by yongkyun.lee 2012.12.07 for file info Popup update
void
CListModel::isPopupID(int popupType)
{
    m_popupType = popupType;
    return;
}
//}modified by yongkyun.lee


void
CListModel::popupEventHandler(int popupType, int msg)
{
    MP_LOG << "popupType =" << popupType << ", msg =" << msg << LOG_ENDL;
	// { added by edo.lee 2012.12.08 for statusbar
	m_pFileOperationsHelper->setDeclarativeView(m_AudioController.GetEngineMain()
		.GetDeclarativeView(AppEngineQMLConstants::APP_FRONT_SCREEN));
	// } added by edo.lee
    switch ((LVEnums::POPUP_TYPE_STATE_T)popupType)
    {
        case LVEnums::POPUP_TYPE_EDIT_MODE:
            emit editPopupClosed();
            break;
        case LVEnums::POPUP_TYPE_MOVING:
            if (msg == 0)
            {
                MP_LOG << "Pause current operation" << LOG_ENDL;
                m_pFileOperationsHelper->pauseOperation();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_CANCEL_MOVE_QUESTION, false, false);
            }
            break;
        case LVEnums::POPUP_TYPE_COPYING:
            if (msg == 0)
            {
                MP_LOG << "Pause current operation" << LOG_ENDL;
                m_pFileOperationsHelper->pauseOperation();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_CANCEL_COPY_QUESTION, false, false);
            }
            break;
			// modified by ravikanth 19.06.13 for ITS 0174577
        case LVEnums::POPUP_TYPE_DELETING:
            if (msg == 0)
            {
                MP_LOG << "cancel current operation" << LOG_ENDL;
                cancelOperation();
                emit copyingPopupClosed();
            }
            break;
        case LVEnums::POPUP_TYPE_COPY_ALL:
        case LVEnums::POPUP_TYPE_MOVE_ALL:
            if (msg == 0)
            {
                setCopyAll(true); // add by wspark 2012.07.25 for CR12226
                emit copyAllConfirmed();
                //setCopyInProgress(true); // modified by ravikanth 19-04-13
            }
            else if(msg == 1)
            {
                emit resetSelectAllItems();
            }
            //{ deleted by yongkyun.lee 20130317 for : ISV 76094
            //else
            //    emit cancelEditMode();
            //} deleted by yongkyun.lee 20130317 
            break;

        case LVEnums::POPUP_QUICK_FOLDER_DELETE:
        case LVEnums::POPUP_QUICK_FILE_DELETE:
            if (msg == 0)
            {
                invokeQuickDelete();
            }
            else
            {
                m_fileOperationData.operation = LVEnums::OPERATION_NONE;
            }
            break;

        case LVEnums::POPUP_QUICK_PLAYLIST_DELETE:
        case LVEnums::POPUP_QUICK_PLAYLIST_FILE_DELETE:
            if (msg == 0)
            {
                editPlayListsCatalog(m_quickEdit.index, LVEnums::OPERATION_DELETE, "");
            }
            break;

        case LVEnums::POPUP_TYPE_DELETE:
            if (msg == 0)
            {
				// modified by ravikanth 25-08-13 for ITS 0184119 
                handleSelectAllItems();
                emit deleteAllConfirmed();
                //startFileOperation(); // commented by ravikanth on 07-09-13
            }
            else if(msg == 1)
            {
                emit resetSelectAllItems();
            }
            else if(msg == 2)
            {
                startFileOperation();
            }
            break;

        case LVEnums::POPUP_TYPE_CLEAR_JUKEBOX:
            if (msg == 1)
            {
                emit hidePopup(true); // modified by eugene.seo 2013.04.10
            }
            else if (msg == 0)
            {
                clearJukebox();
            }
            break;

        case LVEnums::POPUP_TYPE_REPLACE:
        {
			// { modified by eugene 2012.08.23 for static analysis defect #7094
			/*
           		 int popupType = (m_fileOperationData.operation == LVEnums::OPERATION_COPY) ?
                                LVEnums::POPUP_TYPE_COPYING : LVEnums::POPUP_TYPE_MOVING;
             */
			int popupType_replace = (m_fileOperationData.operation == LVEnums::OPERATION_COPY) ?
                          LVEnums::POPUP_TYPE_COPYING : LVEnums::POPUP_TYPE_MOVING;
            // setCopyInProgress(true); // modified by ravikanth 19-04-13
            if (msg == 0)
            {
                emit signalShowPopup(popupType_replace, false, false);
                m_pFileOperationsHelper->setOverwriteMode(OVERWRITE_ONE_EXIST_FILE_MODE);
            }
            else if (msg == 1)
            {
                emit signalShowPopup(popupType_replace, false, false);
                m_pFileOperationsHelper->setOverwriteMode(OVERWRITE_ALL_EXIST_FILE_MODE);
            }
            else if (msg == 2)
            {
                // { added by wspark 2012.09.21 for CR6084
                if(m_fileOperationData.category == FOLDER_DEPTH_0 || m_fileOperationData.category == FOLDER_DEPTH_1)
                {
                    if(m_fileOperationData.fileURLs.count() == 1 && m_fileOperationData.itemNames.count() == 0)
                    {
                        cancelFileOperation();
                        break;
                    }
                }
                else
                {
                    if(m_fileOperationData.fileURLs.count() == 1)
                    {
                        cancelFileOperation();
                        break;
                    }
                }
                // } added by wspark
                emit signalShowPopup(popupType_replace, false, false);
                m_pFileOperationsHelper->setOverwriteMode(NOT_OVERWRITE_EXIST_FILE_MODE);
            }
			// } modified by eugene 2012.08.23 for static analysis defect #7094
            else if (msg == 3)
            {
                // { modified by eugene.seo 2013.05.31
                MP_LOG << "[ESEO] Pause current operation" << LOG_ENDL;
                m_pFileOperationsHelper->pauseOperation();
				// modified by ravikanth 02-07-13 for ITS 0177437
				// modified by ravikanth 07-07-13 for ITS 0178185
                //m_pFileOperationsHelper->setOverwriteMode(OVERWRITE_ONE_EXIST_FILE_MODE, false); // modified by ravikanth 18.06.13 for ITS 0174610
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_CANCEL_COPY_QUESTION, true, false);
                //cancelFileOperation();
                // } modified by eugene.seo 2013.05.31
            }
            break;
        }

        case LVEnums::POPUP_TYPE_CANCEL_COPY_QUESTION:
            if (msg == 0)
            {
                // modified by minho 20121102 for display error bg on list screen when canceled move all/copy all.
                // cancelFileOperation();
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                //m_pFileOperationsHelper->cancelOperation();
                cancelOperation();
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                emit copyingPopupClosed();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
                // modified by minho
               // setCopyInProgress(false); // modified by ravikanth 19-04-13
            }
            else if (msg == 1)
            {
                m_pFileOperationsHelper->continueOperation();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPYING, false, false);
            }
			// modified by ravikanth 07-07-13 for ITS 0178185
            else if (msg == 2)
            {
                replaceFileRequest(QString("/") + m_currentReplaceFileName, -1); // modified by ravikanth 07-07-13 for ITS 0178184
            }
            else if (msg == 3)
            {
                emit cancelEditMode();
            }
            break;

        case LVEnums::POPUP_TYPE_CANCEL_MOVE_QUESTION:
            if (msg == 0)
            {
                // modified by minho 20121102 for display error bg on list screen when canceled move all/copy all.
                // cancelFileOperation();
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                //m_pFileOperationsHelper->cancelOperation();
                cancelOperation();
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                emit copyingPopupClosed();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_MOVE_CANCELED, false, false);
                // modified by minho
            }
            else if (msg == 1)
            {
                m_pFileOperationsHelper->continueOperation();
                emit signalShowPopup((int)LVEnums::POPUP_TYPE_MOVING, false, false);
            }
            break;

        case LVEnums::POPUP_TYPE_CAPACITY_ERROR_MANAGE:
        {
            if (msg == 0)
            {
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                //m_pFileOperationsHelper->cancelOperation();
                cancelOperation();
                emit finishEditMode(); // modified for ITS 0188177
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                //emit copyingPopupClosed(); // commented by ravikanth for ITS 0186736
                //emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
            }
            else if (msg == 1)
            {
                enableAllCheckBoxes(false);
                emit manageJB();
            }
            break;
        }
        // { added by sungkim for CR9616
        case LVEnums::POPUP_TYPE_UNAVAILABLE_FILE:
        case LVEnums::POPUP_TYPE_CORRUPT_FILE:
        {
            emit signalShowPopup((int)LVEnums::POPUP_TYPE_COPY_CANCELED, false, false);
            break;
        }
        // } added by sungkim for CR9616
        // { added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
        case LVEnums::POPUP_TYPE_NO_MUSIC_FILES:
        {
            if(getDisplayMode() == MP::JUKEBOX)
                ((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->HandleBackKey();
            break;
        }
        // } added by wspark
        // { added by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
        case LVEnums::POPUP_TYPE_COPY_CANCEL_INFO:
        {
            m_bCopyMode = false;//  added by yongkyun.lee 20130322 for : ISV 76741
            if (msg == 0)
            {
                emit copyCancelInfo();
            }
            //{ added by yongkyun.lee 20130323 for : ITS 160769
            // else
            //     emit cancelEditMode();
            //} added by yongkyun.lee 20130323 
            break;
         }
        // } added by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
		// modified by ravikanth 21-07-13 for copy cancel confirm on delete
        case LVEnums::POPUP_TYPE_CANCEL_COPY_FOR_DELETE_CONFIRM:
        {
            if (msg == 0)
            {
                cancelOperation();
                // modified by ravikanth 07-08-13 for ITS 0178894
                // @todo - instead of delaying the request from UI,
                // request changes in LGE_Controller for queue the request from UI
                QTimer::singleShot(300, this, SLOT(startFileOperation()));
                //startFileOperation();
            }
            else if(msg == 1)
            {
                cancelOperation();
                handleSelectAllItems();
                emit deleteAllConfirmed();
            }
            break;
        }
        case LVEnums::POPUP_TYPE_CANCEL_COPY_FOR_CLEAR_JUKEBOX:
        {
            if (msg == 0)
            {
                cancelOperation();
                signalShowPopup(LVEnums::POPUP_TYPE_CLEAR_JUKEBOX, false, false);
            }
            break;
        }
        //{ added by hyochang.ryu 20130731 for ITS181088
        case LVEnums::POPUP_TYPE_BT_DURING_CALL:	
        {
            emit hidePopup(true); 
            break;
        }
        //} added by hyochang.ryu 20130731 for ITS181088
        // { added by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
        case LVEnums::POPUP_TYPE_HIGH_TEMPERATURE:
        {
            emit hidePopup(true); 
            break;
        }
        // } added by cychoi 2014.04.09
        //{ added by sangmin.seol 2013.10.22 for ITS_0195517
        case LVEnums::POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM:
        {
            if (msg == 1)       // POPUP_TYPE_COPY_TO_JUKEBOX_CONFIRM Popup Cancel Button..
            {
                emit resetSelectAllItems();
            }

            break;
        }
        //} added by sangmin.seol 2013.10.22 for ITS_0195517
        default:
            MP_LOG << "Unhandled popup event" << LOG_ENDL;
            break;
    }
}

void
CListModel::ErrorMessage(int message, QVariant ext1, QVariant ext2)
{
    emit signalShowPopup(message, ext1, ext2);
}

void CListModel::replaceFileRequest(QString file, int totalFilesForCopy) // modified by ravikanth 07-07-13 for ITS 0178184
{
    MP_LOG << "file =" << file << LOG_ENDL;
	// modified by ravikanth 07-07-13 for ITS 0178185
    m_currentReplaceFileName = file.mid(file.lastIndexOf("/") + 1);
    m_currentReplaceCopyCount = totalFilesForCopy; // modified by ravikanth 07-07-13 for ITS 0178184
    emit hidePopup(true); // modified by eugene.seo 2013.04.10
    //	// { added by lssanh 2013.02.05 ISV72352
    //	if (m_fileOperationData.fileURLs.count() == 1)
    //           emit signalShowPopup(LVEnums::POPUP_TYPE_REPLACE, m_currentReplaceFileName, true);
    //	else
    //    // } added by lssanh 2013.02.05 ISV72352

	// modified by ravikanth 23-07-13 for remove copy duplicate popup and duplicate files
    // new spec request, replace all files
    int popupType_replace = (m_fileOperationData.operation == LVEnums::OPERATION_COPY) ?
                LVEnums::POPUP_TYPE_COPYING : LVEnums::POPUP_TYPE_MOVING;
    emit signalShowPopup(popupType_replace, false, false);
    m_pFileOperationsHelper->setOverwriteMode(OVERWRITE_ALL_EXIST_FILE_MODE);
    //emit signalShowPopup(LVEnums::POPUP_TYPE_REPLACE, m_currentReplaceFileName, false);
}

void CListModel::capacityError(int fileCount, qint64 fileSize)
{
    MP_LOG << "fileCount =" << fileCount << ", fileSize =" << fileSize << LOG_ENDL;
    emit signalShowPopup(LVEnums::POPUP_TYPE_CAPACITY_ERROR, fileCount, fileSize / MB_SIZE); // modified by ruindmby 2012.09.26 for CR#11543
}

// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
// added by ruindmby 2012.09.26 for CR#11543
int CListModel::RemainedCapacity()
{
   QString volumeName = QUrl("/usr_data").path();

   double fullSize = 0;
   double usedSize = 0;

   fullSize = double(GetUSBController()->GetTotalDiscSpace(volumeName)) / MB_SIZE ;
   usedSize = double(GetUSBController()->GetTotalUsedDiscSpace(volumeName)) / MB_SIZE ;

   return fullSize - usedSize;
}
// added by ruindmby 2012.09.26 for CR#11543

void CListModel::capacityView()
{
    MP_LOG << LOG_ENDL;
    QString volumeName = QUrl("/usr_data").path();

    // modified by ruindmby 2012.08.22 for CR 12454
    double fullSize = 0;
    double usedSize = 0;

    fullSize = double(GetUSBController()->GetTotalDiscSpace(volumeName)) / MB_SIZE ;
    usedSize = double(GetUSBController()->GetTotalUsedDiscSpace(volumeName)) / MB_SIZE;

    emit signalShowPopup(LVEnums::POPUP_TYPE_CAPACITY_VIEW, fullSize, usedSize);
    // modified by ruindmby 2012.08.22 for CR 12454
}
// } modified by eugeny.novikov 2012.12.10

QVariant
CListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount()) // modified by ravikanth 27.06.13 for ITS 0175963
    {
        return QVariant();
    }

    switch (role)
    {
        case imageRole:
        {
            //{changed by junam 2013.10.28 for 2nd depth coverart
            //QString imageURI = getMetaInfo( index.row(), m_listMetaInfo.imageSource);
            QString imageURI = m_imageMap.value(getMetaInfo( index.row(), m_listMetaInfo.itemSource));
            //}changed by junam
            if (imageURI.isEmpty())
            {
                if ((m_currentCategoryState == ALBUM_DEPTH_0)||(m_currentCategoryState == ARTIST_DEPTH_1)||  //added by junam 2013.10.23 for add aritst depth
                        (m_currentCategoryState == COMPOSER_DEPTH_1)|| (m_currentCategoryState == GENRE_DEPTH_2))
                    return QVariant::fromValue(QString("/app/share/images/music/coverflow_default_n.png"));
                else if ((m_currentCategoryState == FOLDER_DEPTH_0)||(m_currentCategoryState == FOLDER_DEPTH_1))
                {
                    if(getMetaInfo( index.row(), m_listMetaInfo.isFolder))
                        return QVariant::fromValue(QString("/app/share/images/music/ico_tab_list_folder.png"));
                }
            }
            return QVariant::fromValue(imageURI);
            // } changed by junam
        }

        case titleRole:
        {
            //{ added by junam 2013.12.31 for unknown
            QString itemTitle = getMetaInfo( index.row(), m_listMetaInfo.itemTitle);
            if(itemTitle.compare(" ") == 0)
                return QVariant::fromValue(QApplication::translate("main", "STR_MEDIA_UNKNOWN"));
            //}added by junam
            return QVariant::fromValue( itemTitle) ;
        }

        case urlRole:
        {
            return QVariant::fromValue( getMetaInfo( index.row(), m_listMetaInfo.itemSource)) ;
        }

        case firstCharRole:
        {
            if (!m_listMetaInfo.itemTitle.isEmpty())
            {
                 if (m_currentCategoryState == ALBUM_DEPTH_0
                        || m_currentCategoryState == SONG_DEPTH_0
                        || m_currentCategoryState == ARTIST_DEPTH_0
                        || m_currentCategoryState == COMPOSER_DEPTH_0
                        || m_currentCategoryState == GENRE_DEPTH_1)
                {
                    QString strKeyCode;
                    //{ modified by jaehwan.lee 2014.01.02 for pinyin sorting
                    wchar_t firstLetter;
                    if ( index.row() < 0 || index.row() >=  m_listMetaInfo.itemQuickIndex.count() )
                    {
                        firstLetter = wchar_t(" ");
                    }
                    else
                    {
                        firstLetter = m_listMetaInfo.itemQuickIndex.at(index.row());
                    }
                    return QVariant::fromValue(QString::fromWCharArray(&firstLetter , 1));
                   //} modified by jaehwan.
                }
            }

            return QVariant();
        }

        case checkBoxMarked:
        {
            // { changed by junam 2012.07.15 for CR 10719
            // modified by ravikanth 27.06.13 for ITS 0175963
            //if ( index.row() < m_iBlockOffset || index.row() >=  m_iBlockOffset +  m_listMetaInfo.isItemCheckBoxMarked.count() )
            //    return QVariant();
            //Q_ASSERT(m_listMetaInfo.isItemCheckBoxMarked.size() > index.row() - m_iBlockOffset);
            // return QVariant::fromValue(m_listMetaInfo.isItemCheckBoxMarked.at(index.row() - m_iBlockOffset));

            if ( index.row() < 0 || index.row() >= m_listMetaInfo.isItemCheckBoxMarked.count() ) // modified by ravikanth 27.06.13
                return QVariant();
            return QVariant::fromValue(m_listMetaInfo.isItemCheckBoxMarked.at(index.row()));

            //return (!m_listMetaInfo.isItemCheckBoxMarked.isEmpty()) ?
            //          QVariant::fromValue(m_listMetaInfo.isItemCheckBoxMarked.at(index.row())) :
            //           QVariant::fromValue(false);
            // } changed by junam
        }

        case selectableRole:
        {
            //{removed by junam 2013.08.30 for ITS_KOR_187183
            //if (m_currentRequest.tabId == "Play_list" && m_currentRequest.depth == 1)
            //{
            //    if ( index.row() < m_iBlockOffset || index.row() >=  m_iBlockOffset +  m_listMetaInfo.isSelectable.count() )
            //        return QVariant::fromValue(true);
            //    return QVariant::fromValue(m_listMetaInfo.isSelectable.at(index.row())- m_iBlockOffset);
            //}
            //else if ((m_currentRequest.tabId == "Folder") && m_bCopyMode )
            //{
            //    return QVariant::fromValue(getMetaInfo(index.row(), m_listMetaInfo.isFolder));
            //}
            // } removed by junam
            return QVariant::fromValue(true);
        }
    // { added by junam 2012.10.12 for CR14040
    case imageVisibleRole:
    {
        if ((m_currentCategoryState == ALBUM_DEPTH_0)||(m_currentCategoryState == ARTIST_DEPTH_1)||  //added by junam 2013.10.23 for add aritst depth
            (m_currentCategoryState == COMPOSER_DEPTH_1)|| (m_currentCategoryState == GENRE_DEPTH_2))
            return QVariant::fromValue(true);
        else if ((m_currentCategoryState == FOLDER_DEPTH_0)||(m_currentCategoryState == FOLDER_DEPTH_1))
            return QVariant::fromValue(getMetaInfo( index.row(), m_listMetaInfo.isFolder));
        return QVariant::fromValue(false);
    }
    // } added by junam
        default:
            return QVariant();
    }
}

int
CListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
	// { changed by junam 2012.07.15 for CR 10719
    //return m_listMetaInfo.itemTitle.count();
    return m_nTotalItemCount;
	// } changed by junam
}

// { added by junam 2012.07.15 for CR 10719
QString
CListModel::getMetaInfo( int itemIndex, const QStringList &itemList) const
{
    if ( itemIndex > rowCount() ||  itemIndex < 0 ||
            itemIndex >=  itemList.count() )
        return QString();

    //Q_ASSERT( itemIndex < itemList.count() + m_iBlockOffset );
    return itemList.at( itemIndex) ;
}

bool
CListModel::getMetaInfo( int itemIndex, const QList<bool> &itemList) const
{
    if ( itemIndex > rowCount() ||  itemIndex < 0 ||
            itemIndex >=  itemList.count() )
        return false;

    //Q_ASSERT( itemIndex < itemList.count() + m_iBlockOffset );
    return itemList.at( itemIndex) ;
}

bool
CListModel::setMetaInfo( int itemIndex, QStringList &itemList, QString value)
{
    if ( itemIndex > rowCount() ||  itemIndex < 0 ||
            itemIndex >=  itemList.count() )
        return false;

    //Q_ASSERT( itemIndex < itemList.count() + m_iBlockOffset );
    itemList[ itemIndex] = value ;
    return true;
}

bool
CListModel::setMetaInfo( int itemIndex, QList<bool> &itemList, bool value)
{
    if ( itemIndex > rowCount() ||  itemIndex < 0 ||
            itemIndex >=  itemList.count() )
        return false;

    //Q_ASSERT( itemIndex < itemList.count() + m_iBlockOffset );
    itemList[ itemIndex ] = value ;
    return true;
}
// } added by junam

void
CListModel::clear()
{
    MP_LOG << "ROWS START" << LOG_ENDL;
    //{ added by junam 2013.03.14 for list item selection crash
    setCurrentPlayingItem( -1 );
    emit clearSelection();
    //} added by juam

    beginRemoveRows(QModelIndex(), 0, m_nTotalItemCount - 1);
    m_nTotalItemCount = 0;
    m_itemIndexsOnScreen.clear(); // added by junam 2012.10.12 for CR14040
    m_listMetaInfo.imageSource.clear();
    m_listMetaInfo.itemName.clear();
    m_listMetaInfo.itemTitle.clear();
    m_listMetaInfo.itemQuickIndex.clear(); // added by jaehwan 2013.10.31 for fix quick scroll indexing issue of gener.
    m_listMetaInfo.itemSource.clear();
    m_listMetaInfo.itemArtist.clear(); // added by junam 2012.10.12 for CR14040
    m_listMetaInfo.isFolder.clear();
    m_listMetaInfo.isItemCheckBoxMarked.clear();
    m_listMetaInfo.isSelectable.clear();
    //setCurrentPlayingItem( -1 ); //removed by junam 2013.03.14 for list item selection crash
    endRemoveRows();
    MP_LOG << "ROWS END" << LOG_ENDL;
    //{added by junam 2014.03.09 for ISV74498
    m_currentPlayingItemPreparation = -1;
    m_nTotalItemCountPreparation = -1;
    //}added

    //Added by Tan 2013.10.02, m_countInfo should be initialized.
    if( m_AudioController.GetMediaMode() == MP::IPOD1 || m_AudioController.GetMediaMode() == MP::IPOD2)
    {
        m_countInfo = 0;
    }

    //m_trimDownMetaInfoTimer.stop();  //removed by junam 2013.07.23 for ONDEMAND_PLAYLIST

}

void
CListModel::resetRequestData()
{
    MP_LOG << LOG_ENDL;

    // { changed by junam 2012.11.29 for CR16170
    if(m_currentCategoryState == BOTTOM_DEPTH)
    {
        if(!m_lastRequestStack.isEmpty())
        {
            m_lastRequestStack.removeLast();
            if(m_lastRequestStack.isEmpty())
                m_currentRequest.queryStr = "";
            else
                m_currentRequest.queryStr = m_lastRequestStack.last();
        }
        else
        {
            m_currentRequest.queryStr = "";
        }

        //{ added by junam 2013.05.20 for OnDemandPlayList
        if(!m_lastRequestIndexStack.isEmpty())
        {
            m_lastRequestIndexStack.removeLast();
            if(!m_lastRequestIndexStack.isEmpty())
                m_currentRequest.itemIndex = m_lastRequestIndexStack.last();
        }
        //}added by junam
        m_currentRequest.depth--;
        m_currentCategoryState = UNDEFINED_STATE;
    }
    m_currentRequestCount = 0;


    //    m_currentRequest.tabId = "";
    //    m_currentRequest.queryStr = "";
    //    m_currentRequest.itemIndex = -1;
    //    m_currentRequest.depth = 0;
    //    m_currentRequestCount = 0;
    //    m_currentCategoryState = UNDEFINED_STATE; //added by junam 2012.10.22 more fix CR14040
    // } changed by junam


}

void
CListModel::createPlaylist(int Index, int contentPlayed)
{
    MP_LOG << "Index =" << Index << LOG_ENDL;
    emit disableListBtn();
    emit closeList();

    // { added by eugeny.novikov 2012.11.20 for CR 15408
    if (m_displayMode != m_AudioController.playerMode())
    {
        emit activateTab(m_displayMode, true, true);
    }
    // } added by eugeny.novikov 2012.11.20
    
    m_AudioController.createPlayListFromList(m_listMetaInfo.itemSource, Index, (MP::PLAYBACK_CONTENT_T)contentPlayed);
    emit enableListBtn();
}

/* Get list of songs without folders */
void
CListModel::createPlaylistFromFolderList(QString folderPath, int index)
{
    MP_LOG << "folderPath =" << folderPath << ", index =" << index << LOG_ENDL;

    QStringList itemPathList;
    int fileIndex = 0, newIndex = 0;
    for( int i = 0; i < m_listMetaInfo.itemSource.size() ; i++ )
    {
        if ( m_listMetaInfo.isFolder.at(i) == false)
        {
            itemPathList << m_listMetaInfo.itemSource.at(i);
            if ( i == index)
                newIndex = fileIndex;
            fileIndex++;
        }
    }

    emit disableListBtn();
    emit closeList();

    // { added by eugeny.novikov 2012.11.20 for CR 15408
    if (m_displayMode != m_AudioController.playerMode())
    {
        emit activateTab(m_displayMode, true, true);
    }
    // } added by eugeny.novikov 2012.11.20
    m_AudioController.createPlayListFromList(itemPathList, newIndex, MP::DEFAULT);
    emit enableListBtn();
}

void CListModel::addNewFolderToModel(QString folderPath)
{
    MP_LOG << "folderPath =" << folderPath << LOG_ENDL;

    if (folderPath.isEmpty())
        return;

    MP_LOG << "ROWS START" << LOG_ENDL;
    beginInsertRows(QModelIndex(), 0, 0);
    m_nTotalItemCount++;//Changed by Alexey Edelev 2012.09.11.CR 13020
    m_listMetaInfo.itemTitle.prepend(folderPath.mid(folderPath.lastIndexOf("/") + 1));
    m_listMetaInfo.itemName.prepend(folderPath);
    m_listMetaInfo.itemSource.prepend(folderPath);
    m_listMetaInfo.isFolder.prepend(true);
    // { modified by dongjin 2012.08.29 for New UX
    //m_listMetaInfo.imageSource.prepend("/app/share/images/music/icon_folder_n.png");
    m_listMetaInfo.imageSource.prepend("/app/share/images/music/ico_tab_list_folder.png");
    // } modified by dongjin
    m_listMetaInfo.isItemCheckBoxMarked.prepend(false);
    endInsertRows();
    MP_LOG << "ROWS END" << LOG_ENDL;
    
//{ Added by Radhakrushna for CR 13023 20120918
    setPlaybackIcon();
    emit setCurrentPlayingItemPosition();
//} Added by Radhakrushna for CR 13023 20120918

}
	//{modified by aettie 2013.04.05 for QA 41
// { changed by junam 2012.11.29 for CR16170
//void CListModel::requestUpdateListData()
int CListModel::requestUpdateListData(bool fromCategoryOrBackkey)
{
    MP_LOG << LOG_ENDL;
    //{added by junam 2013.04.16 for ITS162293
    if((m_AudioController.GetMediaMode() == MP::IPOD1 
		|| m_AudioController.GetMediaMode() == MP::IPOD2 )  // added by edo.lee 2013.04.20
		&& !m_AudioController.isMediaSyncfinished())
    {
        MP_LOG<< "isMediaSyncfinished " << m_AudioController.isMediaSyncfinished() << LOG_ENDL;
        return m_currentRequest.depth;
    }
    //}added by junam

    MP_LOG<< "requestListData" << m_currentRequest.tabId <<":"
         <<  m_currentRequest.itemIndex <<":"<< m_currentRequest.depth << LOG_ENDL;

    requestListData(m_currentRequest.tabId, m_currentRequest.itemIndex, m_currentRequest.depth, false, fromCategoryOrBackkey);

    return m_currentRequest.depth;
}
// } changed by junam


void CListModel::requestListData(QString tabId, int itemIndex, int historyStack, bool isRunFromFileManager, bool fromCategoryOrBackkey)
{
    MP_LOG << "(" << tabId << "," << itemIndex << "," << historyStack << ")" << LOG_ENDL;

    m_AudioController.setIsRunFromFileManager(isRunFromFileManager);
    
    if (tabId != "Folder")
    {
        setFolderHistoryStack(-1);
    }

    if (!m_pListTrackerAbstractor)
    {
        MP_HIGH << "Error: m_pListTrackerAbstractor NULL!" << LOG_ENDL; // modified by sangmin.seol 2014.06.02 change error case log level
        return;
    }

    
    emit showJBoxEmptyStr(false);
    
    bool isUpdatingCurrentView = (m_currentRequest.tabId == tabId &&
                                  m_currentRequest.depth == historyStack &&
                                  m_currentRequest.itemIndex == itemIndex);

    /* Backup request data */
    m_currentRequest.tabId = (tabId != "") ? tabId : m_currentRequest.tabId;
    m_currentRequest.itemIndex = itemIndex;
    m_currentRequest.depth = historyStack;

    if (m_currentRequestCount++ > 0)
    {
        /* Other request in progress, so we will process it after. */
        MP_HIGH << "Other request was queried!" << LOG_ENDL; // modified by sangmin.seol 2014.06.02 change error case log level
        return;
    }

    m_currentCategoryState = UNDEFINED_STATE;

    QString listViewHeader = "";

    if (isUpdatingCurrentView == true)
    {
        if(m_currentRequest.tabId == "Folder")
        {
            listViewHeader = m_currentRequest.queryStr.mid(m_currentRequest.queryStr.lastIndexOf("/") + 1);
        }
        else
        {
            listViewHeader = m_currentRequest.queryStr;
        }
    }
    else
    {
        m_currentRequest.queryStr = "";

        if (m_currentRequest.depth == 0)
        {
            /* Need to clear request stack */
            MP_LOG << "Calling m_lastRequestStack.clear()" << LOG_ENDL;
            m_lastRequestStack.clear();
            m_lastRequestIndexStack.clear();//added by junam 2013.05.20 for OnDemandPlayList
        }
        else
        {
            if (itemIndex != -1 && itemIndex < rowCount())
            {
                m_currentRequest.queryStr = getMetaInfo( itemIndex, m_listMetaInfo.itemName);
                listViewHeader = getMetaInfo( itemIndex, m_listMetaInfo.itemTitle);
                m_lastRequestStack.append(m_currentRequest.queryStr);
                m_lastRequestIndexStack.append(m_currentRequest.itemIndex);//added by junam 2013.05.20 for OnDemandPlayList
            }
            else if (itemIndex == -1 && !m_lastRequestStack.isEmpty())
            {
                m_lastRequestStack.removeLast();
                if(m_lastRequestStack.isEmpty())
                    m_currentRequest.queryStr = "";
                else
                    m_currentRequest.queryStr = m_lastRequestStack.last();

            }
        }
    }

    MP_LOG << "queryStr =" << m_currentRequest.queryStr << LOG_ENDL;
    MP_LOG << "listViewHeader =" << listViewHeader << LOG_ENDL;
    MP_LOG << "rowCount() =" << rowCount() << LOG_ENDL;

    m_volumeType = AudioUtils::VolumeType((MP::DEVICE_T)m_displayMode);
    if ( m_volumeType == eDefaultVolume ||
       ( m_volumeType == eIPOD_FRONT && !m_AudioController.isApple1Ready() ) ||
       ( m_volumeType == eIPOD_REAR && !m_AudioController.isApple2Ready() ) )
    {
        MP_HIGH<<"m_displayMode "<<m_displayMode<<", "<<m_AudioController.isApple1Ready()<<", "<<m_AudioController.isApple2Ready() << LOG_ENDL;
        this->clear();
        m_currentRequestCount--;
        return;
    }


    /* TODO: Replace following statements with switch by CATEGORY_STATE_T */
    if (m_currentRequest.tabId == "Song")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack == 0) ? SONG_DEPTH_0 : BOTTOM_DEPTH);
        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all song from volume */
                MP_LOG << "eGetAllSongsWithLimitFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                
                this->clear();
        
                if(m_volumeType == eJukeBox)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isMinerFinished());
                else if(m_volumeType == eUSB_FRONT)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB1MinerFinished());
                else if(m_volumeType == eUSB_REAR)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB2MinerFinished());

                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsFromVolumeQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER);

                //{added by junam 2013.07.20 for ITS_NA_179108
                if(m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                    listViewHeader = QApplication::translate("main", "STR_MEDIA_SONG");
                //}added by junam
                break;
            }
            case 1:
            {
                emit closeList();
                emit disableListBtn(); //added by junam 2013.12.19 for LIST_ENTRY_POINT
                if (m_displayMode != m_AudioController.playerMode())
                {
                    emit activateTab(m_displayMode, true, true);
                }

                if(( m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR ) &&  false == m_AudioController.isDatabaseReady())
                {
                    m_AudioController.createEmptyPlayList(MP::SONG, m_currentRequest.itemIndex);
                }
                else
                {
                    m_AudioController.createPlayListFromList( m_listMetaInfo.itemSource, m_currentRequest.itemIndex, MP::SONG);
                }
                emit enableListBtn();
                return; //added by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Album")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (ALBUM_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all Album from volume */
                MP_LOG << "eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                
                this->clear();

                if(m_volumeType == eJukeBox)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isMinerFinished());
                else if(m_volumeType == eUSB_FRONT)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB1MinerFinished());
                else if(m_volumeType == eUSB_REAR)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB2MinerFinished());

                m_pListTrackerAbstractor->RequestData( m_volumeType, eGetAllAlbumsWithUrlFromVolumeQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER);
                //{added by junam 2013.07.20 for ITS_NA_179108
                if(m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                    listViewHeader = QApplication::translate("main", "STR_MEDIA_ALBUM");
                //}added by junam
                break;
            }
            case 1:
            {

                /* Get all song from specified album */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAllSongsByAlbumFromVolumeQuery_Id from Volume =" << m_volumeType << LOG_ENDL;
                    
                    this->clear();
                    
                    m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByAlbumFromVolumeQuery_Id);
                }
                break;
            }
            case 2:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex,  MP::ALBUM);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Artist")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 2) ? (ARTIST_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all Artists from volume */
                MP_LOG << "eGetAllMusicArtistsWithLimitFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                
                this->clear();

                if(m_volumeType == eJukeBox)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isMinerFinished());
                else if(m_volumeType == eUSB_FRONT)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB1MinerFinished());
                else if(m_volumeType == eUSB_REAR)
                        m_pListTrackerAbstractor->setMinerStatus(m_volumeType, m_AudioController.isUSB2MinerFinished());

                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsFromVolumeQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER);
                //{added by junam 2013.07.20 for ITS_NA_179108
                if(m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                    listViewHeader = QApplication::translate("main", "STR_MEDIA_ARTIST");
                //}added by junam
                break;
            }
            case 1:
            {
                if (itemIndex == -1) //Back key, get Artist
                    listViewHeader = m_AudioController.encodeString(m_lastRequestStack.at(0));

                /* Get list of all Albums from specified Artist */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAllAlbumByArtistQuery_Id for" << m_currentRequest.queryStr << "from volumeType =" << m_volumeType << LOG_ENDL;
                    
                    this->clear();
                    //{changed by junam 2013.10.23 for add aritst depth
                    //m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByArtistFromVolumeQuery_Id);
                    m_pListTrackerAbstractor->RequestData(QStringList(m_currentRequest.queryStr), m_volumeType, eGetAllAlbumByArtistQuery_Id);
                    //}changed by junam
                }
                break;
            }
            case 2:
            {
                //{changed by junam 2013.10.23 for add aritst depth
                //m_AudioController.setIsPlayFromMLT(false);
                //createPlaylist(itemIndex, MP::ARTIST);
                this->clear();
                QStringList requestList;
                requestList.append(m_lastRequestStack.at(0)); // Get Artist
                requestList.append(m_lastRequestStack.at(1)); // Get Album

                if (!requestList.isEmpty())
                {
                    MP_LOG << "eGetAllSongsByArtistAlbumQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                    m_pListTrackerAbstractor->RequestData(requestList, m_volumeType,
                                                          eGetAllSongsByArtistAlbumQuery_Id);
                }
                //}changed by junam
                break;
            }
            case 3:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex, MP::ARTIST);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Composer")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 2) ? (COMPOSER_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all Composer from volume */
                MP_LOG << "eGetAllMusicComposersFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                this->clear();
                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicComposersFromVolumeQuery_Id);
                //{added by junam 2013.07.20 for ITS_NA_179108
                if(m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                    listViewHeader = QApplication::translate("main", "STR_MEDIA_COMPOSER");
                //}added by junam
                break;
            }
            case 1:
            {
                /* Get list of all Albums from specified Composer */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    if (itemIndex== -1 && listViewHeader == "")
                        listViewHeader = m_currentRequest.queryStr;   // added 2013.11.04

                    MP_LOG << "GetAllAlbumByComposerQuery_Id for " << m_currentRequest.queryStr << "from m_volumeType =" << m_volumeType << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(QStringList(m_currentRequest.queryStr), m_volumeType, eGetAllAlbumByComposerQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER);  // modified 2013.11.01
                }
                break;
            }
            case 2:
            {
                /* Get list of all Songs from specified Composer and Album */
                /* Firsr of all we should create QString list*/
                QStringList requestList;
                requestList.append(m_lastRequestStack.at(0)); // Get Composer
                requestList.append(m_lastRequestStack.at(1)); // Get Album

                if (!requestList.isEmpty())
                {
                    MP_LOG << "eGetAllSongsByComposerAlbumQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(requestList, m_volumeType, eGetAllSongsByComposerAlbumQuery_Id);
                }
                break;
            }
            case 3:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                /* Create playList from available songList */
                if (m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2)
                    createPlaylist(itemIndex, MP::COMPOSER);
                else
                    createPlaylist(itemIndex, MP::DEFAULT);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Genre")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (GENRE_DEPTH_0 + historyStack) : BOTTOM_DEPTH);
        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all genre from volume */
                MP_LOG << "eGetAllMusicGenresFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                this->clear();
                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicGenresFromVolumeQuery_Id);
                //{added by junam 2013.07.20 for ITS_NA_179108
                if(m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                    listViewHeader = QApplication::translate("main", "STR_MEDIA_GENRE");
                //}added by junam
                break;
            }
            case 1:
            {
                if (itemIndex == -1) //Back key, get Genre
                {
                    listViewHeader = m_AudioController.encodeString(m_lastRequestStack.at(0));
                }

                /* Request list of all Artist from specified Genre */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAllSongsByGenreFromVolumeQuery_Id for" << m_currentRequest.queryStr << "from volumeType =" << m_volumeType << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByGenreFromVolumeQuery_Id);                 
                }
                break;
            }
            case 2:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex, MP::GENRE);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
            case 3:
            {
                /* Get List of All songs from specified Artist, Genre and Album.
                   Firsr of all we should create QString list */
                QStringList requestList;
                requestList.append(m_lastRequestStack.at(0)); // Get Genre
                requestList.append(m_lastRequestStack.at(1)); // Get Artist
                requestList.append(m_lastRequestStack.at(2)); // Get Album

                if (!requestList.isEmpty())
                {
                    MP_LOG << "eGetAllSongsByGenreArtistAlbumQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(requestList, m_volumeType, eGetAllSongsByGenreArtistAlbumQuery_Id);
                }
                break;
            }
            case 4:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex, MP::GENRE);
                break;
            }
        }
    }
    else if (m_currentRequest.tabId == "Play_list")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (PLAYLIST_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        emit signalQuickScrollInfo(false);
        
        switch (historyStack)
        {
            case 0:
            {
                if (m_volumeType == eJukeBox ||
                    m_volumeType == eUSB_FRONT || m_volumeType == eUSB_REAR)
                {
                    setAudioPlaylistsData();
                    emit updateTextItemView("", 0);  //add 0 by junam 2013.09.06 for ITS_KOR_188332
                    return;
                }
                else
                {
                    /* Get List of all PlayList from volume */
                    MP_LOG << "eGetlistofPlaylistsQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                    this->clear();

                    //{changed by junam 2013.07.23 for ONDEMAND_PLAYLIST
                    //m_pListTrackerAbstractor->RequestData(m_volumeType, eGetlistofPlaylistsQuery_Id);
                    CIpodController* pController = GetIPodController();
                    if(pController)
                    {
                        pController->requestData( eGetlistofPlaylistsQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER , -1,0, 10); //changed by junam 2013.10.29 for playlist population
                    }
                    //}changed by junam

                    listViewHeader = QApplication::translate("main", "STR_MEDIA_PLAYLIST");//added by junam 2013.07.20 for ITS_NA_179108
                }
                break;
            }
            case 1:
            {
                /* Request list of all Songs from specified PlayList */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    if (m_volumeType == eJukeBox ||
                        m_volumeType == eUSB_FRONT || m_volumeType == eUSB_REAR)
                    {
                        setAudioPlaylistsData(m_currentRequest.queryStr);
                        break;
                    }
                    else
                    {
                        MP_LOG << "eGetAllSongsbyPlaylistQuery_Id from PlayList =" << m_currentRequest.queryStr << LOG_ENDL;
                        this->clear();

                        CIpodController* pController = GetIPodController();
                        if(pController)
                        {
                            //{ removed by junam 2013.10.29 for playlist population
                            //m_iScrollQueryOffset = 0;
                            //m_iScrollQueryLimit = ITEMS_PER_SCREEN*3; 
                            //} removed by junam
                            pController->requestData( eGetAllSongsbyPlaylistQuery_Id, REQUID_LIST_DYNAIC_FETCH_ENTER , m_currentRequest.itemIndex, 0, 10);
                        }
                    }
                }
                break;
            }
            case 2:
            {
                if ( getMetaInfo( itemIndex, m_listMetaInfo.isSelectable) || m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR )
                {
                    int index = -1;
                    QStringList srcList;
                    m_currentCategoryState = BOTTOM_DEPTH;  
                    //m_AudioController.setIsPlayFromMLT(false);
                    emit disableListBtn();
                    emit closeList();

                    if (m_displayMode != m_AudioController.playerMode())
                    {
                        emit activateTab(m_displayMode, true, true);
                    }
                    //{added by junam 2013.07.22 for ONDEMAND_PLAYLIST
                    if( (m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR ))
                    {
                        this->clear();
                        m_AudioController.createEmptyPlayList(MP::PLAYLIST, itemIndex, m_lastRequestIndexStack.at(0));
                    }
                    else //}added by junam
                    {
                        for (int i = 0; i < m_listMetaInfo.itemSource.size(); i++)
                        {
                            if (i == itemIndex)
                            {
                                index = srcList.size();
                            }
                            if ( m_listMetaInfo.isSelectable.isEmpty() || m_listMetaInfo.isSelectable[i])
                            {
                                srcList.append(m_listMetaInfo.itemSource.at(i));
                            }
                        }
                        //{changed by junam 2013.07.22 for ONDEMAND_PLAYLIST
                        //MP::PLAYBACK_CONTENT_T contentPlayed = MP::DEFAULT;
                        //if (m_displayMode == MP::JUKEBOX ||
                        //    m_displayMode == MP::USB1 || m_displayMode == MP::USB2)
                        //{
                        //    contentPlayed = MP::PLAYLIST;
                        //}
                        //m_AudioController.createPlayListFromList(srcList, index, contentPlayed);
                        m_AudioController.createPlayListFromList(srcList, index, MP::PLAYLIST);
                        //}changed by junam
                    }
                    emit enableListBtn();
                }
                else
                {
                    MP_LOG << "Cannot play..." << LOG_ENDL;
                    return;
                }

                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Podcast")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (PODCAST_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all Podcast from volume */
                MP_LOG << "eGetAllPodcastsByTitleQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                this->clear();
                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllPodcastsByTitleQuery_Id);
                //{changed by junam 2013.05.31 for ITS164261
                //listViewHeader = "Podcast";
                listViewHeader = QApplication::translate("main", "STR_MEDIA_PODCAST");
                //}changed by junam
                break;
            }
            case 1:
            {
                /* Request list of all a from specified podcast titles */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAllPodcastsEpisodeNamesQuery_Id from podcast =" << m_currentRequest.queryStr << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllPodcastsEpisodeNamesQuery_Id);
                }
                break;
            }
            case 2:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex, MP::PODCAST);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "itunes")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (ITUNES_DEPTH_0 + historyStack) : BOTTOM_DEPTH);

        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all itunes from volume */
                MP_LOG << "eGetAlliTunesUByTitleQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                this->clear();
                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAlliTunesUByTitleQuery_Id);
                //{changed by junam 2013.05.31 for ITS164261
                //listViewHeader = "iTunes U";
                listViewHeader = QApplication::translate("main", "STR_MEDIA_ITUNES_U");
                //}changed by junam
                break;
            }
            case 1:
            {
                /* Request list of all a from specified itunes titles */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAlliTunesUEpisodeNamesQuery_Id from podcast =" << m_currentRequest.queryStr << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAlliTunesUEpisodeNamesQuery_Id);
                }
                break;
            }
            case 2:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist(itemIndex, MP::ITUNES);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Audiobook")
    {
        m_currentCategoryState = CATEGORY_STATE_T((historyStack >= 0 && historyStack <= 1) ? (AUDIOBOOK_DEPTH_0 + historyStack) : BOTTOM_DEPTH);
        
        switch (historyStack)
        {
            case 0:
            {
                /* Get List of all AudioBook from volume */
                MP_LOG << "eGetAllAudiobooksByTitleQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
                this->clear();
                m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAudiobooksByTitleQuery_Id);
                //{changed by junam 2013.05.31 for ITS164261
                //listViewHeader = "Audio Book";
                listViewHeader = QApplication::translate("main", "STR_MEDIA_AUDIOBOOK");
                //}changed by junam
                break;
            }
            case 1:
            {
                /* Request list of all AudioBook from specified audioBook titles */
                if (!m_currentRequest.queryStr.isEmpty())
                {
                    MP_LOG << "eGetAllAudiobookChapterNamesQuery_Id from PlayList =" << m_currentRequest.queryStr << LOG_ENDL;
                    this->clear();
                    m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllAudiobookChapterNamesQuery_Id);
                }
                break;
            }
            case 2:
            {
                //m_AudioController.setIsPlayFromMLT(false);
                createPlaylist (itemIndex, MP::AUDIOBOOK);
                return; //changed by junam 2013.12.19 for LIST_ENTRY_POINT
            }
        }
    }
    else if (m_currentRequest.tabId == "Folder")
    {
        if (m_volumeType != eJukeBox && m_volumeType != eUSB_FRONT && m_volumeType != eUSB_REAR)
        {
            /* "Before indexing" feature available only for USB and Jukebox volumeType */
            MP_LOG << "Error: invalid volumeType" << LOG_ENDL;
            this->clear();
            QVector<QStringList> fileList;
            ListRequestComplete(fileList, eGetFolderTabList_Id);
            return;
        }
        MP_LOG << "requestListData tabid = Folder :  historyStack = "<<historyStack << LOG_ENDL;

        m_currentCategoryState = CATEGORY_STATE_T((historyStack == 0) ? FOLDER_DEPTH_0 : FOLDER_DEPTH_1);// added by junam 2012.10.12 for CR14040
        
        QString rootPath = (m_volumeType == eJukeBox)   ? JUKEBOX_ROOT :
                           (m_volumeType == eUSB_FRONT) ? m_AudioController.getMountPath(MP::USB1) :
                           (m_volumeType == eUSB_REAR)  ? m_AudioController.getMountPath(MP::USB2)
                                                        : AudioUtils::GetUSBRoot();

        QString tmp_stackedPlayingPath = rootPath;
        QString currentPlayingFile = m_AudioController.GetCurrentFilePath();
        //modified by aettie 20130522 folder name for NP
        int currentFolderCnt = currentPlayingFile.contains("rw_data")? currentPlayingFile.count("/") - 5:currentPlayingFile.count("/") - 4;
        switch (historyStack)
        {
            case 0:
            {
                if(fromCategoryOrBackkey || currentPlayingFile=="" || currentFolderCnt <= 0)
                {
                    this->clear();
                    MP_LOG << "FOLDERS: rootPath"<<rootPath << LOG_ENDL; 
                    emit updateTextItemView(listViewHeader, m_folderHistoryStack);  //chang by youngsim ISV 96195
                    m_pListTrackerAbstractor->RequestData(rootPath, eGetFolderTabList_Id);

                    //setFolderHistoryStack(currentFolderCnt);    // added by sungha.choi 2013.08.22 for ITS 0185510    // deleted.
                }
                else
                {
                    this->clear();

                    m_currentPlayingPath = currentPlayingFile.mid(0, currentPlayingFile.lastIndexOf("/"));
                    MP_LOG << "FOLDERS: m_currentPlayingPath"<<m_currentPlayingPath << LOG_ENDL; 
                    setFolderHistoryStack(currentFolderCnt); //chang by youngsim ISV 96195
                    listViewHeader = m_currentPlayingPath.mid(m_currentPlayingPath.lastIndexOf("/") + 1); //chang by youngsim ISV 96195
                    emit updateTextItemView(listViewHeader, m_folderHistoryStack);  //chang by youngsim ISV 96195
                    m_pListTrackerAbstractor->RequestData(m_currentPlayingPath, eGetFolderTabList_Id);

                    //setFolderHistoryStack(currentFolderCnt); //chang by youngsim ISV 96195
                    
                    //listViewHeader = m_currentPlayingPath.mid(m_currentPlayingPath.lastIndexOf("/") + 1); //chang by youngsim ISV 96195

                    for(int i=0;i<currentFolderCnt;i++){
                        tmp_stackedPlayingPath = currentPlayingFile.mid(0, currentPlayingFile.indexOf("/", (tmp_stackedPlayingPath.length() + 1)));
                        m_lastRequestStack.append(tmp_stackedPlayingPath);                        
                    }
                }
                break;

            }
            default:
            {
                if (isUpdatingCurrentView || (itemIndex >= 0 && itemIndex < m_listMetaInfo.isFolder.size() && m_listMetaInfo.isFolder.at(itemIndex)) || (itemIndex == -1))
                {
                    this->clear();
                    bool retVal = false;
                    QString requestPath = "";
                    requestPath = m_currentRequest.queryStr;
                    MP_LOG << "FOLDERS: requestPath"<<requestPath << LOG_ENDL;
                    listViewHeader = requestPath.mid(requestPath.lastIndexOf("/") + 1); //chang by youngsim ISV 96195
                    m_currentPlayingPath = requestPath; //chang by youngsim ISV 96195
                    emit updateTextItemView(listViewHeader, m_folderHistoryStack);  //chang by youngsim ISV 96195
                    retVal = m_pListTrackerAbstractor->RequestData( requestPath, eGetFolderTabList_Id);
                    //listViewHeader = requestPath.mid(requestPath.lastIndexOf("/") + 1); //chang by youngsim ISV 96195
                    //m_currentPlayingPath = requestPath; //chang by youngsim ISV 96195

                    if (!retVal)
                    {
                        updateCountInfoString("Songs", 0, 0);
                    }
                }
                else
                {
                    m_currentCategoryState = BOTTOM_DEPTH;
                    //m_AudioController.setIsPlayFromMLT(false);
                    emit closeList();
                    setFolderHistoryStack(-1);
                    //emit updateTextItemView(listViewHeader, m_folderHistoryStack);  //removed by junam 2013.12.30 for LIST_ENTRY_POINT
                    if (m_displayMode != m_AudioController.playerMode())
                    {
                        emit activateTab(m_displayMode, true, true);
                    }

                    if (m_displayMode == MP::JUKEBOX || m_displayMode == MP::USB1 || m_displayMode == MP::USB2)
                    {
                        m_selectedFileFromList = (itemIndex >= 0 && itemIndex < m_listMetaInfo.itemSource.size()) ? m_listMetaInfo.itemSource.at(itemIndex) : "";
                        m_pListTrackerAbstractor->RequestData(AudioUtils::VolumeType((MP::DEVICE_T)m_displayMode), eGetMusicLimitFileListDir_Id, 
                                                              GET_TA_REQUID(REQUID_LIST_ALL_FILES, m_displayMode));
                    }
                    else
                    {
                        createPlaylistFromFolderList(m_currentRequest.queryStr, itemIndex);
                    }
                    return; //added by junam 2013.12.30 for LIST_ENTRY_POINT
                }
                break;
            }
        }
        m_currentRequestCount = 0;
        emit signalQuickScrollInfo(false);
        //modified by Dmitry 30.09.13 for ITS0192158
        //emit updateTextItemView(listViewHeader, m_folderHistoryStack);  chang by youngsim ISV 96195 //changed by junam 2013.09.06 for ITS_KOR_188332
        return;
    }
    else
    {
        MP_LOG << "Warning: Unhandled category" << LOG_ENDL;
        m_currentRequestCount--;
    }

    //{changed by junam 2013.06.10 for ITS_179109
    //bool quickInfo = false;
    //CATEGORY_STATE_T quickCategory = calcCategoryState();
    //if (quickCategory == ARTIST_DEPTH_0 ||
    //    quickCategory == SONG_DEPTH_0 ||
    //    quickCategory == ALBUM_DEPTH_0||
    //    quickCategory == GENRE_DEPTH_1)
    //{
    //    quickInfo = true;
    //}
    bool quickInfo = isQuickViewVisible();
    //}changed by junam

    emit signalQuickScrollInfo(quickInfo);
    emit updateTextItemView(listViewHeader, historyStack);  //changed by junam 2013.09.06 for ITS_KOR_188332
}

void CListModel::ListRequestComplete(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{ 
    MP_HIGH << "queryStr =" << MAP_QUERY_IDS.key(queryId) << ", queryId =" << queryId << "fileList.count() =" << fileList.count() << LOG_ENDL;
    //MP_HIGH << "fileList.count() =" << fileList.count() << LOG_ENDL;

    // { removed by kihyung 2013.05.24
    /*
    if(reqUID != REQUID_LIST_DYNAIC_FETCH_ENTER)
    {
        emit signalListLoadingIcon(false);
    }
    */
    // } removed by kihyung 2013.05.24    

    if (m_currentRequestCount > 1 &&
        !(queryId == eGetAllSongsWithTitleCountFromVolumeQuery_Id ||
          queryId == eGetAllAlbumsCountFromVolumeQuery_Id ||
          queryId == eGetAllMusicArtistsCountFromVolumeQuery_Id ||
          queryId == eGetSongIndexFromVolumeQuery_Id ||
          queryId == eGetAlbumIndexFromVolumeQuery_Id ||
          queryId == eGetArtistIndexFromVolumeQuery_Id ||
          queryId == eGetSongIndexWithTitleCountFromVolumeQuery_Id ||    // { added by sungha.choi 2013.07.23
          queryId == eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id ||
          queryId == eGetArtistIndexWithArtistCountFromVolumeQuery_Id))    // } added by sungha.choi 2013.07.23
    {
        /* We already have new request, so skip current and process new */
        MP_LOG << "Ignoring received data due to pending request!" << LOG_ENDL;
        m_currentRequestCount = 0;
        requestListData(m_currentRequest.tabId, m_currentRequest.itemIndex, m_currentRequest.depth);
        return;
    }

    /************************************************************************/
    switch (queryId)
    {
        /* Song */
        case eGetAllSongsFromVolumeQuery_Id:
        {
            if (m_fileOperationData.bCreatingUrlList)
            {
                // { Added by sangmin.seol NOCR Fix to copy-all fail in genre_depth_0, m_fileOperationData.fileURLs have genre item name. clear it..
                if(m_fileOperationData.category == GENRE_DEPTH_0 && m_fileOperationData.fileURLs.size() > 0)
                {
                    m_fileOperationData.fileURLs.clear();
                }
                // } Added by sangmin.seol NOCR Fix to copy-all fail in genre_depth_0, m_fileOperationData.fileURLs have genre item name. clear it..

                foreach (QStringList list, fileList)
                {
                    m_fileOperationData.fileURLs.append(list[0]);

                    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        m_fileOperationData.trackTitles.append(list[1]);
                }
                m_fileOperationData.bCreatingUrlList = false;

                // { Modified by sangmin.seol ITS_0209610 Change condition for consider to delete-all & copy-all case in ARTIST_DEPTH_0, ALBUM_DEPTH_0, GENRE_DEPTH_0
                if( CheckBoxSelectable_count == m_nTotalItemCount
                    && ( m_fileOperationData.category == ARTIST_DEPTH_0
                    || m_fileOperationData.category == ALBUM_DEPTH_0
                    || m_fileOperationData.category == GENRE_DEPTH_0) )
                {
                    invokePendingFileOperation();
                }
	            // } Added by sangmin.seol ITS_0189462 Change query for copy all in artist, genre, album category depth 0							
            }
            else
            {
//{changed by junam 2013.10.24
                updateListData(fileList, queryId, reqUID);
                //if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER
                //        || ( reqUID == REQUID_LIST_DATABASE_UPDATE && m_currentFocuseIndex < 0 ))
                //{
                //    emit setCurrentPlayingItemPosition();
                //}
//}changed by junam
            }
//            else
//            {
//                int startIndex = m_currentRequest.itemIndex;
                
//                /* Handle click on song to create a playlist from all songs from volume */
//                this->clear();
//                //m_AudioController.setIsPlayFromMLT(false);
//                QStringList itemSources;
//                foreach (QStringList list, fileList)
//                {
//                    list[0].remove(FilePath_Prefix);
//                    itemSources.append(list[0]);
//                }

//                emit closeList();

//                if (m_displayMode != m_AudioController.playerMode())
//                {
//                    emit activateTab(m_displayMode, true, true);
//                }

//                m_AudioController.createPlayListFromList(itemSources, startIndex, MP::SONG);

//                emit enableListBtn();
//            }
            break;
                }

        case eGetAllAlbumsWithUrlFromVolumeQuery_Id:   
        case eGetAllAlbumByComposerQuery_Id: //aded by junam 2013.10.28 for 2nd depth coverart
            //{changed by junam 2013.10.24
            updateListData(fileList, queryId, reqUID);
            //if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER
            //        || ( reqUID == REQUID_LIST_DATABASE_UPDATE && m_currentFocuseIndex < 0 ))
            //{
            //    emit setCurrentPlayingItemPosition();
            //}
            //}changed by junam
            break;


        /* All Artists, Albums */
        case eGetAllMusicArtistsFromVolumeQuery_Id:
        case eGetAllAlbumsFromVolumeQuery_Id:
            if (m_fileOperationData.bCreatingUrlList)
            {
                m_fileOperationData.itemNames.clear();
                foreach (QStringList list, fileList)
                {
                    m_fileOperationData.itemNames.append(list[0]);
                }
                m_fileOperationData.bCreatingUrlList = false;
            }
            else
            {  
                //{changed by junam 2013.10.24
                updateListData(fileList, queryId, reqUID);
                //if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER
                //        || ( reqUID == REQUID_LIST_DATABASE_UPDATE && m_currentFocuseIndex < 0 ))
                //{
                //    emit setCurrentPlayingItemPosition();
                //}
                //}changed by junam
            }
            break;

        /* Song */
        case eGetAllSongsByArtistAlbumQuery_Id:
        case eGetAllSongsbyPlaylistQuery_Id:
        case eGetAllSongsWithLimitFromVolumeQuery_Id:
        case eGetAllSongsByComposerAlbumQuery_Id:
        case eGetAllSongsQuery_Id:
        {
            //{added by junam 2013.06.25 for ITS175216
            if(reqUID == REQUID_LIST_DATABASE_UPDATE)
            {
                replaceListData(fileList, queryId);
            }
            else //}added by junam
            {
                addChunkToListModel(fileList);
            }
            updateCountInfoString("Songs", m_nTotalItemCount);
            break;
        }

        /* Song */
        case eGetAllSongsByAlbumFromVolumeQuery_Id:
        case eGetAllSongsByGenreArtistAlbumQuery_Id:
        case eGetAllSongsByArtistFromVolumeQuery_Id:
        case eGetAllSongsByGenreFromVolumeQuery_Id:
        /* iPod */
        case eGetAllPodcastsEpisodeNamesQuery_Id:
        case eGetAllAudiobookChapterNamesQuery_Id:
        case eGetAlliTunesUEpisodeNamesQuery_Id:
        {
            //{added by junam 2013.06.25 for ITS175216
            if(reqUID == REQUID_LIST_DATABASE_UPDATE)
            {
                replaceListData(fileList, queryId);
            } 
            else //}added by junam
            {
                foreach (QStringList list, fileList)
                {
                    list[1].remove(FilePath_Prefix);

                    m_listMetaInfo.itemSource.append(list[1]);
                    m_listMetaInfo.itemTitle.append(m_AudioController.encodeString(list[0]));
                    m_listMetaInfo.itemName.append(list[0]);
                    m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[0])); // added by jaehwan 2013.10.31 to avoid crash
                    //m_listMetaInfo.isItemCheckBoxMarked.append(false); // modified by ravikanth 27.06.13 for ITS 0175963
                }
                makeQuckIndexTable(); //added jaehwan 2013.11.11 for quck index table
                beginInsertRows(QModelIndex(), 0, fileList.count() - 1);
                m_nTotalItemCount = fileList.count();
                endInsertRows();
				// modified by ravikanth 27.06.13 for ITS 0175963
                for(int i = m_nTotalItemCount; i > 0; i--)
                {
                    m_listMetaInfo.isItemCheckBoxMarked.append(false);
                }

                setPlaybackIcon();
                emit setCurrentPlayingItemPosition();
            }

            QString catStr = (queryId == eGetAllPodcastsEpisodeNamesQuery_Id ||
                              queryId == eGetAlliTunesUEpisodeNamesQuery_Id)   ? "Episodes" :
                             (queryId == eGetAllAudiobookChapterNamesQuery_Id) ? "Chapters"
                                                                               : "Songs";
            updateCountInfoString(catStr, rowCount());
            break;
        }

        /* Artist */
        case eGetAllMusicArtistsWithLimitFromVolumeQuery_Id:
        case eGetAllArtistByGenreQuery_Id:
        case eGetAllMusicArtistsQuery_Id:
        /* Composer */
        case eGetAllMusicComposersFromVolumeQuery_Id:
        /* Genre */
        case eGetAllMusicGenresFromVolumeQuery_Id:
        /* iPod */
        case eGetAllAudiobooksByTitleQuery_Id:
        case eGetAllPodcastsByTitleQuery_Id:
        case eGetAlliTunesUByTitleQuery_Id:
        case eGetlistofPlaylistsQuery_Id:
        {
            //{added by junam 2013.06.25 for ITS175216
            if(reqUID == REQUID_LIST_DATABASE_UPDATE)
            {
                replaceListData(fileList, queryId);
            }
            else //}added by junam
            {
                addChunkToListModel(fileList);

                //{added by junam 2013.10.30
                if(fileList.isEmpty())
                    emit emptyItemViewList(m_currentRequest.depth);
                //}added by junam
            }

            QString catStr = (queryId == eGetAllMusicGenresFromVolumeQuery_Id) ? "Genres" :
                             (queryId == eGetAllAudiobooksByTitleQuery_Id)     ? "Audiobooks" :
                             (queryId == eGetAllPodcastsByTitleQuery_Id)       ? "Podcasts" :
                             (queryId == eGetlistofPlaylistsQuery_Id)          ? "Playlists":
                             (queryId == eGetAlliTunesUByTitleQuery_Id)        ? "iTunes U" :
                             (queryId == eGetAllMusicComposersFromVolumeQuery_Id)  ? "Composer"  //added by junam 2013.11.21 for ISV_NA_90331
                                                                               : "Artists";
            updateCountInfoString(catStr, m_nTotalItemCount);
            break;
        }

        /* Album */
        case eGetAllAlbumsWithCoverArtFromVolumeQuery_Id:
        case eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id:
        {
            addChunkToListModel(fileList);
            updateCountInfoString("Albums", m_nTotalItemCount);
            break;
        }

        /* Album */
        case eGetAllAlbumByArtistQuery_Id:
        case eGetAllAlbumByGenreArtistQuery_Id:
        //case eGetAllAlbumByComposerQuery_Id: //removed by junam 2013.10.28 for 2nd depth coverart
        {
            if(reqUID == REQUID_LIST_DATABASE_UPDATE)
            {
                replaceListData(fileList, queryId);
            }
            else //}added by junam
            {
                //{changed by junam 2013.10.28 for 2nd depth coverart
                foreach (QStringList list, fileList)
                {
                    m_listMetaInfo.itemTitle.append(list[0]);
                    m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[0])); //added by jaehwan 2013.10.31 for quick scroll index.
                    m_listMetaInfo.itemName.append(list[0]);
                    m_listMetaInfo.itemSource.append(list.size() < 2 ? "" : list.at(1));
                    m_listMetaInfo.itemArtist.append(m_currentRequest.queryStr);
                    //m_listMetaInfo.imageSource.append(QString());
                }
                //}changed by junam

                makeQuckIndexTable(); // added by jaehwan 2013.11.11 for quick index table

                beginInsertRows(QModelIndex(), 0, m_listMetaInfo.itemTitle.size() - 1);
                m_nTotalItemCount = m_listMetaInfo.itemTitle.size();
                endInsertRows();
                for(int i = m_nTotalItemCount; i > 0; i--)
                {
                    m_listMetaInfo.isItemCheckBoxMarked.append(false);
                }

                //m_pListTrackerAbstractor->RequestData( albumName, artistName, eGetMusicAlbumCoverArt_Id);//removed by junam 2013.06.25 for ITS175216
                setPlaybackIcon();
                emit setCurrentPlayingItemPosition();
            }
            //{changed by junam 2013.06.25 for ITS175216
            //int count = (category == ARTIST_DEPTH_1) ? rowCount() - 1 : rowCount();
            //int count = (m_currentCategoryState == ARTIST_DEPTH_1) ? rowCount() - 1 : rowCount();
            //}changed by junam
            int count = rowCount();  //modified by youngsim.jo for artist depth 1 count.
            updateCountInfoString("Albums", count);
            break;
        }

        /* Album cover art */
        case eGetMusicAlbumCoverArt_Id:
        {
            int startIndex = 0;

            if (ARTIST_DEPTH_1 == calcCategoryState())
            {
                m_listMetaInfo.imageSource.append("");
                startIndex++;
            }

            foreach (QStringList list, fileList)
            {
                if (list[0].contains ("No CoverArt Image Found") ||
                   (list[0].count() && list[0].endsWith("- Error - No artist found")) ||
                        !isCoverFileSizeValid((QString)list[0]) )
                {
                    m_listMetaInfo.imageSource.append("/app/share/images/music/coverflow_default_n.png");
                }
                else
                {
                    m_listMetaInfo.imageSource.append(list[0]);
                }
            }

            if (startIndex < m_listMetaInfo.imageSource.size())
            {
                emit dataChanged(this->index(startIndex), this->index(m_listMetaInfo.imageSource.size()-1));
            }
            break;
        }

        /* Counters */
        case eGetAllSongsWithTitleCountFromVolumeQuery_Id:
        case eGetAllAlbumsCountFromVolumeQuery_Id:
        case eGetAllMusicArtistsCountFromVolumeQuery_Id:
        {
            if(reqUID >= REQUID_LIST_JUKEBOX_CATEGORY_AVAILABLE && reqUID <= REQUID_LIST_USB2_CATEGORY_AVAILABLE)   // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
            {
                int totalItemCount = 0;
                if (!fileList.isEmpty() && !fileList[0].isEmpty())
                {
                    MP_HIGH << "totalItemCount =" << fileList[0][0] << LOG_ENDL;
                    totalItemCount = fileList[0][0].toInt();
                }
                else
                {
                    MP_HIGH << "totalItemCount = empty" << LOG_ENDL;
                    totalItemCount = 0;
                }

                if(totalItemCount > 0) {
                    if(reqUID == REQUID_LIST_JUKEBOX_CATEGORY_AVAILABLE) {
                        m_bCategoryTabAvailableJukeBox = true;
                        MP_LOG << "JukeBox category list enable" << LOG_ENDL;
                    } else if(reqUID == REQUID_LIST_USB1_CATEGORY_AVAILABLE) {
                        m_bCategoryTabAvailableUSB1 = true;
                        MP_LOG << "USB1 category list enable" << LOG_ENDL;
                    } else if(reqUID == REQUID_LIST_USB2_CATEGORY_AVAILABLE) {
                        m_bCategoryTabAvailableUSB2 = true;
                        MP_LOG << "USB2 category list enable" << LOG_ENDL;
                    }
                    //{added by junam 2013.11.01 for category disabled state
                    if(isCategoryTabAvailable())
                    {
                        MP_LOG << " categoryTabAvailableTimer - STOP " << LOG_ENDL;
                        m_categoryTabAvailableTimer.stop();
                    }
                    //}added by junam

                    if(m_AudioController.isListView()) {
                        if( (m_displayMode == MP::JUKEBOX && reqUID == REQUID_LIST_JUKEBOX_CATEGORY_AVAILABLE)
                        ||  (m_displayMode == MP::USB1 && reqUID == REQUID_LIST_USB1_CATEGORY_AVAILABLE)
                        ||  (m_displayMode == MP::USB2 && reqUID == REQUID_LIST_USB2_CATEGORY_AVAILABLE) ) {
                                                //added by aettie 20130808 for category dimming(ITS 182674)
                            MP_LOG << "categorydim : emit categoryTabUpdate" << LOG_ENDL;
                            emit categoryTabUpdated(true);  // modified by sungha.choi 2013.08.12 for ITS 182674_3rd
                        }
                    }
                }
            }   // } added by sungha.choi 2013.08.05 for ITS 182674
            else   // modified by sungha.choi 2013.09.05
            {
                if (!fileList.isEmpty() && !fileList[0].isEmpty())
                {
                    MP_LOG << "m_nTotalItemCount =" << fileList[0][0] << LOG_ENDL;
                    m_nTotalItemCountPreparation = fileList[0][0].toInt();
                }
                else
                {
                    m_nTotalItemCountPreparation = 0;
                }
            }
            return;
        }

        case eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id:
        case eGetMultipleGenreSongsFromVolumeQuery_Id:
        case eGetMultipleArtistSongsByGenreFromVolumeQuery_Id:
        case eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id:
        case eGetMultipleFolderSongsQuery_Id:
        case eGetMultipleArtistSongsFromVolumeQuery_Id:
        case eGetMultipleAlbumSongsFromVolumeQuery_Id:
        {
            MP_LOG << "Handle multiple items response" << LOG_ENDL;
            if (m_fileOperationData.bCreatingUrlList)
            {
                foreach (QStringList list, fileList)
                {
                    m_fileOperationData.fileURLs.append(list[1]);

                    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
                        m_fileOperationData.trackTitles.append(list[0]);
                }

                m_fileOperationData.bCreatingUrlList = false;
                invokePendingFileOperation();
            }
            break;
        }
        
        case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
        case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
        case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
        {
            if (!fileList.isEmpty() && !fileList[0].isEmpty())
            {
                int indexQuickScroll = fileList[0][0].toInt();

                if (indexQuickScroll < 0 || indexQuickScroll > m_nTotalItemCount)
                {
                    MP_LOG << "Warning! Tracker returned invalid quilk scroll index" << LOG_ENDL;
                    return;
                }

                if (m_quickScrollIdx == indexQuickScroll)
                {
                    MP_LOG << "duplicated request skipped =" << fileList[0][0] << LOG_ENDL;
                    return;
                }
                m_quickScrollIdx = indexQuickScroll;
                m_iTopItemIdx = indexQuickScroll;

                if(m_nTotalItemCount > FETCHED_BLOCK_ITEM_COUNT)
                {
                    int iOffset = -1, iLimit = 0;
                    int iCenter = indexQuickScroll + (ITEMS_PER_SCREEN / 2);
                    //iCenter = (iCenter < 0) ? 0 : iCenter;//remove by edo.lee 2013.07.15 STATIC TEST

                    if (getEmptyBlock(iCenter, iOffset, iLimit))
                    {
                        m_pListTrackerAbstractor->setQueryOffset(iOffset);
                        m_pListTrackerAbstractor->setQueryLimit(iLimit);

                        //{add reqUID  by junam 2013.07.25 for ITS_EU_180117
                        if (queryId == eGetAllSongsWithTitleIndexFromVolumeQuery_Id)
                            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithLimitFromVolumeQuery_Id, reqUID);
                        else if (queryId == eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id)
                            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, reqUID);
                        else if (queryId == eGetAllMusicArtistsIndexFromVolumeQuery_Id)
                            m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsWithLimitFromVolumeQuery_Id, reqUID);
                        break;
                        //}changed by junam
                    }
                    else
                    {
                        //m_trimDownMetaInfoTimer.start(500);//removed by junam 2013.07.23 for ONDEMAND_PLAYLIST
                    }
                }

                emit setSelectedAlphaOnTop(indexQuickScroll);
            }
            break;
        }

        case eGetFolderTabList_Id:
        {
            if (m_volumeType != eJukeBox && m_volumeType != eUSB_FRONT && m_volumeType != eUSB_REAR)
            {
                QStringList m_listDiscTitleTemp = m_AudioController.getDiscInfoList();
                if(m_listDiscTitleTemp.isEmpty())
                {
                    int Count = m_AudioController.GetMediaNum();
                    for ( int i = 0; i < Count; i++)
                    {
                        //{ modified by cychoi 2013.10.19 for "Track" display by Language Setting
                        m_listMetaInfo.itemTitle << QString(QApplication::translate("main", "STR_MEDIA_TRACK") + " %1").arg(i + 1);
                        //m_listMetaInfo.itemTitle << "Track " + QString::number(i+1);
                        //} modified by cychoi 2013.10.19
                    }
                    updateCountInfoString("Track", Count);
                }
                else
                {
                    m_listMetaInfo.itemTitle << m_listDiscTitleTemp;
                    updateCountInfoString("Track", m_listDiscTitleTemp.count());
                }

                beginInsertRows(QModelIndex(), 0, m_listMetaInfo.itemTitle.count() - 1);
                m_nTotalItemCount = m_listMetaInfo.itemTitle.count();
                endInsertRows();

                setPlaybackIcon();
                emit setCurrentPlayingItemPosition();
            }
            else
            {
                int folderCount = 0, fileCount = 0;

                foreach (QStringList list, fileList)
                {
                    // modified by ravikanth 17-08-13 for hotfix, list not updated in usb audio list while copy
                    //if(((m_bCopyMode &&  list.at(1) == "false") || (!m_bCopyMode && (list.at(1) == "true") && (list.at(2) == "true"))))
                    if(((m_volumeType == eJukeBox) && m_bCopyMode) || ((list.at(1) == "true") && (list.at(2) == "true")))
                        continue;
                    
                    if (m_bCopyMode && m_fileOperationData.operation == LVEnums::OPERATION_MOVE &&
                        list.at(1) == "true" && !m_fileOperationData.itemNames.isEmpty() &&
                       (m_fileOperationData.category == FOLDER_DEPTH_0 || m_fileOperationData.category == FOLDER_DEPTH_1) &&
                        m_fileOperationData.itemNames.contains(list.at(0)))
                    {
                        MP_LOG << "Filter out" << list.at(0) << "folder cause it will be moved" << LOG_ENDL;
                        continue;
                    }

                    m_listMetaInfo.itemTitle << m_AudioController.encodeString(list.at(0).section('/', -1));
                    m_listMetaInfo.itemName  << list.at(0);
                    m_listMetaInfo.itemSource << list.at(0);
                    m_listMetaInfo.isFolder  << ( list.at(1) == "true" );
                    m_listMetaInfo.isItemCheckBoxMarked << false;

                    if (  list.at(1) == "true" )
                    {
                        m_listMetaInfo.imageSource << "/app/share/images/music/ico_tab_list_folder.png";
                        folderCount++;
                    }
                    else
                    {
                        m_listMetaInfo.imageSource << "";
                        fileCount++;
                    }
                }

                beginInsertRows(QModelIndex(), 0, m_listMetaInfo.itemTitle.count () - 1);
                m_nTotalItemCount = m_listMetaInfo.itemTitle.count ();
                endInsertRows ();

                setPlaybackIcon();
                emit setCurrentPlayingItemPosition();

                if ( folderCount )
                    updateCountInfoString("Folders", folderCount, fileCount);
                else
                    updateCountInfoString("Songs", fileCount );

                if ( FOLDER_DEPTH_0 == calcCategoryState() )
                {
                    emit showJBoxEmptyStr( folderCount== 0 && fileCount == 0 && m_displayMode == MP::JUKEBOX);
                }
            }
            break;
        }


        case eGetSongIndexFromVolumeQuery_Id:
        case eGetAlbumIndexFromVolumeQuery_Id:
        case eGetArtistIndexFromVolumeQuery_Id:
        {
            if( reqUID == REQUID_LIST_DATABASE_UPDATE )
            {
                m_currentPlayingItemPreparation = fileList[0][0].toInt();

                m_pListTrackerAbstractor->setQueryOffset(0);
                if(m_listMetaInfo.itemTitle.count()  < FETCHED_BLOCK_ITEM_COUNT)
                    m_pListTrackerAbstractor->setQueryLimit(FETCHED_BLOCK_ITEM_COUNT);
                else
                    m_pListTrackerAbstractor->setQueryLimit(m_listMetaInfo.itemTitle.count());

                if (queryId == eGetSongIndexFromVolumeQuery_Id )
                    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithLimitFromVolumeQuery_Id, reqUID);
                else if(queryId == eGetAlbumIndexFromVolumeQuery_Id)
                    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, reqUID);
                else if(queryId == eGetArtistIndexFromVolumeQuery_Id)
                    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsWithLimitFromVolumeQuery_Id, reqUID);
            }
            else
            {
                bool bRequest = false;

                int currentPlayItem = m_currentPlayingItem;
                int totalItemCount = m_nTotalItemCountPreparation;

                if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER)
                {
                    m_currentPlayingItemPreparation = fileList[0][0].toInt();
                    currentPlayItem = m_currentPlayingItemPreparation;
                    totalItemCount = m_nTotalItemCountPreparation;
                }
                else
                {
                    setCurrentPlayingItem(fileList[0][0].toInt()); //Apply directly.
                    //{added by junam 2013.07.02 for DQA
                    currentPlayItem = fileList[0][0].toInt();
                    totalItemCount = m_nTotalItemCount;
                    //}added by junam
                }

                //{added by junam 2013.07.15 for ISV_NA_87444
                if(reqUID == REQUID_LIST_TRACK_CHANGE && moving())
                {
                    MP_LOG<<"List is moving no need to change to play position" << LOG_ENDL;
                    setCurrentPlayingItem(currentPlayItem);
                    break;
                }
                //}added by junam

                if(totalItemCount > FETCHED_BLOCK_ITEM_COUNT)
                {
                    int iOffset=-1, iLimit=0;
                    bRequest = getEmptyBlock((currentPlayItem < 0) ? 0 : currentPlayItem, iOffset, iLimit);
                    if (bRequest)
                    {
                        m_pListTrackerAbstractor->setQueryOffset(iOffset);
                        m_pListTrackerAbstractor->setQueryLimit(iLimit);
                    }
                }
                else
                {
                    if (m_listMetaInfo.itemTitle.isEmpty())
                    {
                        m_pListTrackerAbstractor->setQueryOffset(0);
                        m_pListTrackerAbstractor->setQueryLimit(totalItemCount);
                        bRequest = true;
                    }
                }

                if (bRequest)
                {
                    if (queryId == eGetSongIndexFromVolumeQuery_Id )
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithLimitFromVolumeQuery_Id, reqUID);
                    else if(queryId == eGetAlbumIndexFromVolumeQuery_Id)
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, reqUID);
                    else if(queryId == eGetArtistIndexFromVolumeQuery_Id)
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsWithLimitFromVolumeQuery_Id, reqUID);
                }
                else if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER )
                {
                    // emit signalListLoadingIcon(false); // removed by kihyung 2013.05.24
                    setCurrentPlayingItem(m_currentPlayingItemPreparation);
                }
                if( m_AudioController.getCurrentAutoPlayItem() == -1 )
                {
                    emit setCurrentPlayingItemPosition(); // modified on 03-10-13 new spec change, don't update highlight when current play item changed while in list
                }
            }
            break;
        }
        // { added by sungha.choi 2013.07.23
        case eGetSongIndexWithTitleCountFromVolumeQuery_Id:
        case eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id:
        case eGetArtistIndexWithArtistCountFromVolumeQuery_Id:
        {
            if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER )
            {
                bool bRequest = false;

                if (!fileList.isEmpty() && !fileList[0].isEmpty())
                {
                    MP_LOG << "m_nTotalItemCount =" << fileList[0][0] << LOG_ENDL;
                    m_nTotalItemCountPreparation = fileList[0][0].toInt();
                }
                else
                {
                    m_nTotalItemCountPreparation = 0;
                }

                m_currentPlayingItemPreparation = fileList[0][1].toInt();
                int currentPlayItem = m_currentPlayingItemPreparation;
                int totalItemCount = m_nTotalItemCountPreparation;

                if(totalItemCount > FETCHED_BLOCK_ITEM_COUNT)
                {
                    int iOffset=-1, iLimit=0;
                    bRequest = getEmptyBlock((currentPlayItem < 0) ? 0 : currentPlayItem, iOffset, iLimit);
                    if (bRequest)
                    {
                        m_pListTrackerAbstractor->setQueryOffset(iOffset);
                        m_pListTrackerAbstractor->setQueryLimit(iLimit);
                    }
                }
                else
                {
                    if (m_listMetaInfo.itemTitle.isEmpty())
                    {
                        m_pListTrackerAbstractor->setQueryOffset(0);
                        m_pListTrackerAbstractor->setQueryLimit(totalItemCount);
                        bRequest = true;
                    }
                }

                if (bRequest)
                {
                    if (queryId == eGetSongIndexWithTitleCountFromVolumeQuery_Id )
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithLimitCacheFromVolumeQuery_Id, reqUID);
                    else if(queryId == eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id)
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithCoverArtWithLimitCacheFromVolumeQuery_Id, reqUID);
                    else if(queryId == eGetArtistIndexWithArtistCountFromVolumeQuery_Id)
                        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsWithLimitCacheFromVolumeQuery_Id, reqUID);
                }
                else
                {
                    setCurrentPlayingItem(m_currentPlayingItemPreparation);
                }
                emit setCurrentPlayingItemPosition();
            }
            break;
        }
        // } added by sungha.choi 2013.07.23
        default:
            break;
    }

    if(reqUID == REQUID_LIST_COPY_OPERATION) {    // added by sungha.choi 2013.09.22 for ITS 0189462
        emit contentRequestAllComplete();
    }
    m_currentRequestCount = 0;
}

int CListModel::setPlaybackIcon() // modified by Dmitry 31.07.13 for ITS0181701
{
    CATEGORY_STATE_T category = calcCategoryState();
    int idx = -1;
    MP_LOG << "category =" << category << LOG_ENDL;

    switch (category)
    {
        //{removed by junam 2013.10.23 for add aritst depth
        //case ALBUM_DEPTH_0:
        //{
        //    MediaInfo info;
        //    m_AudioController.GetCurrentMediaInfo(&info);
        //    if(!info.Album.isEmpty())
        //        idx = m_listMetaInfo.itemTitle.indexOf(info.Album);
        //    break;
        //}
        //}removed by bjunam
        case ARTIST_DEPTH_0:
        {
            MediaInfo info;
            m_AudioController.GetCurrentMediaInfo(&info);
            if(!info.Artist.isEmpty())
                idx = m_listMetaInfo.itemTitle.indexOf(info.Artist);
            break;
        }

        case SONG_DEPTH_0:
        case PLAYLIST_DEPTH_1:
        case ARTIST_DEPTH_2: //changed by junam 2013.10.23 for add aritst depth
        case ALBUM_DEPTH_1:
        case GENRE_DEPTH_1: 
        case FOLDER_DEPTH_0:
        case FOLDER_DEPTH_1:
        case ITUNES_DEPTH_1:
        case PODCAST_DEPTH_1:
        case AUDIOBOOK_DEPTH_1:
        case COMPOSER_DEPTH_2:
        {
            QString currentPlayingFile = m_AudioController.GetCurrentFilePath();
            MP_LOG << "currentPlayingFile =" << currentPlayingFile << LOG_ENDL;

            if (m_AudioController.GetMediaMode() == MP::DISC && m_AudioController.getDiscType() == MP::AUDIO_CD)
            {
                if( m_AudioController.isPlayFromMLT())
                    idx = m_AudioController.GetCurrentMltTrackIndex();
                else
                    idx = m_AudioController.GetCurrentTrackIndex();
            }
            else
            {
                idx = indexOfFile(m_AudioController.GetCurrentFilePath(), m_listMetaInfo.itemSource);
            }
            break;
        }

        case ALBUM_DEPTH_0:  //{changed by junam 2013.10.23 for add aritst depth
        case GENRE_DEPTH_2:
        case ITUNES_DEPTH_0:
        case PODCAST_DEPTH_0:
        case AUDIOBOOK_DEPTH_0:
        case COMPOSER_DEPTH_1:
        {
            MediaInfo info;
            m_AudioController.GetCurrentMediaInfo(&info);
            if(!info.Album.isEmpty())
                idx = m_listMetaInfo.itemName.indexOf(info.Album);
            break;
        }

        //{added by junam 2013.10.30
        case ARTIST_DEPTH_1:
        {
            MediaInfo info;
            m_AudioController.GetCurrentMediaInfo(&info);
            if(m_currentRequest.queryStr == info.Artist && !info.Album.isEmpty())
                idx = m_listMetaInfo.itemName.indexOf(info.Album);
            break;
        }
        //}added by junam

        case GENRE_DEPTH_0:
        {
            MediaInfo info;
            m_AudioController.GetCurrentMediaInfo(&info);
            if(!info.Genre.isEmpty())
                idx = m_listMetaInfo.itemName.indexOf(info.Genre);
            break;
        }
        
        case COMPOSER_DEPTH_0:
        {
            MediaInfo info;
            m_AudioController.GetCurrentMediaInfo(&info);
            if(!info.Composer.isEmpty())
                idx = m_listMetaInfo.itemName.indexOf(info.Composer);
            break;
        }
        
        default:
        {
            break;
        }
    }

    if( idx < rowCount() )
    {
        setCurrentPlayingItem( idx );
    }
    return idx; // modified by Dmitry 31.07.13 for ITS0181701
}

// { modified by kihyung 2013.09.13 for ISV 89552
void CListModel::setCurrPlayingItemPos(QString album, QString artist, QString filename)
{
    if( m_AudioController.isListView() == false)
        return;

    MP_LOG << "album:" << album << LOG_ENDL;
    MP_LOG << "artist:" << artist << LOG_ENDL;
    MP_LOG << "filename:" << filename << LOG_ENDL;
    
//    CATEGORY_STATE_T category = calcCategoryState();

//    switch (category)
//    {
//    case SONG_DEPTH_0:
//        if(filename.isEmpty() == true) return;
//        if(!filename.startsWith("file://"))
//            filename.prepend("file://");
//        m_pListTrackerAbstractor->RequestData( filename, m_volumeType, eGetSongIndexFromVolumeQuery_Id, REQUID_LIST_TRACK_CHANGE);
//        break;
//    case ALBUM_DEPTH_0:
//        if(album.isEmpty() == true) return;
//        setFocusChange(false);
//        m_pListTrackerAbstractor->RequestData( album, m_volumeType, eGetAlbumIndexFromVolumeQuery_Id, REQUID_LIST_TRACK_CHANGE);
//        break;
//    case ARTIST_DEPTH_0:
//        if(artist.isEmpty() == true) return;
//        setFocusChange(false);
//        m_pListTrackerAbstractor->RequestData( artist, m_volumeType, eGetArtistIndexFromVolumeQuery_Id, REQUID_LIST_TRACK_CHANGE);
//        break;
//    default:
        int idx = setPlaybackIcon(); // modified by Dmitry 31.07.13 for ITS0181701
        // modified by ravikanth 03-10-13 new spec change, don't update highlight when current play item changed while in list
        if( m_AudioController.getCurrentAutoPlayItem() == -1 )
        {
            //{added by junam 2013.07.15 for ISV_NA_87444
            if(moving())
            {
                MP_LOG<<"List is moving no need to change to play position" << LOG_ENDL;
                return;
            }
            //}added by junam
            if (idx != -1) emit setCurrentPlayingItemPosition(); // modified by Dmitry 31.07.13 for ITS0181701
        }
//        break;
//    }
}
// } modified by kihyung 2013.09.13 for ISV 89552

void CListModel::updateCountInfoString(QString category, int count, int filesCount)
{
    MP_LOG << "count =" << count << ", category" << category << ", filesCount = " << filesCount << LOG_ENDL;

    m_countInfo = count;
    m_categoryInfo = category;
    m_filesCount = filesCount;
    emit signalUpdateCountInfo();
    // Modified by jonghwan.cho@lge.com 2013.10.22. for ITS_EU_0197349
    //emit updateClusterIndex( m_currentPlayingItem + 1, m_nTotalItemCount);
}

void CListModel::setAudioPlaylistsData(QString playlistPath)
{
    MP_LOG << "playlistPath =" << playlistPath << LOG_ENDL;

    this->clear();
    if (playlistPath.isEmpty())
    {
        /* (History stack == 0) case.
         * List of created playlists should be displayed */
        QStringList pathList, nameList;
        if (playlistList(m_displayMode, pathList, nameList))
        {
     //       beginInsertRows(QModelIndex(), 0, pathList.count() - 1);// remove by junam 2012.07.16 for CR 10719
            m_listMetaInfo.itemSource = pathList;

            // { changed by junam 2012.12.17 for Play_list history re-entering
            //m_listMetaInfo.itemName = nameList;
            m_listMetaInfo.itemName = pathList;
            // } changed by junam

            for (int i = 0; i < m_listMetaInfo.itemSource.count(); i++)
            {
                m_listMetaInfo.itemTitle.append(m_AudioController.encodeString(nameList[i]));
            }
			// {add by junam 2012.07.16 for CR 10719
			MP_LOG << "ROWS START" << LOG_ENDL;
            beginInsertRows(QModelIndex(), 0, pathList.count() - 1);
            m_nTotalItemCount = pathList.count ();
			// }add by junam
            endInsertRows();
            MP_LOG << "ROWS END" << LOG_ENDL;
            // added by minho 20120808
            // { for CR12467 Playing icon is not displayed on current playing song in Playlist screen
            setPlaybackIcon();
            emit setCurrentPlayingItemPosition();
            // } added by minho
        }
        updateCountInfoString("Playlists", rowCount());
    }
    else
    {
        /* (History stack == 1) case.
         * Display list of tracks in selected playlist */
        QStringList srcList, trackList;
        if (playlistRead(playlistPath, srcList, trackList))
        {
            //beginInsertRows(QModelIndex(), 0, srcList.count() - 1); // remove by junam 2012.07.16 for CR 10719
            m_listMetaInfo.itemSource = srcList;
            m_listMetaInfo.itemName = trackList;

            QString rootPath = JUKEBOX_ROOT;
            if (m_displayMode == MP::USB1 || m_displayMode == MP::USB2)
            {
                rootPath = m_AudioController.getMountPath((MP::DEVICE_T)m_displayMode);

                if (rootPath.isEmpty() || !rootPath.startsWith(AudioUtils::GetUSBRoot()))
                    rootPath = AudioUtils::GetUSBRoot();
            }

            /* Need to fill other fields to display currently played song in playlist */
            for (int i = 0; i < m_listMetaInfo.itemSource.count(); i++)
            {
                m_listMetaInfo.itemTitle.append(m_AudioController.encodeString(trackList[i]));
                m_listMetaInfo.imageSource.append("");

                if (srcList.at(i).startsWith(rootPath))
                    m_listMetaInfo.isSelectable.append(QFile::exists(srcList.at(i)));
                else
                    m_listMetaInfo.isSelectable.append(false);
            }
            //setPlaybackIcon(); // deleted by minho 20120808 CR12467
			// {add by junam 2012.07.16 for CR 10719
			MP_LOG << "ROWS START" << LOG_ENDL;
            beginInsertRows(QModelIndex(), 0, srcList.count() - 1);
            m_nTotalItemCount = srcList.count ();
			// }add by junam
            endInsertRows();
            MP_LOG << "ROWS END" << LOG_ENDL;
            setPlaybackIcon(); // added by minho 20120808 CR12467
            emit setCurrentPlayingItemPosition();
        }
        m_playlistSource = playlistPath;
        updateCountInfoString("Songs", rowCount());
    }

    m_currentRequestCount = 0;
}

bool CListModel::isPlaylistsExist() const
{
    QString      rootPath(JUKEBOX_ROOT);
    QDirIterator fileIter(rootPath,
                         (QStringList() << "*.m3u" << "*.m3a"),
                          QDir::Files,
                          QDirIterator::Subdirectories);

    return fileIter.hasNext();
}

void CListModel::updatePopupList()
{
    MP_LOG << "playlists count =" << m_listMetaInfo.itemTitle.count() << LOG_ENDL;

    QStringList pathList, nameList;
    playlistList(MP::JUKEBOX, pathList, nameList);
    emit signalUpdatePopupList(nameList, pathList);
}

void CListModel::HandleError(int errorCode)
{
    MP_LOG << "errorCode =" << errorCode << LOG_ENDL;

    switch(errorCode)
    {
        case eMP_UNSUPPORTED_FORMAT_COPY_ERROR:
        {
            if( getPopType() != (int)LVEnums::POPUP_TYPE_CAPACITY_ERROR ) // modified by ravikanth 20.06.13 for ITS 0173704
            {
                // { modified by dongjin 2013.02.18 for ISV 72965
                //emit signalShowPopup(LVEnums::POPUP_TYPE_UNAVAILABLE_FILE, false, false);
            //    emit signalShowPopup(LVEnums::POPUP_TYPE_CORRUPT_FILE, false, false); // removed by Michael.Kim 2014.02.13. for ITS 224959
                // } modified by dongjin
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
                //m_pFileOperationsHelper->cancelOperation();
            //    cancelOperation(); // removed by Michael.Kim 2014.02.13. for ITS 224959
                //modified  by yongkyun.lee 2012.11.07 for Miner controll
            //    emit copyingPopupClosed(); // removed by Michael.Kim 2014.02.13. for ITS 224959
            }
            break;
        }

        case eMP_JUKEBOX_ERROR:
        {
            emit signalShowPopup(LVEnums::POPUP_TYPE_JUKEBOX_ERROR, false, false);
            //modified  by yongkyun.lee 2012.11.07 for Miner controll
            //m_pFileOperationsHelper->cancelOperation();
            cancelOperation();
            //modified  by yongkyun.lee 2012.11.07 for Miner controll
            emit copyingPopupClosed();
            break;
        }

        case eMP_CORRUPT_FILE_COPY_ERROR:
        {
            emit signalShowPopup(LVEnums::POPUP_TYPE_CORRUPT_FILE, false, false);
            //modified  by yongkyun.lee 2012.11.07 for Miner controll
            //m_pFileOperationsHelper->cancelOperation();
            cancelOperation();
            //modified  by yongkyun.lee 2012.11.07 for Miner controll
            emit copyingPopupClosed();
            break;
        }

        default:
            break;
    }
}

// { removed by junam 2013.10.29 for playlist population
//void CListModel::scrollToSelectAlpha(QString letter)
//{
//    //unused function

//    //
//    MP_HIGH << "!!!!!!!!!!!!!!!!!!!! UNUSED FUNCTION CALLED scrollToSelectAlpha"  << LOG_ENDL;
//    MP_LOG << "letter =" << letter << LOG_ENDL;
//    CATEGORY_STATE_T category = calcCategoryState();

//    if (category == SONG_DEPTH_0 || category == ALBUM_DEPTH_0 || category == ARTIST_DEPTH_0)
//    {
//        //{added by junam 2013.07.25 for ITS_EU_180117
//        if(letter == "#" && (m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2))
//        {
//            if(category == SONG_DEPTH_0)
//            {
//                MP_LOG<<"iPod sorting find only first number by requesting #" << LOG_ENDL;
//            }
//            else
//            {
//                MP_LOG<<"other list find any symbol include nubmer by requesting space" << LOG_ENDL;
//                letter = " ";
//            }
//        }
//        //}added by junam
//        //{added by jaehwan.lee 2013.10.15 , if usb and juke box search letter change '#' to '!' first char of specical chars.
//        else if (letter == "#" && (m_displayMode == MP::JUKEBOX || m_displayMode == MP::USB1 || m_displayMode == MP::USB2))
//        {
//            MP_LOG<<"letter change '#' to '!' first char of specical chars " << LOG_ENDL;
//            letter = "!";
//        }
//        //}

//        ETrackerAbstarctor_Query_Ids queryId =
//                (category == SONG_DEPTH_0)  ? eGetAllSongsWithTitleIndexFromVolumeQuery_Id :
//                (category == ALBUM_DEPTH_0) ? eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id
//                                            : eGetAllMusicArtistsIndexFromVolumeQuery_Id;
//        //{changed by junam 2013.07.25 for ITS_EU_180117
//        //m_pListTrackerAbstractor->RequestData(QString::fromLocal8Bit(letter.toLocal8Bit().data()),
//        //                                      m_volumeType, queryId);
//        m_pListTrackerAbstractor->RequestData(QString::fromLocal8Bit(letter.toLocal8Bit().data()),
//                                              m_volumeType, queryId, REQUID_LIST_QUICK_SCROLL);
//        //}chagned by junam
//    }
//    else
//    {
//        QString curChar = m_AudioController.encodeString(letter);
//        int first_str = curChar.data()[0].unicode();

//        if (first_str >= 0x3130 && first_str <= 0x318F)
//        {
//            // while (first_str <= 0x314E)
//			if(first_str <= 0x314E) // modified by eugene.seo 2013.04.09
//            {
//                for (int i = 0; i < rowCount(); i++)
//                {
//                    bool isCheck = true;
//                    // { changed by junam 2012.07.15 for CR 10719
//                    //QString strKeyCode = m_listMetaInfo.itemTitle.at (i);
//                    QString strKeyCode = getMetaInfo(i, m_listMetaInfo.itemTitle);

//                    if (!strKeyCode.isEmpty())
//                    {
//                    // } changed by junam
//                        int nKeyCode = strKeyCode.data()[0].unicode();
//                        int UniValue = 0;

//                        if (nKeyCode >= 0x3130 && nKeyCode <= 0x318F)
//                        {
//                            UniValue = nKeyCode - 0x3130;
//                        }
//                        else if (nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF)
//                        {
//                            UniValue = nKeyCode - 0xAC00;
//                        }
//                        else
//                        {
//                            isCheck = false;
//                        }

//                        if (isCheck)
//                        {
//                            int final_con = UniValue % 28;
//                            int initial_con = ( ( UniValue - final_con ) / 28 ) / 21;
//                            if(initial_con < 19) //added by edo.lee 2012.10.29 STATIC TEST 0134364
//                           	{
//	                            wchar_t initialChr = m_categoryKoreaTable[initial_con];
//	                            if (initialChr == first_str)
//	                            {
//	                                emit setSelectedAlphaOnTop(i);
//	                                return;
//	                            }
//                            }
//                        }
//                    }// added by junam 2012.07.15 for CR 10719
//                }
//            }
//        }
//        else
//        {
//            while (curChar.at(0).toAscii() <= 'Z')
//            {
//                for (int i = 0; i < rowCount(); i++)
//                {
//                    // { changed by junam 2012.07.15 for CR 10719
//                    QString itemTitle = getMetaInfo(i, m_listMetaInfo.itemTitle);

//                    if (itemTitle.startsWith(curChar))
//                    //if (m_listMetaInfo.itemTitle.at(i).startsWith(curChar))
//                    // } changed by junam
//                    {
//                        emit setSelectedAlphaOnTop(i);
//                        return;
//                    }
//                }
//                curChar = (curChar.at(0).toAscii() + 1);
//            }
//        }
//    }
//}
// } removed by junam

void CListModel::editPlayListsCatalog(int index, int operation, QString plName)
{
    MP_LOG << "index =" << index << ", operation =" << operation << ", plName =" << plName << LOG_ENDL;

    if (index <  0 || index >= m_listMetaInfo.itemSource.count())
    {
        MP_LOG << "Incorrect playList index" << LOG_ENDL;
        return;
    }

    if (m_currentRequest.depth == 0)
    {
        QUrl fileUrl(m_listMetaInfo.itemSource.at(index));

        switch ((LVEnums::OPERATION_TYPE_T)operation)
        {
            case LVEnums::OPERATION_DELETE:
                QFile::remove(fileUrl.path());
				emit signalShowPopup(LVEnums::POPUP_TYPE_DELETE_COMPLETE, false, false); // added by lssanh 2012.09.24 for sanity 272, 276
                break;

            case LVEnums::OPERATION_RENAME:
            {
                QFileInfo fileInfo(fileUrl.path());
                QString dst = fileInfo.dir().absolutePath();
                dst.append("/");
                dst.append(plName);
                dst.append(".");
                dst.append(fileInfo.suffix());
                QFile::rename(fileUrl.path(), dst);
                break;
            }

            default:
                break;
        }
        setAudioPlaylistsData();
    }
    else  if (m_currentRequest.depth == 1)
    {
        QStringList srcList, trackList;
        if (playlistRead(m_playlistSource , srcList, trackList))
        {
            int editIndex = srcList.indexOf( getMetaInfo( index, m_listMetaInfo.itemSource ) );// chage to use getInfo by junam 2012.07.15 for CR 10719
            if ((LVEnums::OPERATION_TYPE_T)operation == LVEnums::OPERATION_DELETE)
            {
                srcList.removeAt( editIndex );
                trackList.removeAt( editIndex );
            }
            else if ((LVEnums::OPERATION_TYPE_T)operation == LVEnums::OPERATION_RENAME)
            {
                trackList[ editIndex ] = plName;
            }
            else
            {
                MP_LOG << "[ERROR] Unknow operation =" << operation << LOG_ENDL;
                return;
            }
            playlistsWrite( m_playlistSource, srcList, trackList);
            setAudioPlaylistsData( m_playlistSource );
        }
    }
}

void CListModel::resetPartialFetchData()
{
    MP_LOG << LOG_ENDL;
    m_nCurrentY = 0;
    m_iTopItemIdx = -1;
    //m_iBottomItemIdx = -1; //removed by junam 2014.03.09 for ISV74498
    m_quickScrollIdx = -1; // added by eugeny.novikov 2012.08.24 for CR 9731
    m_iScrollQueryOffset = -1; // added by junam 2012.10.12 for CR14040
    m_iScrollQueryLimit = 0; //added by junam 2013.10.12 for scroll performance

    //{added by junam 2013.04.23 for disable crash
    setCurrentPlayingItem( -1 );
    emit clearSelection();
    //} added by juam

    // { added by junam 2012.07.15 for CR 10719
    if( m_nTotalItemCount && !m_currentRequestCount)  // modified by junggil 2012.09.17 for CR13549 Music application crashed by continous tapping on the song,album, Artist tab in list
    {
        MP_LOG << "ROWS START" << LOG_ENDL;
        beginRemoveRows(QModelIndex(), 0, m_nTotalItemCount - 1);
        m_nTotalItemCount = 0;
        endRemoveRows();
        MP_LOG << "ROWS END" << LOG_ENDL;
    }
    // } added by junam

    //m_bScrollRequestInProgress = false; // removed by junam 2012.09.21 for CR13485
    // deleted by junggil 2012.07.18 for CR11957 m_CurrentScrollItemIndex = -1; // added by junggil 2012.07.14 for CR10185
}


// { removed by junam 2013.10.29 for playlist population
//void CListModel::handleListScrollEvent(int idx)
//{
//    //{changed by junam 2013.08.08 for ONDEMAND_PLAYLIST
//    ETrackerAbstarctor_Query_Ids  queryId;
//    bool isRequestFromIPOD = false;

//    switch(m_currentCategoryState)
//    {
//    case PLAYLIST_DEPTH_0:
//        queryId = eGetlistofPlaylistsQuery_Id;
//        isRequestFromIPOD = true;
//        break;
//    case PLAYLIST_DEPTH_1:
//        queryId = eGetAllSongsbyPlaylistQuery_Id;
//        isRequestFromIPOD = true;
//        break;
//    default:
//        return;
//    }
//    //}changed by junam
    
//    m_quickScrollIdx = -1; //added by junam 2013.06.17 for ITS 0172947

//    if( m_nTotalItemCount <= FETCHED_BLOCK_ITEM_COUNT || m_iTopItemIdx == idx || idx < 0)
//        return;

//    m_iTopItemIdx = idx;

//    int iOffset = -1, iLimit = 0;

//    if (getEmptyBlock(m_iTopItemIdx, iOffset, iLimit))
//    {
//        //{changed by junam 2013.10.12 for scroll performance
//        //if(m_iScrollQueryOffset == iOffset)
//        //{
//        //    MP_LOG<<"Already query this offset" << LOG_ENDL;
//        //    return;
//        //}
//        if(m_iScrollQueryOffset >= 0 && iOffset >= m_iScrollQueryOffset && iOffset <  m_iScrollQueryOffset + m_iScrollQueryLimit )
//        {
//            if(getMetaInfo( idx, m_listMetaInfo.itemTitle).isEmpty())
//            {
//                emit flickingEnabled(false);
//            }
//            return;
//        }
//        //}changed by junam

//        m_iScrollQueryOffset = iOffset;
//        m_iScrollQueryLimit = iLimit; //added by junam 2013.10.12 for scroll performance

//        if(isRequestFromIPOD)
//        {
//            CIpodController* pController = GetIPodController();
//            if(pController)
//            {
//                pController->requestData( queryId, 0 , m_currentRequest.itemIndex, iOffset, iLimit);
//            }
//        }
//        else
//        {
//            m_pListTrackerAbstractor->setQueryOffset(iOffset);
//            m_pListTrackerAbstractor->setQueryLimit(iLimit);
//            m_pListTrackerAbstractor->RequestData(m_volumeType, queryId);
//        }
//    }
//    //}changed by junam
//}
//}removed by junam

CATEGORY_STATE_T
CListModel::calcCategoryState()
{
    CATEGORY_STATE_T retVal = UNDEFINED_STATE;

    if (m_currentRequest.tabId == "Play_list")
    {
        retVal = (m_currentRequest.depth == 0) ? PLAYLIST_DEPTH_0 :
                 (m_currentRequest.depth == 1) ? PLAYLIST_DEPTH_1 : UNDEFINED_STATE;
    }
    else if (m_currentRequest.tabId == "Artist")
    {
        retVal = (m_currentRequest.depth == 0) ? ARTIST_DEPTH_0 :
                 (m_currentRequest.depth == 1) ? ARTIST_DEPTH_1 :
                 (m_currentRequest.depth == 2) ? ARTIST_DEPTH_2 : UNDEFINED_STATE;
    }
    else if (m_currentRequest.tabId == "Song")
    {
        retVal = SONG_DEPTH_0;
    }
    else if (m_currentRequest.tabId == "Album")
    {
        retVal = (m_currentRequest.depth == 0) ? ALBUM_DEPTH_0 :
                 (m_currentRequest.depth == 1) ? ALBUM_DEPTH_1 : UNDEFINED_STATE;
    }
    else if (m_currentRequest.tabId == "Genre")
    {
        retVal = (m_currentRequest.depth == 0) ? GENRE_DEPTH_0 :
                 (m_currentRequest.depth == 1) ? GENRE_DEPTH_1 :
                 (m_currentRequest.depth == 2) ? GENRE_DEPTH_2 :
                 (m_currentRequest.depth == 3) ? GENRE_DEPTH_3 : UNDEFINED_STATE;
    }
    else if (m_currentRequest.tabId == "Folder")
    {
        retVal = (m_currentRequest.depth == 0) ? FOLDER_DEPTH_0 : FOLDER_DEPTH_1 ;
    }
// { Added by junam 2012.09.03 for CR13014
    else if(m_currentRequest.tabId == "itunes")
    {
        retVal = CATEGORY_STATE_T(ITUNES_DEPTH_0 + m_currentRequest.depth);
    }
    else if(m_currentRequest.tabId == "Podcast")
    {
        retVal = CATEGORY_STATE_T(PODCAST_DEPTH_0 + m_currentRequest.depth);
    }
    else if(m_currentRequest.tabId == "Audiobook")
    {
        retVal = CATEGORY_STATE_T(AUDIOBOOK_DEPTH_0 + m_currentRequest.depth);
    }
    else if(m_currentRequest.tabId == "Composer")
    {
        retVal = CATEGORY_STATE_T(COMPOSER_DEPTH_0 + m_currentRequest.depth);
    }
    else
    {
        MP_LOG<<"unhandled tab id : "<<m_currentRequest.tabId << LOG_ENDL;
    }
// } Added by junam
    return retVal;
}

void CListModel::partialRequestComplete(QVector<QStringList> fileList,
                                        ETrackerAbstarctor_Query_Ids queryId, int offset, uint reqUID)
{
    MP_HIGH << "queryStr =" << MAP_QUERY_IDS.key(queryId) << ", queryId =" << queryId << LOG_ENDL;
    MP_HIGH << "offset =" << offset  << " fileList.count() =" << fileList.count() << " reqUID = " << reqUID << LOG_ENDL;
//{changed by junam 2013.10.29 for playlist population
    switch(queryId)
    {
    case eGetAllSongsbyPlaylistQuery_Id:
    case eGetlistofPlaylistsQuery_Id:
    {
        if(queryId == eGetlistofPlaylistsQuery_Id && m_currentCategoryState != PLAYLIST_DEPTH_0)
        {
            MP_LOG<<"Now playlist 1st depth but unexpected query received" << LOG_ENDL;
            break;
        }
        if(queryId == eGetAllSongsbyPlaylistQuery_Id && m_currentCategoryState != PLAYLIST_DEPTH_1)
        {
            MP_LOG<<"Now playlist 2nd depth but unexpected query received" << LOG_ENDL;
            break;
        }
        if(reqUID != REQUID_LIST_DYNAIC_FETCH_ENTER)
        {
            MP_LOG<<"Invaild reqUID = "<<reqUID << LOG_ENDL;
            break;
        }
        int itemCount = 0;
        if( fileList.isEmpty() == false && fileList.at(0).isEmpty() == false)
        {
            itemCount = fileList.at(0).at(0).toInt();
            fileList.remove(0,1);
        }
        if(itemCount && offset == m_listMetaInfo.itemName.size())
        {
            beginInsertRows(QModelIndex(), offset, offset+fileList.size() - 1);
            foreach (QStringList list, fileList)
            {
                m_listMetaInfo.itemTitle.append(list.at(0));
                m_listMetaInfo.itemName.append(list.at(0));
            }
            m_nTotalItemCount = m_listMetaInfo.itemName.size();
            endInsertRows();

            if(m_nTotalItemCount < itemCount)
            {
                CIpodController* pController = GetIPodController();
                if(pController)
                {
                    pController->requestData( queryId, REQUID_LIST_DYNAIC_FETCH_ENTER ,
                                             m_currentRequest.itemIndex,m_nTotalItemCount, 20);
                }
            }
            else
            {
                MP_LOG<<"finish loop.." << LOG_ENDL;
            }
        }
        else
        {
            MP_LOG<< "itemCount = "<< itemCount << LOG_ENDL;
            MP_LOG<< "offset = "<<offset<<" size = "<<m_listMetaInfo.itemName.size() << LOG_ENDL;
        }

        if(offset == 0)
        {
            if(m_nTotalItemCount)
                emit setCurrentPlayingItemPosition();
            else
                emit emptyItemViewList(m_currentRequest.depth);//added by junam 2013.10.30
            updateCountInfoString("Playlists", itemCount); //added by junam 2013.10.30
        }
        m_currentRequestCount = 0;
        break;
    }
    default:
        break;
    }
//}changed by junam
}

void CListModel::ListRequestCompleteWithVolumeType(QVector<QStringList> fileList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID)
{
    MEDIA_SOURCE_T source = AudioUtils::DeviceType(volumeType);
    MP::DEVICE_T device = AudioUtils::DeviceType(source);

    MP_HIGH << device << fileList.count() << LOG_ENDL;

    switch (queryId)
    {   
    case eGetMusicLimitFileListDir_Id:
    {
        MP_LOG << "TASCAN -> eGetMusicLimitFileListDir_Id" << reqUID << LOG_ENDL;
        
        if(reqUID == GET_TA_REQUID(REQUID_LIST_ALL_FILES, m_displayMode)) 
        {
            QStringList songList;

            foreach (QStringList file, fileList)
            {
                for (int i = 0; i < file.count(); i++)
                {
                    // MP_LOG << file[i] << LOG_ENDL;
                    songList.append(file[i]);
                }
            }

            MP_LOG << "TASCAN -> eGetMusicLimitFileListDir_Id playlist size:" << songList.count() << LOG_ENDL;

            if (!songList.isEmpty())
            {
                int index = songList.indexOf(m_selectedFileFromList);
                if(index < 0) index = 0;
                m_AudioController.createPlaylistFromFolderTab(songList, index);
            }
        }
        break;
    }
    default:
        break;
    }
}

bool CListModel::getEmptyBlock(int idx, int &offset, int &limit) 
{
    if (idx < 0)
        return false;

    int nCurrent = idx / ITEMS_PER_SCREEN;
    offset = -1, limit = 0;

    for ( int nBlock = nCurrent - 1 ; nBlock <= nCurrent+1 ; nBlock++ )
    {
        if(nBlock < 0)
            continue;
        //{changed by junam 2013.10.12 for scroll performance
        //if ( (nBlock*ITEMS_PER_SCREEN) < m_iBlockOffset ||
        //        (nBlock*ITEMS_PER_SCREEN) >= m_iBlockOffset + m_listMetaInfo.itemTitle.count() ||
        //        m_listMetaInfo.itemTitle.at((nBlock*ITEMS_PER_SCREEN) - m_iBlockOffset).isEmpty() )
        if(getMetaInfo( nBlock*ITEMS_PER_SCREEN, m_listMetaInfo.itemTitle).isEmpty() ) //}changed by junam
        {
            if(offset < 0)
                offset = nBlock*ITEMS_PER_SCREEN;
            limit += ITEMS_PER_SCREEN;
        }
    }

    int count = (m_nTotalItemCountPreparation < 0) ? m_nTotalItemCount: m_nTotalItemCountPreparation;

    //{added by junam 2013.10.12 for scroll performance
    if(limit)
    {
        while(limit < MAX_LIMIT_BLOCK*ITEMS_PER_SCREEN)
        {
            if (getMetaInfo(offset + limit, m_listMetaInfo.itemTitle).isEmpty())
            {
                limit += ITEMS_PER_SCREEN;
            }
            else if(getMetaInfo(offset - ITEMS_PER_SCREEN, m_listMetaInfo.itemTitle).isEmpty())
            {
                offset = offset - ITEMS_PER_SCREEN;
                limit += ITEMS_PER_SCREEN;
            }
            else
            {
                break;
            }
        }
    }
    //}added by junam

    if ( limit + offset >= count)
        limit = count - offset;

    return (limit>0) ? true : false;
}
// } added by junam

void CListModel::addChunkToListModel(QVector<QStringList> &fileList)
{
    MP_LOG << LOG_ENDL;
    
    QStringList itemTitlesTmp;
    QStringList itemNamesTmp;
    QStringList itemSourcesTmp;
    QStringList imageSourcesTmp;
    QList<wchar_t> itemQuickIndexTemp; // added by jaehwan 2014.01.22
    QList<bool> checkBoxesTmp;

	// { added by junam 2012.9.6 for CR13467 - protection code.
    if (!fileList.count())
        return;
	// } added by junam

    foreach (QStringList list, fileList)
    {
        if (!(m_currentRequest.tabId == "Album" && itemNamesTmp.contains(list[0])))
        {
            list[0].remove(FilePath_Prefix);

            if (m_currentRequest.tabId == "Song" ||
               (m_currentRequest.tabId == "Artist" && m_currentRequest.depth == 2) ||
               (m_currentRequest.tabId == "Composer" && m_currentRequest.depth == 2))
            {
                itemTitlesTmp.append(m_AudioController.encodeString(list[1]));
                itemQuickIndexTemp.append(getIndexChar(list[1]));  // added by jaehwan 2014.01.22
                itemNamesTmp.append(list[1]);
                itemSourcesTmp.append(list[0]);
                imageSourcesTmp.append("");
            }
            // { added by eunkoo 2012.07.16 for 11199
            else if(m_currentRequest.tabId == "Play_list" && m_currentRequest.depth == 1)
            {
                itemTitlesTmp.append(m_AudioController.encodeString(list[0]));
                itemNamesTmp.append(list[0]);
                itemSourcesTmp.append(list[1]);
                imageSourcesTmp.append("");
            }
            // } added by eunkoo
            else
            {
                itemTitlesTmp.append(m_AudioController.encodeString(list[0]));
                itemQuickIndexTemp.append(getIndexChar(list[0])); // added by jaehwan 2014.01.22
                itemNamesTmp.append(list[0]);
                itemSourcesTmp.append("");

                QString str = (m_currentRequest.tabId != "Album") ? "" :
                              (list[1].contains("No CoverArt Image Found") ||
                              (list[1].count() && list[1].endsWith("- Error - No artist found")) ||
                              !isCoverFileSizeValid((QString)list[1]) ) ? // added by wspark 2012.09.19 for CR13633
                                   "/app/share/images/music/coverflow_default_n.png" : list[1];

                // { removed by junam 2013.10.28 for 2nd depth coverart
                //if ( m_currentRequest.tabId == "Album" && m_currentRequest.depth == 0)
                //{
                //    MP_LOG << "PathViewModel" << LOG_ENDL;
                //    PathViewModel *pPathView = m_AudioController.GetPathViewModel();
                //    if( pPathView && !str.startsWith("file://"))
                //    {
                //        int idxPath = pPathView->getAlbumIndex( list[0] );
                //        if(idxPath >= 0)
                //        {
                //            QString coverURI =pPathView->getAlbumCover(idxPath);
                //            if (coverURI.startsWith("file://"))
                //                str = coverURI;
                //        }
                //    }
                //}
                // } removed by junam
                imageSourcesTmp.append(str);
            }
            checkBoxesTmp.append(false);
        }
    }

    {
		// { changed by junam 2012.9.6 for CR13467
        //int beginIndex = m_iBlockOffset + m_listMetaInfo.itemTitle.size() - 1;
        int beginIndex = m_listMetaInfo.itemTitle.size();
		// } changed by junam
        m_listMetaInfo.itemTitle << itemTitlesTmp;
        // {   added by jaehwan 2014.01.22
        if (itemQuickIndexTemp.count() > 0 )
        {
            m_listMetaInfo.itemQuickIndex.clear();
            m_listMetaInfo.itemQuickIndex << itemQuickIndexTemp;
        }
        //} added by jaehwan
        m_listMetaInfo.itemName << itemNamesTmp;
        m_listMetaInfo.itemSource << itemSourcesTmp;
        m_listMetaInfo.imageSource << imageSourcesTmp;
        m_listMetaInfo.isItemCheckBoxMarked << checkBoxesTmp;

   		// { added by junam 2012.07.15 for CR 10719
        if(m_nTotalItemCount)
        {
            // { added by eugeny.novikov 2012.08.24 for CR 9731
            MP_LOG << "Emit dataChanged() from beginIndex =" << beginIndex << "to" << m_listMetaInfo.itemTitle.size() - 1 << LOG_ENDL;
            // } added by eugeny.novikov
            emit dataChanged(this->index(beginIndex), this->index(m_listMetaInfo.itemTitle.size() - 1));
        }
        else
        {
            MP_LOG << "ROWS START" << LOG_ENDL;
            beginInsertRows(QModelIndex(), 0, m_listMetaInfo.itemTitle.size() - 1);
            m_nTotalItemCount = m_listMetaInfo.itemTitle.size();
            endInsertRows();
            MP_LOG << "ROWS END" << LOG_ENDL;
        }

        setPlaybackIcon();

        emit setCurrentPlayingItemPosition();
    }
}
//{ added by jaehwan 2013.10.29 to improve qucikscroll performance
wchar_t CListModel::getIndexChar(QString title)
{
    //{ modified by jaehwan 2014.01.08 for pinyin sorting
    bool bIPodSort = ((m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2) && (m_currentCategoryState == SONG_DEPTH_0));
    //MP_HIGH << "bIPodSort = "<<bIPodSort << LOG_ENDL;
    bool bPinyinSort = (AppEngineBase::GetLanguage() == LANGUAGE_ZH_CMN &&
            !(m_volumeType == eIPod || m_volumeType == eIPOD_FRONT || m_volumeType == eIPOD_REAR  ));
    return CTrackerAbstractor::GetIndexChar(title, bIPodSort, bPinyinSort);
    // } modified by jaehwan.lee
}
//}

void CListModel::updateListData(QVector<QStringList> &fileList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID) //changed by junam 2013.10.24   
{
    QString categoryTitle;

    //{added by junam 2013.12.31
    if(reqUID == REQUID_LIST_DATABASE_UPDATE)
    {
        if(!checkUpdateResponse(queryId))
            return;
    }
    //}added by junam

    m_listMetaInfo.itemTitle.clear();
    m_listMetaInfo.itemName.clear();
    m_listMetaInfo.itemArtist.clear();
    m_listMetaInfo.itemSource.clear();
    m_listMetaInfo.imageSource.clear();
    m_listMetaInfo.itemQuickIndex.clear();

    switch(queryId)
    {
        case eGetAllSongsFromVolumeQuery_Id: //song
            {
                foreach (QStringList list, fileList)
                {
                    list[0].remove(FilePath_Prefix);
                    m_listMetaInfo.itemSource.append(list[0]);
                    m_listMetaInfo.itemTitle.append(list[1]);
                    m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[1]));
                    m_listMetaInfo.itemName.append(list[1]);
                }

                makeQuckIndexTable(); // added by jaehwan 2013.11.11 for quick index table;
                categoryTitle = "Songs";
             }
            break;
        case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
        case eGetAllAlbumByComposerQuery_Id: //added by junam 2013.10.28 for 2nd depth coverart
        {
            //{added by junam for coverart loading performance
            if(m_displayMode == MP::JUKEBOX || m_displayMode == MP::USB1 || m_displayMode == MP::USB2)
            {
                m_AudioController.GetPathViewModel()->checkMediaArt(fileList);
            }
            //}added by junam
            foreach (QStringList list, fileList)
            {
                m_listMetaInfo.itemTitle.append(list[0]);
                m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[0]));
                m_listMetaInfo.itemName.append(list[0]);
                m_listMetaInfo.itemArtist.append(list[1]);
                m_listMetaInfo.itemSource.append(list[2]);
                //m_listMetaInfo.imageSource.append(QString()); //removed by junam 2013.10.28 for 2nd depth coverart
            }
            makeQuckIndexTable(); // added by jaehwan 2013.11.11 for quick index table;
            categoryTitle = "Albums";
            break;
        }
        case eGetAllMusicArtistsFromVolumeQuery_Id:
            foreach (QStringList list, fileList)
            {
                m_listMetaInfo.itemTitle.append(list[0]);
                m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[0]));
                m_listMetaInfo.itemName.append(list[0]);
            }
            makeQuckIndexTable(); // added by jaehwan 2013.11.11 for quick index table;
            categoryTitle = "Artists";
            break;

        default:
            break;
    }

    m_listMetaInfo.isItemCheckBoxMarked.clear();
    for(int i = 0; i < fileList.size(); i++)
    {
        m_listMetaInfo.isItemCheckBoxMarked.append(false);
    }

    if(fileList.isEmpty())
    {
        MP_LOG<<"decrease update from "<<m_nTotalItemCount<<" to zero" << LOG_ENDL;
        return;
    }
    else if(m_nTotalItemCount == 0)
    {
        MP_LOG<<"increase update from zero to"<<fileList.count() << LOG_ENDL;
        beginInsertRows(QModelIndex(), 0, fileList.count() - 1);
        m_nTotalItemCount = fileList.count();
        endInsertRows();
    }
    else if(m_nTotalItemCount < fileList.count())
    {
        MP_LOG<<"increase update from "<< m_nTotalItemCount <<" to "<< fileList.count() << LOG_ENDL;
        emit dataChanged( this->index(0), this->index(m_nTotalItemCount-1));
        beginInsertRows(QModelIndex(), m_nTotalItemCount, fileList.count() - 1);
        m_nTotalItemCount = fileList.count();
        endInsertRows();
    }
    else if(m_nTotalItemCount > fileList.count())
    {
        MP_LOG<<"decrease update from "<< m_nTotalItemCount <<" to "<< fileList.count() << LOG_ENDL;
        emit dataChanged(this->index(0), this->index(fileList.count()-1));
        beginRemoveRows(QModelIndex(), fileList.count(), m_nTotalItemCount - 1);
        m_nTotalItemCount = fileList.count();
        endRemoveRows();
    }
    else
    {
        MP_LOG<<"no data changed" << LOG_ENDL;
        return;
    }

    int previousePlayingItem = getCurrentPlayingItem();  //added by junam 2013.10.24   

    setPlaybackIcon();
    updateCountInfoString(categoryTitle, rowCount() );
    //{added by junam 2013.10.24
    if( reqUID == REQUID_LIST_DYNAIC_FETCH_ENTER)
    {
        emit setCurrentPlayingItemPosition();
    }
    else if( reqUID == REQUID_LIST_DATABASE_UPDATE)
    {
        MP_LOG<<"change playing item : "<<previousePlayingItem<<" => "<<getCurrentPlayingItem() << LOG_ENDL;
        if( m_currentFocuseIndex < 0 || (previousePlayingItem == -1 && getCurrentPlayingItem() >= 0))
        {
            emit setCurrentPlayingItemPosition();
        }
    }
    //}added by junam
}

//{added by junam 2013.06.25 for ITS175216
void CListModel::replaceListData(QVector<QStringList> &fileList, ETrackerAbstarctor_Query_Ids queryId)
{
    MP_LOG<<"enter "<< fileList.size() << LOG_ENDL;

    if(m_nTotalItemCount ==  fileList.size())
    {
        MP_LOG<<"no data changed" << LOG_ENDL;
        return;
    }

    if(!checkUpdateResponse(queryId))
        return;

    m_listMetaInfo.imageSource.clear(); //added by junam 2013.11.13 for remove coverart image when updating
    m_listMetaInfo.itemSource.clear();
    m_listMetaInfo.itemTitle.clear();
    m_listMetaInfo.itemQuickIndex.clear();//added by jaehwan 2013.10.31 for quick scroll index.
    m_listMetaInfo.itemName.clear();
    m_listMetaInfo.isItemCheckBoxMarked.clear();

    foreach (QStringList list, fileList)
    {        
        //{added by junam 2013.12.31
        if(queryId == eGetAllSongsByArtistAlbumQuery_Id)
        {
            m_listMetaInfo.itemTitle.append(list.at(1));
            m_listMetaInfo.itemQuickIndex.append(getIndexChar(list.at(1)));
            m_listMetaInfo.itemName.append(list.at(1));
            m_listMetaInfo.itemSource.append(list[0].remove(FilePath_Prefix));
        }
        else //}added by junam
        {
            m_listMetaInfo.itemTitle.append(list[0]);
            m_listMetaInfo.itemQuickIndex.append(getIndexChar(list[0])); //added by jaehwan 2013.10.31 for quick scroll index.
            m_listMetaInfo.itemName.append(list[0]);
            if(list.size() >= 2)
            {
                list[1].remove(FilePath_Prefix);
                m_listMetaInfo.itemSource.append(list[1]);
            }
            else
            {
                m_listMetaInfo.itemSource.append("");
            }
        }
        m_listMetaInfo.isItemCheckBoxMarked.append(false);
    }
    makeQuckIndexTable(); // added by jaehwan 2013.11.11 for quick index table;

    if(fileList.isEmpty())
    {
        MP_LOG<<"decrease update from "<<m_nTotalItemCount<<" to zero" << LOG_ENDL;
        return;
    }
    else if(m_nTotalItemCount == 0)
    {
        MP_LOG<<"increase update from zero to"<<fileList.count() << LOG_ENDL;
        beginInsertRows(QModelIndex(), 0, fileList.count() - 1);
        m_nTotalItemCount = fileList.count();
        endInsertRows();
    }
    else if(m_nTotalItemCount < fileList.count())
    {
        MP_LOG<<"increase update from "<< m_nTotalItemCount <<" to "<< fileList.count() << LOG_ENDL;
        emit dataChanged( this->index(0), this->index(m_nTotalItemCount-1));
        beginInsertRows(QModelIndex(), m_nTotalItemCount, fileList.count() - 1);
        m_nTotalItemCount = fileList.count();
        endInsertRows();
    }
    else if(m_nTotalItemCount > fileList.count())
    {
        MP_LOG<<"decrease update from "<< m_nTotalItemCount <<" to "<< fileList.count() << LOG_ENDL;
        emit dataChanged(this->index(0), this->index(fileList.count()-1));
        beginRemoveRows(QModelIndex(), fileList.count(), m_nTotalItemCount - 1);
        m_nTotalItemCount = fileList.count();
        endRemoveRows();
    }
    else
    {
        MP_LOG<<"no data changed" << LOG_ENDL;
        return;
    }

    setPlaybackIcon();
}
//}added by junam

bool
CListModel::playlistRead(QString fileName, QStringList &srcList, QStringList &trackList )
{
    MP_LOG << "fileName =" << fileName << LOG_ENDL;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString line = in.readLine();
    QString path;
    QString track;
    int ret = line.compare(EXTM3U);
    if( ret == 0 || ret == 1)
    {
        while (!in.atEnd())
        {
            line = in.readLine();
            line = line.trimmed();
            if(line.isEmpty()||line.isNull())
                continue;

            if( line.startsWith("#EXTALB") || line.startsWith("#EXTART"))
                continue;
            else if(line.startsWith('#'))
            {
            	// { modified by eugene.seo 2012.01.05 for ITS 153712
                //QStringList list1 = line.split(':');
                QStringList list = line.split(":0,"); 		
                if(list.size() >= 2)
                {
                    QString line0 = list.at(0).trimmed();
                    if(line0.compare(EXTINF) == 0)
                    {

     					QString line1 = list.at(1).trimmed();
                        track = line1;						
						/*
						QStringList list2 = list1.at(1).split(',');
				        if(list2.size() >= 2)
		                {
                            track = list2.at(1);
			            }
					    */
                    }
                    else
                    {
                        MP_LOG << "Unhandled line =" << line << LOG_ENDL;
                    }
                }
            	// } modified by eugene.seo 2012.01.05 for ITS 153712

                line = in.readLine();
                line = line.trimmed();
                if(line.isEmpty()||line.isNull())
                    continue;
            }
            else
            {
                track = QString("");
            }

            path = line;
            srcList << path;
            trackList << track;
        }
    }
    file.close();
    return srcList.isEmpty() ? false : true;
}

bool
CListModel::playlistsWrite(QString fileName, QStringList &srcList, QStringList &trackList)
{
    MP_LOG << "fileName =" << fileName << ", srcList.size() =" << srcList.size()
           << ", trackList.size() =" << trackList.size() << LOG_ENDL;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out<<EXTM3U<<endl;

    for(int index = 0; index < srcList.size(); index++)
    {
        out<<EXTINF<<":0,"<<trackList.at(index)<<endl;
        out<<srcList.at(index)<<endl;
        out<<endl;
    }
    file.close();
    return srcList.isEmpty() ? false : true;
}

bool
CListModel::playlistList(int device, QStringList &pathList, QStringList &nameList)
{
    MP_LOG << "device =" << device << LOG_ENDL;

    QStringList rootPaths;
    if (device == MP::JUKEBOX)
    {
        rootPaths.append(JUKEBOX_ROOT);
    }
    else if (device == MP::USB1 || device == MP::USB2)
    {
        rootPaths.append(JUKEBOX_ROOT);
        QString mountPath = m_AudioController.getMountPath((MP::DEVICE_T)device);

        if (mountPath.isEmpty() || !mountPath.startsWith(AudioUtils::GetUSBRoot()))
            mountPath = AudioUtils::GetUSBRoot();

        rootPaths.append( mountPath );
    }
    else
    {
        MP_LOG << "[ERROR] Unspported device =" << device << LOG_ENDL;
        return false;
    }

    // { changed by junam for 2012.10.26 CR#14566
    foreach( QString path, rootPaths)
    {
        QDir dir(path);

        dir.setSorting( QDir::Name | QDir::IgnoreCase );
        dir.setFilter(QDir::Files | QDir::NoSymLinks);

        foreach(QFileInfo fileInfo, dir.entryInfoList((QStringList() << "*.m3u")))
        {
            pathList.append(fileInfo.absoluteFilePath());
            nameList.append(fileInfo.baseName());
        }

    }
    // } changed by junam

    return pathList.isEmpty() ? false : true;
}

bool
CListModel::playlistAdd(QString fileName, QStringList &srcAdd, QStringList &trackAdd)
{
    MP_LOG << "fileName =" << fileName << "srcAdd.size() =" << srcAdd.size()
           << "trackAdd.size()" << trackAdd.size() << LOG_ENDL;

    for (int i = 0; i < srcAdd.count(); i++)
    {
        srcAdd[i].remove(FilePath_Prefix);
    }

    QStringList srcList, trackList;

    if (playlistRead(fileName, srcList, trackList))
    {
        for (int index = 0; index < srcAdd.size(); index++)
        {
            if (!srcList.contains(srcAdd.at(index)))
            {
                srcList.append(srcAdd.at(index));
                trackList.append(trackAdd.at(index));
            }
        }
        return playlistsWrite(fileName, srcList, trackList);
    }
    else
    {
        return playlistsWrite(fileName, srcAdd, trackAdd);
    }
}

bool
CListModel::playlistRemove(QString fileName, QStringList &srcRem, QStringList &trackRem )
{
    MP_LOG << "fileName =" << fileName << ", srcRem.size() =" << srcRem.size()
           << ", trackRem.size()" << trackRem.size() << LOG_ENDL;

    QStringList srcList, trackList;

    if(playlistRead( fileName, srcList, trackList ))
    {
        for( int index = 0; index < srcRem.size(); index++ )
        {
            if(! srcList.contains( srcRem.at( index )))
            {
                int rmIndex =  srcList.indexOf( srcRem.at( index ) );
                srcList.removeAt( rmIndex );
                trackList.removeAt( rmIndex );
            }
        }
        return playlistsWrite( fileName, srcList, trackList);
    }
    return false;
}

void CListModel::onNewFolderBtnClicked()
{
    MP_LOG << LOG_ENDL;
    m_quickEdit.operation = LVEnums::OPERATION_CREATE_FOLDER;
    emit showKeyPad("");
}

// { modified by ravikanth 23-04-13
void CListModel::onOperationComplete()
{
    onOperationComplete(0,-1); // modified by ravikanth for ITS 0189096
    emit listItemsUpdated();
}
// } modified by ravikanth 23-04-13

void CListModel::onOperationComplete(int skipCount, int copyFilesCount) // modified by ravikanth 22-04-13
{
    MP_LOG << "operation =" << m_fileOperationData.operation << LOG_ENDL;
    m_skipFileCount = skipCount; // modified by ravikanth 22-04-13
    m_copyFileCount = ( copyFilesCount == -1 ) ? 0 : copyFilesCount; // modified by ravikanth 22-04-13

	m_fileOperationData.bCreatingUrlList = false; // added by eugene.seo 2013.04.05

    //added by ruindmby 2012.09.17 for CR11770
    ((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->ResyncGracenote(0); // 0 is jukebox
    //added by ruindmby 2012.09.17 for CR11770

   // setCopyInProgress(false);
    bool isCopyOperation = (FileOperationsHelper::operationCopy == m_pFileOperationsHelper->currentOperation()); // Modified for ITS 0198505
    bool isDeleteMode = (m_fileOperationData.operation == LVEnums::OPERATION_DELETE); // modified for ITS 0214991

    int popupType = ((m_fileOperationData.operation == LVEnums::OPERATION_COPY) || isCopyOperation) ?
                        LVEnums::POPUP_TYPE_COPY_COMPLETE :
                    (m_fileOperationData.operation == LVEnums::OPERATION_MOVE) ?
                        LVEnums::POPUP_TYPE_MOVE_COMPLETE :
                    (((m_fileOperationData.operation == LVEnums::OPERATION_DELETE) && (copyFilesCount == -1)) ||
                     m_fileOperationData.operation == LVEnums::OPERATION_CLEAR ||
                     m_quickEdit.operation == LVEnums::OPERATION_DELETE) ?
                        LVEnums::POPUP_TYPE_DELETE_COMPLETE : LVEnums::POPUP_TYPE_UNDEFINED;
    // { added by yungi 2013.2.20 for Music Copy Completed not match OSD
//    if(popupType == LVEnums::POPUP_TYPE_COPY_COMPLETE  ||
//       popupType == LVEnums::POPUP_TYPE_DELETE_COMPLETE )
//    {
//        MP_LOG << "yungi popupType"<<popupType << LOG_ENDL;
//         //m_pFileOperationsHelper->setDisplay(m_AudioController.GetEngineMain().GetNotifier()->GetOSDDisplay());
//        //m_pFileOperationsHelper->setDisplay(GetOSDDisplay()); remove by edo.lee 2013.03.22
//    }
    if(popupType == LVEnums::POPUP_TYPE_DELETE_COMPLETE )
    {
         deleteSelectedModelItems();
    }
    // } added by yungi 2013.2.20 for Music Copy Completed not match OSD

    if ( m_fileOperationData.operation == LVEnums::OPERATION_DELETE ||
         m_quickEdit.operation == LVEnums::OPERATION_DELETE ||
         m_fileOperationData.operation == LVEnums::OPERATION_MOVE )
    {
        updateActivePlaylist();
    }
    // { added by junam 2012.08.14 for 3PDMS 136415
    else if ((m_fileOperationData.operation == LVEnums::OPERATION_CLEAR)&&
             (m_AudioController.playerMode() == MP::JUKEBOX))
    {
        emit contentSize(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC, 0);
    }
    // } added by junam
    // { added by eugene 2012.11.29 for CR 16076
    else if( m_fileOperationData.operation == LVEnums::OPERATION_COPY)
    {
        if(m_AudioController.isListView()) { // added by wspark 2013.01.24 ISV 69587
    	    isCopyCompletedMode = true;
        }
		isCopyingMode = false;	 // added by eugene 2013.01.10 for ISV 69609
    }
	// } added by eugene 2012.11.29 for CR 16076

    if(!(isDeleteMode && (popupType != LVEnums::POPUP_TYPE_DELETE_COMPLETE))) // modified for ITS 0214991
        m_fileOperationData.operation = LVEnums::OPERATION_NONE;

    if (popupType != LVEnums::POPUP_TYPE_UNDEFINED && popupType != LVEnums::POPUP_TYPE_COPY_COMPLETE) // modified by eugene.seo 2013.06.08 
    {
        emit finishEditMode();
        emit hidePopup(true); // modified by eugene.seo 2013.04.10
        emit signalShowPopup(popupType, false, false);

        /* Jikebox content was changed, so update content size info */
        // m_AudioController.requestContentSize(MP::JUKEBOX); // deleted by wspark 2012.11.27 for ISV64438
    }
    if ( popupType == LVEnums::POPUP_TYPE_COPY_COMPLETE )
    {
        isCopyingMode = false; // modified for ITS 0216048
        // { modified by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
        // setDisplayMode(MP::JUKEBOX); // added by wspark 2012.08.04 for CR11532
        //if(m_AudioController.isListView())
        //{
            //setDisplayMode(MP::JUKEBOX); //deleted by yungi 2013.2.7 for UX Scenario 5. File Copy step reduction
        //}
        // } modified by wspark
        //m_AudioController.sendUpdateOSD(OSD_EVENT_COPY_COMPLETED);
        // { added by eugene.seo 2013.06.08

        // modified for ITS 0208724
        //emit finishEditMode();
        //emit hidePopup(true); // modified by eugene.seo 2013.04.10
		// modified by ravikanth 14.06.2013 for ITS 0174012
        //if(GetOSDDisplay() != DISPLAY_REAR) // added by eugene.seo 2013.06.08

        if((skipCount == 0) && (copyFilesCount==0))
        {
            MP_LOG << "Copy complete not successful" << LOG_ENDL;
            popupType = LVEnums::POPUP_TYPE_COPY_CANCELED;
            emit finishEditMode();
            emit hidePopup(true); // modified by eugene.seo 2013.04.10
        }
        if( MP::USB1 == ( MP::DEVICE_T ) m_AudioController.playerMode()
                ||  MP::USB2 == ( MP::DEVICE_T ) m_AudioController.playerMode() )
        {
            if(m_AudioController.isListView())
            {
                if(popupType == LVEnums::POPUP_TYPE_COPY_COMPLETE)
                {
                    QTimer::singleShot(100, this, SLOT(showCopyCompletePopup()));
                }
                else
                {
                    emit signalShowPopup(popupType, false, false);
                }
            }
        }
        // modified for ITS 0212233
        if((popupType == LVEnums::POPUP_TYPE_COPY_COMPLETE) && !m_AudioController.isListView()) // modified for ITS 0210831
        {
            m_AudioController.sendUpdateOSD(OSD_EVENT_COPY_COMPLETED);
        }
        // } added by eugene.seo 2013.06.08
    }
    else if ( popupType == LVEnums::POPUP_TYPE_MOVE_COMPLETE)
    {
        m_AudioController.sendUpdateOSD(OSD_EVENT_MOVE_COMPLETED);
    }
    // { added by wspark 2012.08.30 for CR13375
    else if ( popupType == LVEnums::POPUP_TYPE_DELETE_COMPLETE)
    {
        setOperationCompleteforTA(true); // added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
        m_AudioController.sendUpdateOSD(OSD_EVENT_DELETE_COMPLETED);
    }
    // } added by wspark 2012.08.30 for CR13375


    //added by yongkyun.lee 2012.11.07 for Miner controll
    MinerControler(false); // 0 is jukebox
    //added by yongkyun

    if(popupType != LVEnums::POPUP_TYPE_DELETE_COMPLETE && m_AudioController.isListView())
    {
		// modified to avoid list update in edit state on copy complete 
        if((MP::JUKEBOX == ( MP::DEVICE_T ) m_AudioController.playerMode()) &&
                !isDeleteMode)
        {
            emit restartTimerForListUpdate();
        }
        else
        {
            m_bUpdateListOnEditExit = true;
        }
    }
}

void CListModel::updateActivePlaylist()
{
    MP_LOG << LOG_ENDL;
    if (!actuallyDeletedFiles.isEmpty())
    {
        // { modified by kihyung 2013.08.14 for ITS 0183530 
        MP_LOG << "actuallyDeletedFiles.isEmpty() is false" << LOG_ENDL;
        /*
        MP::DEVICE_T device            = ( MP::DEVICE_T ) m_AudioController.playerMode();
        QList<QString> *playlist       = m_AudioController.GetDeviceHistory(device)->playList.list;
        MP::PLAYBACK_CONTENT_T content = m_AudioController.GetDeviceHistory(device)->ContentPlayed;
        if (!playlist->isEmpty())
        {
            QSet<QString> updatedPL  = (playlist->toSet()).subtract(actuallyDeletedFiles.toSet());
            // Removing Deleted tracks from Active Playlist
            if (updatedPL != playlist->toSet())
            {
                m_AudioController.createPlayListFromList(updatedPL.toList(), 0, content);
            }
        }
        */
        // } modified by kihyung 2013.08.14 for ITS 0183530 
        m_AudioController.GetPathViewModel()->clearModelData();
        actuallyDeletedFiles.clear();
    }
}

void CListModel::onProgress(int size, int totalFiles, int percent, QString fileDeleted)
{
    MP_LOG << "File =" << fileDeleted << LOG_ENDL;
    if (fileDeleted != "")
    {
        actuallyDeletedFiles.append(fileDeleted);
    }
    emit progressToPopup(size, totalFiles, percent);
}

void CListModel::onItemLongPress(int itemIdx)
{
    MP_LOG << "itemIdx =" << itemIdx << LOG_ENDL;

    if (m_displayMode != MP::JUKEBOX ||
        itemIdx < 0 || itemIdx >= rowCount())
    {
        MP_LOG << "Quick edit is applicable for Jukebox only!" << LOG_ENDL;
        return;
    }

    resetQuickEditData();
    m_quickEdit.category = calcCategoryState();
    MP_LOG << "category =" << m_quickEdit.category << LOG_ENDL;

    if (m_currentRequest.tabId == "Folder" ||
        m_quickEdit.category == PLAYLIST_DEPTH_0 || m_quickEdit.category == PLAYLIST_DEPTH_1)
    {
        m_quickEdit.index = itemIdx;
        m_quickEdit.itemURL = getMetaInfo( itemIdx, m_listMetaInfo.itemSource);// chage to use getMetaInfo by junam 2012.07.15 for CR 10719

        QString popupTitle = getMetaInfo( itemIdx, m_listMetaInfo.itemTitle);// chage to use getMetaInfo by junam 2012.07.15 for CR 10719

        if (m_currentRequest.tabId == "Folder" && ! getMetaInfo(itemIdx, m_listMetaInfo.isFolder))// chage to use getMetaInfo by junam 2012.07.15 for CR 10719
        {
            popupTitle = popupTitle.left(popupTitle.lastIndexOf("."));
        }

        m_quickEdit.itemTitle = popupTitle;
        MP_LOG << "popupTitle =" << popupTitle << LOG_ENDL;
		// modified by ravikanth 25-07-13 for copy spec changes
        //emit signalShowPopup((int)LVEnums::POPUP_TYPE_RENAME, popupTitle, itemIdx);
        onQuickEdit(1); // delete current file
    }
}

void CListModel::onQuickEdit(int index)
{
    MP_LOG << "Clicked index =" << index << LOG_ENDL;

    if (index == 0) // rename
    {
        if (isItemUsed())
        {
            int popup = getMetaInfo( m_quickEdit.index, m_listMetaInfo.isFolder) ? // chage to use getMetaInfo by junam 2012.07.15 for CR 10719
                            LVEnums::POPUP_TYPE_FOLDER_IS_USED : LVEnums::POPUP_TYPE_FILE_IS_USED;

            emit signalShowPopup(popup, false, false);
            return;
        }

        m_quickEdit.operation = LVEnums::OPERATION_RENAME;
        emit showKeyPad(m_quickEdit.itemTitle);
    }
    else if (index == 1) // delete
    {
        m_quickEdit.operation = LVEnums::OPERATION_DELETE;
        m_fileOperationData.operation = LVEnums::OPERATION_DELETE;

        int popupType = (m_quickEdit.category == PLAYLIST_DEPTH_0) ? LVEnums::POPUP_QUICK_PLAYLIST_DELETE :
                        (m_quickEdit.category == PLAYLIST_DEPTH_1) ? LVEnums::POPUP_QUICK_PLAYLIST_FILE_DELETE
                                                                   : LVEnums::POPUP_TYPE_UNDEFINED;

        if (m_currentRequest.tabId == "Folder")
        {
            popupType = QFileInfo(m_quickEdit.itemURL).isDir() ? LVEnums::POPUP_QUICK_FOLDER_DELETE
                                                               : LVEnums::POPUP_QUICK_FILE_DELETE;
        }

        if (popupType != LVEnums::POPUP_TYPE_UNDEFINED)
        {
            emit signalShowPopup(popupType, false, false);
        }
    }
}

bool CListModel::isItemUsed()
{
    if ((m_quickEdit.category != FOLDER_DEPTH_0 && m_quickEdit.category != FOLDER_DEPTH_1) ||
         m_quickEdit.index >= m_listMetaInfo.isFolder.count())
    {
        return false;
    }

    QString curPlayingPath = m_AudioController.GetCurrentFilePath();
    MP_LOG << "curPlayingPath =" << curPlayingPath << LOG_ENDL;
    MP_LOG << "m_quickEdit.itemURL =" << m_quickEdit.itemURL << LOG_ENDL;

    if (!curPlayingPath.isEmpty())
    {
        return getMetaInfo( m_quickEdit.index, m_listMetaInfo.isFolder) ? // chage to use getMetaInfo by junam 2012.07.15 for CR 10719
                   curPlayingPath.contains(m_quickEdit.itemURL + "/") :
                   curPlayingPath == m_quickEdit.itemURL;
    }
    return false;
}

void CListModel::handleTextEntered(QString enteredStr, QString curPath)
{
    MP_LOG << "Current operation" << m_quickEdit.operation << LOG_ENDL;
    MP_LOG << "curPath =" << curPath << LOG_ENDL;

    if (enteredStr.isEmpty())
        return;

    if (m_fileOperationData.operation == LVEnums::OPERATION_ADD_TO_PLAYLIST)
    {
        QString plPath = enteredStr;
        plPath.prepend(JUKEBOX_ROOT + "/");
        plPath.append(".m3u");

        startFileOperation("", plPath);
    }
    else if (m_quickEdit.operation == LVEnums::OPERATION_RENAME)
    {
        invokeRenameOperation(enteredStr);
    }
    else if (m_quickEdit.operation == LVEnums::OPERATION_CREATE_FOLDER)
    {
        if (curPath.isEmpty())
            curPath = JUKEBOX_ROOT;

        if (QFileInfo(QDir(curPath), enteredStr).exists())
        {
            ErrorMessage(LVEnums::POPUP_FOLDER_ALREADY_EXIST);
            emit resetEnteredStr(); // added by junggil 2012.10.05 for SANITY_CM_AJ196 ~ 201
        }
        else
        {
            curPath += "/" + enteredStr;
            MP_LOG << "Creating folder" << curPath << LOG_ENDL;
            m_pFileOperationsHelper->createFolder(curPath);
            addNewFolderToModel(curPath);
            emit hideKeyPad();
        }
    }
}

void CListModel::invokeRenameOperation(QString enteredStr)
{
    MP_LOG << "enteredStr =" << enteredStr << LOG_ENDL;

    switch (m_quickEdit.category)
    {
        case PLAYLIST_DEPTH_0:
        case PLAYLIST_DEPTH_1:
            editPlayListsCatalog(m_quickEdit.index, LVEnums::OPERATION_RENAME, enteredStr);
            emit hideKeyPad();
            break;

        case FOLDER_DEPTH_0:
        case FOLDER_DEPTH_1:
        {
            MP_LOG << "m_quickEdit.itemURL =" << m_quickEdit.itemURL << LOG_ENDL;
            if (m_quickEdit.itemTitle == enteredStr)
            {
                /* User is able to discard renaming by keeping current name */
                emit hideKeyPad();
                return;
            }

            QFileInfo fileInfo(m_quickEdit.itemURL);
            QString currentPath = fileInfo.absolutePath();
            MP_LOG << "currentPath =" << currentPath << LOG_ENDL;

            QString newName = enteredStr;
            if (fileInfo.isFile())
            {
                newName.append(".");
                newName.append(fileInfo.suffix());
            }
            MP_LOG << "newName =" << newName << LOG_ENDL;

            QFileInfo newFileInfo(QDir(currentPath), newName);
            // modified by minho 20121211 for [3PDMS][148377] Changed file name isn't applied in play info
            // if (QFile::rename(fileInfo.absoluteFilePath(),
            //                   newFileInfo.absoluteFilePath()))
            if ((QFile::rename(fileInfo.absoluteFilePath(), newFileInfo.absoluteFilePath()))
                 || (((QFileInfo(QDir(currentPath), enteredStr).exists())) == false))
            // modified by minho
            {
                emit hideKeyPad();

                /* Update necessary List model data for proper displaying renamed item */
                if (m_quickEdit.index < rowCount())
                {
                    setMetaInfo( m_quickEdit.index, m_listMetaInfo.itemSource, newFileInfo.absoluteFilePath()); // chage to use setMetaInfo by junam 2012.07.15 for CR 10719
                    setMetaInfo( m_quickEdit.index, m_listMetaInfo.itemName, newFileInfo.absoluteFilePath()); // chage to use setMetaInfo by junam 2012.07.15 for CR 10719
                }
                setData(this->index(m_quickEdit.index), QVariant::fromValue(newName), titleRole);
                MP_LOG << "Item successfully renamed to" << getMetaInfo( m_quickEdit.index, m_listMetaInfo.itemName) << LOG_ENDL;// chage to use getMetaInfo by junam 2012.07.15 for CR 10719
            }
            else
            {
                fileInfo.isDir() ? ErrorMessage(LVEnums::POPUP_FOLDER_ALREADY_EXIST)
                                 : ErrorMessage(LVEnums::POPUP_FILE_ALREADY_EXIST);
            }
            break; // added by minho 20121211 for [3PDMS][148377] Changed file name isn't applied in play info
        }

        default:
            break;
    }
}

void CListModel::invokeQuickDelete()
{
    MP_LOG << m_quickEdit.itemURL << LOG_ENDL;

    if (m_quickEdit.itemURL.isEmpty() || m_quickEdit.operation != LVEnums::OPERATION_DELETE)
        return;

    QStringList itemPathList; // File list for controller only
    if (QFileInfo(m_quickEdit.itemURL).isDir())
    {
        // { modified by ruinseve for CR#12584
        QDirIterator dirIter(m_quickEdit.itemURL,
                             m_AudioController.GetControllers().getExtList(MEDIA_TYPE_MUSIC),
                             QDir::Files, QDirIterator::Subdirectories);
        // } modified by ruinseve for CR#12584.
        while (dirIter.hasNext())
        {
            dirIter.next();
            itemPathList << dirIter.fileInfo().absoluteFilePath();
        }
    }
    else
    {
        itemPathList << m_quickEdit.itemURL;
    }
    MP_LOG << "itemPathList.count()" << itemPathList.count() << LOG_ENDL;

    emit signalShowPopup(LVEnums::POPUP_TYPE_DELETING, false, false);
    m_pFileOperationsHelper->deleteFiles(QStringList(m_quickEdit.itemURL), AUDIO_FILE); // modified by ravikanth 02-09-13
}

QString
CListModel::getUrl(int index)
{
    if (index >= 0 && index < rowCount())
        return getMetaInfo( index, m_listMetaInfo.itemSource);// chage to use getMetaInfo by junam 2012.07.15 for CR 10719
    else
        return "";
}

// added by ruindmby 2012.08.27 for CR 12133
bool
CListModel::isSelectable(int index)
{
   return data(this->index(index), selectableRole).toBool();
}
// added by ruindmby 2012.08.27 for CR 12133

// { modified by lssanh 2012.09.12 for CR13482
void CListModel::clearJukebox()
{
    MP_LOG << LOG_ENDL;
    // { added by eugene.seo 2013.05.19
	bool isCopying = ((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->isCopyInProgress();
	if(isCopying)
	{
		((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->CancelCopyJukebox();
		((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->CancelAudioCopy();
	}
	// } added by eugene.seo 2013.05.19
/*
    emit signalShowPopup(LVEnums::POPUP_TYPE_DELETING, true, false);
    setOperation(LVEnums::OPERATION_CLEAR);
*/
    emit signalShowPopup(LVEnums::POPUP_TYPE_FORMATING, false, false);

    m_pFormatTimer = new QTimer(this);
    m_pFormatTimer->setSingleShot(true);
    m_pFormatTimer->setInterval(2000);
    m_pFormatTimer->start();
    connect(m_pFormatTimer, SIGNAL(timeout()), SLOT(startJukeBoxFormat()));
}
// } modified by lssanh 2012.09.12 for CR13482

//HWS
QString CListModel::getListStatus(){
    QString countInfoString="";
    countInfoString = m_categoryInfo;

return countInfoString;
}
//HWS
QString CListModel::getListFolders(){
    QString countInfoString="";

    if( calcCategoryState() == FOLDER_DEPTH_0 || ( m_countInfo > 0 && m_filesCount > 0 ) )
        {
            if( m_filesCount > 0 )
                countInfoString = "comparefolder"; //+ QString(" ") + QString("Files") + QString("/") + QString::number(m_countInfo) + QString(" ") + m_categoryInfo;
        }

return countInfoString;
}
//HWS
QString CListModel::getCompareFolders(int currentIndex, bool isSongList){
    QString countInfoString="";

    m_currentIndex = currentIndex;
    if(isSongList)
    {
        if( m_filesCount == -1 ||
                (currentIndex >= 0 &&
                 m_listMetaInfo.isFolder.count() > currentIndex &&
                 getMetaInfo( currentIndex, m_listMetaInfo.isFolder)))// chage to use getMetaInfo by junam 2012.07.15 for CR 10719
        {
            countInfoString="icofoler";

        }
    }

return countInfoString;
}
//}modified by HWS 2013.03.24 for New UX


QString CListModel::getCountInfo(int currentIndex, bool isSongList)
{
    QString countInfoString;
    MP_LOG << "calcCategoryState(): " << calcCategoryState() << LOG_ENDL;
    MP_LOG << "currentIndex: " << currentIndex << LOG_ENDL;
    MP_LOG << "isSongList: " << isSongList << LOG_ENDL;

    m_currentIndex = currentIndex; // added by eugene.seo 2013.01.07 for scroll issue

    if(isSongList)
    {
    // } modified by lssanh 2013.02.15 list query change

        if( m_filesCount == -1 ||
                (currentIndex >= 0 &&
                 m_listMetaInfo.isFolder.count() > currentIndex &&
                 getMetaInfo( currentIndex, m_listMetaInfo.isFolder)))// chage to use getMetaInfo by junam 2012.07.15 for CR 10719
        {
            //{added by junam 2013.07.16 for playlist count blinking
            if(m_countInfo == 0)
            {
                countInfoString ="";
            }
            else //}added by junam
            {
                if(currentIndex+1 <= 0) //add by youngsim 2013.10.21 for 0/123  //added by junam 2013.10.20 for counter blinking
                    countInfoString =  "";
                else if(m_AudioController.GetEngineMain().middleEast()) //modifed by nhj 2013.10.07 for middleeast
                    countInfoString = QString::number(m_countInfo) + QString("/") + QString::number(currentIndex + 1);
                else
                    countInfoString = QString::number(currentIndex + 1) + QString("/") + QString::number(m_countInfo);
            }
        }
        else
        {
            if(currentIndex + 1 - m_countInfo <=0) //add by youngsim 2013.10.21 for 0/123 //added by junam 2013.10.20 for counter blinking
                countInfoString =  "";
            else if(m_AudioController.GetEngineMain().middleEast()) //modifed by nhj 2013.10.07 for middleeast
                countInfoString = QString::number(m_filesCount) + QString("/") + QString::number(currentIndex + 1 - m_countInfo);
            else
                countInfoString = QString::number(currentIndex + 1 - m_countInfo) + QString("/") + QString::number(m_filesCount);
                //m_isFoldersCount = false;	//added by aettie.ji 2012.12.20 for new ux
        }
    }
    else
    {
        //{added by junam 2014.01.10 for list counter icon
        if(m_filesCount > 0 && m_countInfo > 0)
            countInfoString = QString::number(m_filesCount) ;
        else //}added by junam
            countInfoString = QString::number(m_countInfo) ;
    }
    return countInfoString;
}
//HWS
QString CListModel::getCountInfoFirst(int currentIndex, bool isSongList)
{
    QString countInfoString="";
    MP_LOG << "calcCategoryState(): " << calcCategoryState() << LOG_ENDL;
    MP_LOG << "currentIndex: " << currentIndex << LOG_ENDL;
    MP_LOG << "isSongList: " << isSongList << LOG_ENDL;

    m_currentIndex = currentIndex;

    if(isSongList)
    {
        if( m_filesCount == -1 ||
                (currentIndex >= 0 &&
                 m_listMetaInfo.isFolder.count() > currentIndex &&
                 getMetaInfo( currentIndex, m_listMetaInfo.isFolder)))
        {
            //countInfoString = QString::number(currentIndex + 1 + m_iBlockOffset) + QString("/") + QString::number(m_countInfo); // modified by junggil 2012.07.14 for CR10185
            countInfoString = "";            
        }
        else
        {
            countInfoString = "";//QString::number(currentIndex + 1 - m_countInfo) + QString("/b") + QString::number(m_filesCount);
        }
    }
    else if( calcCategoryState() == FOLDER_DEPTH_0 || ( m_countInfo > 0 && m_filesCount > 0 ) )
    {
        //{ changed by junam 2014.01.10 for list counter icon
        //if( m_filesCount > 0 )
        //    countInfoString =  QString::number(m_filesCount);
        if( m_filesCount > 0 && m_countInfo > 0)
            countInfoString =  QString::number(m_countInfo); //}changed by junam
        else
            countInfoString =  "" ;
    }
    else
    {
        countInfoString =  "";
    }
    return countInfoString;
}

// { added by sungkim for CR9616
void CListModel::clearPopup()
{
    emit hidePopup(true); // modified by eugene.seo 2013.04.10
}

void CListModel::showPopupByExt(int type, QVariant extParam, QVariant extParam2)
{
     emit signalShowPopup(type, extParam, extParam2);
}
// } added by sungkim for CR9616

// { modified by Michael.Kim 2014.02.05 for ITS 223011
// { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot
// { modified by lssanh 2012.09.12 for CR13482
void CListModel::startJukeBoxFormat()
{
    MP_LOG << LOG_ENDL;

    if (GetUSBController())
    {
        QString volumeName = QUrl("/usr_data").path();
        //GetUSBController()->StartDiscFormat(volumeName); // modified by eugeny.novikov 2012.11.20 for CR 15408
        USBController *ptr = m_AudioController.GetControllers().GetUSBController();

        connect(ptr, SIGNAL(OnError(int)), this, SLOT(HandleError(int)), Qt::UniqueConnection);
        connect(ptr, SIGNAL(OnFormatFinished(int)), this, SLOT(FormattingComleteHandler(int)), Qt::UniqueConnection);

        ptr->StartDiscFormat(volumeName);
    }
}
// } modified by eugeny.novikov 2012.12.10

void CListModel::FormattingComleteHandler(int status)
{
    MP_LOG << "status" << status << LOG_ENDL; // modified by eugeny.novikov 2012.11.20 for CR 15408
    USBController *ptr = m_AudioController.GetControllers().GetUSBController();
    disconnect(ptr, SIGNAL(OnError(int)), this, SLOT(HandleError(int)));
    disconnect(ptr, SIGNAL(OnFormatFinished(int)), this, SLOT(FormattingComleteHandler(int)));

    emit hidePopup(true); // modified by eugene.seo 2013.04.10
    emit signalShowPopup(LVEnums::POPUP_TYPE_FORMAT_COMPLETE, false, false);
    setOperationCompleteforTA(true); // added by wspark 2012.11.06 for showing no files popup after deleting all jukebox audio.
    emit contentSize(MEDIA_SOURCE_JUKEBOX, MEDIA_TYPE_MUSIC, 0);// added by eunhye 2012.11.06 for Empty message is not display after formating the jukebox.
}
// } modified by lssanh 2012.09.12 for CR13482
// } modified by Michael.Kim 2014.02.05 for ITS 223011

void CListModel::onFileSystemChanged()
{
    MP_LOG << LOG_ENDL;

    m_pListTrackerAbstractor->clearCategoryListCache(eJukeBox);  // added by sungha.choi 2013.08.21 for ITS 0184905
    // removed by ravikanth 28-08-13 for ITS 0186400, 0185495 error handling in helper class
    m_AudioController.GetEngineMain().requestTrackerAbsRescan(eJukeBox);
    //Added by Puneet for not showing loading Popup on front video/Audio while copying photo in rear. 2013.06.02
     ((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->setRescanProgressLoading();
}
// added by edo.lee 2013.03.22
void CListModel::onOperationDisplay(DISPLAY_T disp)
{
	MP_LOG << LOG_ENDL;
	m_pFileOperationsHelper->setDisplay(disp);
	QTranslator& translator = m_AudioController.GetEngineMain().GetTranslator(); //added by edo.lee 2013.06.22
	m_pFileOperationsHelper->setTranslator(&translator); //added by edo.lee 2013.06.22
}
// added by edo.lee 2013.03.22

void CListModel::copyPlayingSongToJukebox()
{
    MP_LOG << LOG_ENDL;

    m_fileOperationData.fileURLs.clear();
    m_fileOperationData.operation = LVEnums::OPERATION_COPY;
    m_fileOperationData.destPath = JUKEBOX_ROOT;

    QString filePath = m_AudioController.GetCurrentFilePath();

    if (filePath.isEmpty() || !QFile::exists(filePath))
    {
        MP_LOG << "Invalid file path to copy" << LOG_ENDL;
        m_fileOperationData.operation = LVEnums::OPERATION_NONE;
        return;
    }
    setCopyAll(false); // added by wspark 2012.11.27 for ISV64438
    m_fileOperationData.fileURLs << filePath;
    invokePendingFileOperation();
}

void CListModel::addPlayingTrackToPlaylist(QString playlistStr)
{
    MP_LOG << "playlistStr =" << playlistStr << LOG_ENDL;

    m_fileOperationData.fileURLs.clear();
    m_fileOperationData.trackTitles.clear();
    m_fileOperationData.operation = LVEnums::OPERATION_ADD_TO_PLAYLIST;

    if (playlistStr.startsWith(JUKEBOX_ROOT) == false)
    {
        playlistStr.prepend(JUKEBOX_ROOT + "/");
        playlistStr.append(".m3u");
        MP_LOG << "Changed playlistStr =" << playlistStr << LOG_ENDL;
    }

    m_fileOperationData.plPath = playlistStr;
    m_fileOperationData.fileURLs << m_AudioController.GetCurrentFilePath();
    m_fileOperationData.trackTitles << m_AudioController.GetCurrentSongName();

    invokePendingFileOperation();
}

void CListModel::onFileMoved(QString fileMoved)
{
    MP_LOG << "File =" << fileMoved << LOG_ENDL;
    if (fileMoved != "")
    {
        actuallyDeletedFiles.append(fileMoved);
    }
}

void CListModel::setCurrentPlayingItem( int idx )
{
    MP_LOG << "currentPlayingItem = " << idx << LOG_ENDL;
    if( m_currentPlayingItem != idx )
    {
        m_currentPlayingItem = idx;
        emit currentPlayingItemChanged();
    }
}

// { added by junam 2012.10.12 for CR14040
void CListModel::onComponentCompleted(int idx)
{
    if (isCoverArtVisible(m_currentCategoryState)) //changed by junam 2013.10.28 for 2nd depth coverart
    {
        m_itemIndexsOnScreen.append(idx);
        requestCoverTimerStart();
    }
}
void CListModel::onComponentDestruction(int idx)
{

    if (isCoverArtVisible(m_currentCategoryState)) //changed by junam 2013.10.28 for 2nd depth coverart
    {
        int pos = m_itemIndexsOnScreen.indexOf(idx);
        if(pos >= 0)
            m_itemIndexsOnScreen.removeAt(pos);
    }
}

bool CListModel::getOnDemandCoverArt( QStringList& albums, QStringList& artists, QStringList& files, int fetchCount)
{
    QString itemSource;
    while(!m_itemIndexsOnScreen.isEmpty())
    {
        int idx = m_itemIndexsOnScreen.at(0);
        m_itemIndexsOnScreen.removeFirst();

        //{changed by junam 2013.10.28 for 2nd depth coverart
        //if(getMetaInfo(idx, m_listMetaInfo.imageSource).isEmpty() == true &&
        //        getMetaInfo(idx, m_listMetaInfo.itemName).isEmpty() == false)
        itemSource = getMetaInfo(idx, m_listMetaInfo.itemSource);
        if(!itemSource.isEmpty() && !m_imageMap.contains(itemSource)) //}changed by junam
        {
            albums.append(getMetaInfo(idx, m_listMetaInfo.itemName));
            artists.append(getMetaInfo(idx, m_listMetaInfo.itemArtist));
            files.append(getMetaInfo(idx, m_listMetaInfo.itemSource));

            if (m_AudioController.GetMediaMode() == MP::IPOD1 ||
                    m_AudioController.GetMediaMode() == MP::IPOD2)
                break;
            else if(albums.size() >= USB_ODCA_COUNT || albums.size() >= fetchCount) //modified by jaehwan.lee 2013.12.14 for ISV 96345
                break;
        }
    }

    return (albums.isEmpty() == false);
}


void CListModel::fillAlbumCover(const QStringList& albums, const QStringList& covers)
{
    if (!isCoverArtVisible(m_currentCategoryState)) //changed by junam 2013.10.28 for 2nd depth coverart
    {
        MP_LOG<<"[ERROR] Invalid category ="<<m_currentCategoryState << LOG_ENDL;
        return;
    }

    QModelIndex indexModel;

    //Q_ASSERT(albums.size() == covers.size());

    int indexCover=0, indexList;
    foreach(QString album, albums)
    {
        indexList = m_listMetaInfo.itemName.indexOf(album);
        if (indexList < 0)
        {
            MP_LOG<<"ERROR: Cannot find such album: "<<album << LOG_ENDL;
        }
        else if(covers.at(indexCover).compare("No CoverArt Found") == 0)
        {
            MP_LOG<<"skip["<<indexList <<"]  = "<<covers.at(indexCover) << LOG_ENDL;
            //setMetaInfo(indexList, m_listMetaInfo.imageSource, QString("/app/share/images/music/coverflow_default_n.png")); //removed by junam 2013.10.28 for 2nd depth coverart
        }
        else if( false == isCoverFileSizeValid(covers.at(indexCover)))
        {
            MP_LOG<<"invalid ["<<indexList<<"]  = "<<covers.at(indexCover) << LOG_ENDL;
            //setMetaInfo(indexList, m_listMetaInfo.imageSource, QString("/app/share/images/music/coverflow_default_n.png")); //removed by junam 2013.10.28 for 2nd depth coverart
        }
        else
        {
            MP_LOG<<"add ["<<indexList<<"]  = "<<covers.at(indexCover) << LOG_ENDL;
            indexModel = index( indexList, 0);
            setData( indexModel, QVariant::fromValue( covers.at(indexCover) ), imageRole);
        }
        indexCover++;
    }
    //{changed by junam 2013.06.12 for hold coverloading at indexing
    //m_requestCoverTimer.start(100);
    requestCoverTimerStart(100);
    //}changed by junam
}
// } added by junam


// { added by changjin 2012.09.14 for CR 13127
QString CListModel::getExtractSongTitle(QString songTitle)
{
    QString strTmp = "";
    QString retStr = "";

    int nIndex = songTitle.lastIndexOf('/');
    strTmp = songTitle.remove(0, nIndex + 1);
    retStr = strTmp.left(strTmp.lastIndexOf("."));

    return retStr;
}
// } added by changjin 2012.09.14

// { added by wspark 2012.09.20 for CR13633
bool CListModel::isCoverFileSizeValid(QString coverURI)
{
    QFile coverImageFile;
    QString fileName = coverURI;
    if(fileName.startsWith("file://"))
        fileName.remove( "file://" );
    coverImageFile.setFileName(fileName);
    int filesize = coverImageFile.size();
    if(filesize > 0)
        return true;
    else
        return false;
}
// } added by wspark


//added by yongkyun.lee 2012.11.07 for Miner controll
void CListModel::MinerControler(bool miner)
{
    if(miner)
    {
        m_AudioController.pauseMiner();
    }
    else
    {
        m_AudioController.resumeMiner();
        m_AudioController.setJukeboxUpdate(true); //added by junam 2013.10.03 for ITS_KOR_192927
    }

}
//added by yongkyun.lee

// { added by yungi 2013.2.20 for Music Copy Completed not match OSD
DISPLAY_T CListModel::GetOSDDisplay()
{
    DISPLAY_T nDisp = DISPLAY_FRONT_REAR;

    if ( m_AudioController.GetEngineMain().GetFrontView() < APP_MEDIA_MODE_PHOTO_FRONT &&
         m_AudioController.GetEngineMain().GetFrontView() == m_AudioController.GetEngineMain().GetFrontMode() )
    {
        nDisp = DISPLAY_REAR;
    }

    if ( m_AudioController.GetEngineMain().GetRearView() < APP_MEDIA_MODE_PHOTO_FRONT &&
         m_AudioController.GetEngineMain().GetRearView() == m_AudioController.GetEngineMain().GetRearMode() )
    {
        nDisp = DISPLAY_FRONT;
    }

    return nDisp;
}
// } added by yungi 2013.2.20 for Music Copy Completed not match OSD

// { modified by ravikanth 16-04-13
void CListModel::cancelFileCopyOperation(bool isRemoved)
{
    cancelOperationChangeSource(isRemoved);
    // setCopyInProgress(false); // modified by ravikanth 19-04-13
}

void CListModel::setCopyInProgress(bool isCopyInProgress, int usbType) // modified by eugene.seo 2013.05.29
{
    ((AppMediaPlayer_EngineAudio*)m_AudioController.GetEngineMain().GetEngine(APP_MEDIA_MODE_AUDIO))->setCopyToJukeboxStatus(isCopyInProgress, usbType); // modified by eugene.seo 2013.05.29
}

void CListModel::fileOperationStateChanged(bool isCopyInProgress)
{
    // m_fileOperationData.operation = isCopyInProgress ? LVEnums::OPERATION_COPY : LVEnums::OPERATION_NONE);
}
// } modified by ravikanth 16-04-13

// { modified by ravikanth 19-04-13
void CListModel::onCopyOpertaionExit()
{
    MP_LOG<<"copy exit" << LOG_ENDL;
    setCopyInProgress(false, 0); // modified by eugene.seo 2013.05.29
    MinerControler(false); // changes for ITS 0193916
}
// } modified by ravikanth 19-04-13
//{changed by junam 2013.05.14 for iPod re-sync
void CListModel::onDatabaseUpdate(int count)
{
    MP_LOG<<"count = "<< count << LOG_ENDL;    

    //{added by junam 2013.07.22 for ONDEMAND_PLAYLIST
    if(m_currentCategoryState == PLAYLIST_DEPTH_0 || m_currentCategoryState == PLAYLIST_DEPTH_1)
    {
        MP_LOG<<"Playlist category do not need update..." << LOG_ENDL;
        return;
    }
    //}added by junam

    //{added by junam 2013.12.2 for list update
    if( !isCategoryTabAvailable() || m_currentCategoryState == FOLDER_DEPTH_0 || m_currentCategoryState == FOLDER_DEPTH_1)
    {
        MP_LOG<<"No need update..."<<isCategoryTabAvailable()<<" m_currentCategoryState = "<<m_currentCategoryState << LOG_ENDL;
        return;
    }
    //}added by junam

    if(count == 0 )
    {
        this->clear();
        m_currentRequest.itemIndex = 0;
        m_currentRequest.depth = 0;
        m_currentRequest.queryStr = "";
        m_lastRequestStack.clear();
        updateCountInfoString("", 0);
        // emit signalListLoadingIcon(true); // removed by kihyung 2013.05.24
        emit updateTextItemView("", 0);  //changed by junam 2013.09.06 for ITS_KOR_188332
        emit moveTopHistoryStack();
        return;
    }
//}changed by junam

    switch(m_currentCategoryState)
    {
    case ARTIST_DEPTH_0:
    {
        //trimDownMetaInfo();
        //m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsCountFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);

        //MediaInfo info;
        //m_AudioController.GetCurrentMediaInfo(&info);
        ////{added by junam 2013.05.14 for iPod re-sync
        //if (info.Artist.isEmpty())
        //{
        //    m_pListTrackerAbstractor->setQueryOffset(0);
        //    m_pListTrackerAbstractor->setQueryLimit(FETCHED_BLOCK_ITEM_COUNT);
        //    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsWithLimitFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}
        //else //}added by junam
        //{
        //    m_pListTrackerAbstractor->RequestData(info.Artist, m_volumeType, eGetArtistIndexFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}

        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicArtistsFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        break;
    }
    case SONG_DEPTH_0:
    {
        //trimDownMetaInfo();
        //m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithTitleCountFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);

        //MediaInfo info;
        //m_AudioController.GetCurrentMediaInfo(&info);
        ////{added by junam 2013.05.14 for iPod re-sync
        //if (info.fileName.isEmpty())
        //{
        //    m_pListTrackerAbstractor->setQueryOffset(0);
        //    m_pListTrackerAbstractor->setQueryLimit(FETCHED_BLOCK_ITEM_COUNT);
        //    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsWithLimitFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}
        //else //}added by junam
        //{
        //    m_pListTrackerAbstractor->RequestData(info.fileName, m_volumeType, eGetSongIndexFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}

        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllSongsFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        break;
    }
    case ALBUM_DEPTH_0:
    {
        //trimDownMetaInfo();
        //m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsCountFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);

        //MediaInfo info;
        //m_AudioController.GetCurrentMediaInfo(&info);
        ////{added by junam 2013.05.14 for iPod re-sync
        //if (info.Album.isEmpty())
        //{
        //    m_pListTrackerAbstractor->setQueryOffset(0);
        //    m_pListTrackerAbstractor->setQueryLimit(FETCHED_BLOCK_ITEM_COUNT);
        //    m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}
        //else //}added by junam
        //{
        //    m_pListTrackerAbstractor->RequestData(info.Album, m_volumeType, eGetAlbumIndexFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        //}

        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllAlbumsWithUrlFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        break;
    }

    //{added by junam 2013.06.25 for ITS175216
    case ALBUM_DEPTH_1:
    {
        if (!m_currentRequest.queryStr.isEmpty())
        {
            MP_LOG << "eGetAllSongsByAlbumFromVolumeQuery_Id from Volume =" << m_volumeType << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByAlbumFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    case ARTIST_DEPTH_1: // modified 2013.11.11
    {
        if (!m_currentRequest.queryStr.isEmpty())
        {
            MP_LOG << "eGetAllAlbumByArtistQuery_Id for" << m_currentRequest.queryStr << "from volumeType =" << m_volumeType << LOG_ENDL;
            //m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByArtistFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
            m_pListTrackerAbstractor->RequestData(QStringList(m_currentRequest.queryStr), m_volumeType, eGetAllAlbumByArtistQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    case ARTIST_DEPTH_2: // added 2013.11.11
    {
        QStringList requestList;
        requestList.append(m_lastRequestStack.at(0)); // Get Artist
        requestList.append(m_lastRequestStack.at(1)); // Get Album
        if ( requestList.isEmpty() == false && m_lastRequestStack.isEmpty() == false )
        {
            MP_LOG << "eGetAllSongsByArtistAlbumQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
            // { modified by sangmin.seol 2013.12.03 Fixed Query in for database update same with requstListData.
            //m_pListTrackerAbstractor->RequestData(requestList, m_volumeType, eGetAllSongsByArtistFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
            m_pListTrackerAbstractor->RequestData(requestList, m_volumeType, eGetAllSongsByArtistAlbumQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    case GENRE_DEPTH_0:
    {
        MP_LOG << "eGetAllMusicGenresFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicGenresFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        break;
    }
    case GENRE_DEPTH_1:
    {
        if (!m_currentRequest.queryStr.isEmpty())
        {
            MP_LOG << "eGetAllSongsByGenreFromVolumeQuery_Id for" << m_currentRequest.queryStr << "from volumeType =" << m_volumeType << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(m_currentRequest.queryStr, m_volumeType, eGetAllSongsByGenreFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    case COMPOSER_DEPTH_0:
    {
        MP_LOG << "eGetAllMusicComposersFromVolumeQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
        m_pListTrackerAbstractor->RequestData(m_volumeType, eGetAllMusicComposersFromVolumeQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        break;
    }
    case COMPOSER_DEPTH_1:
    {
        if (!m_currentRequest.queryStr.isEmpty())
        {
            MP_LOG << "GetAllAlbumByComposerQuery_Id for " << m_currentRequest.queryStr << "from m_volumeType =" << m_volumeType << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(QStringList(m_currentRequest.queryStr), m_volumeType, eGetAllAlbumByComposerQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    case COMPOSER_DEPTH_2:
    {
        QStringList requestList;
        requestList.append(m_lastRequestStack.at(0)); // Get Composer
        requestList.append(m_lastRequestStack.at(1)); // Get Album
        if ( requestList.isEmpty() == false && m_lastRequestStack.isEmpty() == false )
        {
            MP_LOG << "eGetAllSongsByComposerAlbumQuery_Id from volumeType =" << m_volumeType << LOG_ENDL;
            m_pListTrackerAbstractor->RequestData(requestList, m_volumeType, eGetAllSongsByComposerAlbumQuery_Id, REQUID_LIST_DATABASE_UPDATE);
        }
        break;
    }
    //}added by junam

    default:
        MP_LOG<<"calling requestUpdateListData with m_currentCategoryState"<< m_currentCategoryState  << LOG_ENDL;
        requestUpdateListData();
        break;
    }
}
//}added by junam

//{added by junam 2013.12.31
bool CListModel::checkUpdateResponse(ETrackerAbstarctor_Query_Ids queryId)
{
    switch(m_currentCategoryState)
    {
    case SONG_DEPTH_0:
        if(queryId == eGetAllSongsFromVolumeQuery_Id) return true;
        break;
    case ALBUM_DEPTH_0:
        if(queryId == eGetAllAlbumsWithUrlFromVolumeQuery_Id) return true;
        break;
    case ALBUM_DEPTH_1:
        if(queryId == eGetAllSongsByAlbumFromVolumeQuery_Id) return true;
        break;
    case ARTIST_DEPTH_0:
        if(queryId == eGetAllMusicArtistsFromVolumeQuery_Id) return true;
        break;
    case ARTIST_DEPTH_1:
        if(queryId == eGetAllAlbumByArtistQuery_Id) return true;
        break;
    case ARTIST_DEPTH_2:
        if(queryId == eGetAllSongsByArtistAlbumQuery_Id) return true;
        break;
    case GENRE_DEPTH_0:
        if(queryId == eGetAllMusicGenresFromVolumeQuery_Id) return true;
        break;
    case GENRE_DEPTH_1:
        if(queryId == eGetAllSongsByGenreFromVolumeQuery_Id) return true;
        break;
    case COMPOSER_DEPTH_0:
        if(queryId == eGetAllMusicComposersFromVolumeQuery_Id) return true;
        break;
    case COMPOSER_DEPTH_1:
        if(queryId == eGetAllAlbumByComposerQuery_Id) return true;
        break;
    case COMPOSER_DEPTH_2:
        if(queryId == eGetAllSongsByComposerAlbumQuery_Id) return true;
        break;
    default:
        return true;
    }
    MP_LOG<<"Invaild query respoonse on state : queryID = "<<queryId<<" state = "<<m_currentCategoryState << LOG_ENDL;
    return false;
}
//}added by junam

//{added by junam 2013.06.12 for hold coverloading at indexing
bool CListModel::requestCoverTimerStart(int iTime)
{
//    MP_LOG <<" "<<m_AudioController.isMinerFinished()
//          <<" "<<m_AudioController.isUSB1MinerFinished()
//          <<" "<<m_AudioController.isUSB2MinerFinished()
//          <<" "<<m_AudioController.isFlowViewEnable() << LOG_ENDL;

    if (isCoverArtVisible(m_currentCategoryState)) //changed by junam 2013.10.28 for 2nd depth coverart
    {
        MP::DEVICE_T device = m_AudioController.GetMediaMode();

        if(((device == MP::IPOD1 || device == MP::IPOD2) && m_AudioController.isFlowViewEnable())
                || (device == MP::JUKEBOX && m_AudioController.isMinerFinished())
                || (device == MP::USB1 && m_AudioController.isUSB1MinerFinished())
                || (device == MP::USB2 && m_AudioController.isUSB2MinerFinished()))
        {
            m_requestCoverTimer.start(iTime);
            return true;
        }
    }
    return false;
}
//}added by junam

// modified by ravikanth 21.06.13 for ITS 0174571
void CListModel::onOperationIncomplete()
{
    emit hidePopup(true);
    emit signalShowPopup(LVEnums::POPUP_TYPE_DELETE_INCOMPLETE, false, false);
}

//{added by junam 2013.06.28
void CListModel::setCurrentFocuseIndex( int idx )
{
    m_currentFocuseIndex = idx;
    if(m_currentCategoryState == ALBUM_DEPTH_0 || m_currentCategoryState == ARTIST_DEPTH_0 )
    {
        m_currentFocuseItemName = getMetaInfo( idx, m_listMetaInfo.itemName);
        m_currentFocuseItemTitle = getMetaInfo( idx, m_listMetaInfo.itemTitle);
    }
}
//}added by junam
//{added by junam 2013.06.10 for ITS_179109
bool CListModel::isQuickViewVisible(void)
{
    switch(m_currentCategoryState)
    {
    case ARTIST_DEPTH_0:
    case SONG_DEPTH_0:
    case ALBUM_DEPTH_0:
    case GENRE_DEPTH_1:
        return true;
    default: // make compiler happy
        break;
    }
    return false;
}
//}added by junam

//{added by junam 2013.07.23 for ONDEMAND_PLAYLIST
CIpodController* CListModel::GetIPodController()
{
    if(m_displayMode == MP::IPOD1)
        return m_AudioController.GetControllers().GetIPod1Controller();

    if(m_displayMode == MP::IPOD2)
        return m_AudioController.GetControllers().GetIPod2Controller();

    return NULL;
}
//}added by junam

bool CListModel::isCategoryTabAvailable()   // { added by sungha.choi 2013.08.05 for ITS 182674
{
    if(m_displayMode == MP::JUKEBOX) {
        return m_bCategoryTabAvailableJukeBox;
    } else if(m_displayMode == MP::USB1) {
        return m_bCategoryTabAvailableUSB1;
    } else if(m_displayMode == MP::USB2) {
        return m_bCategoryTabAvailableUSB2;
    } else {    // If any other category except JUKEBOX or USB,
        return true;
    }
}   // } added by sungha.choi 2013.08.05 for ITS 182674

void CListModel::setCategoryTabAvailable(int device, bool available)   // { added by sungha.choi 2013.08.05 for ITS 182674
{
    if(device == MP::JUKEBOX)
    {
        m_bCategoryTabAvailableJukeBox = available;

        //{added by junam 2013.12.11 for ITS_ME_214412
        if(available == false &&  m_categoryTabAvailableTimer.isActive() == false)
            m_categoryTabAvailableTimer.start();
        //}added by junam
    } else if(device == MP::USB1) {
        m_bCategoryTabAvailableUSB1 = available;
    } else if(device == MP::USB2) {
        m_bCategoryTabAvailableUSB2 = available;
    } else {    // If any other category except JUKEBOX or USB,

    }
    //{added by junam 2013.11.20 for dim update
    if(m_AudioController.isListView() && m_bCategoryTabAvailableJukeBox == false && m_displayMode == MP::JUKEBOX && device == MP::JUKEBOX)
    {
        emit categoryTabUpdated(false);
    }
    //}added by junam
}   // } added by sungha.choi 2013.08.05 for ITS 182674

//{changed by junam 2013.11.01 for category disabled state
void CListModel::onCategoryTabAvailable(bool bUSB1Mounted, bool bUSB2Mounted)  
{
    MP_HIGH << "m_bCategoryTabAvailableJukeBox = " << m_bCategoryTabAvailableJukeBox << " m_bCategoryTabAvailableUSB1 = "
          << m_bCategoryTabAvailableUSB1 << " m_bCategoryTabAvailableUSB2 = " << m_bCategoryTabAvailableUSB2 << LOG_ENDL;
    bool bRequested = false;
    if(m_bCategoryTabAvailableJukeBox == false)
    {
        bRequested = true;
        m_pListTrackerAbstractor->RequestData(eJukeBox, eGetAllSongsWithTitleCountFromVolumeQuery_Id, REQUID_LIST_JUKEBOX_CATEGORY_AVAILABLE);      // moved by sungha.choi 2013.07.23
    }
    if(bUSB1Mounted && m_bCategoryTabAvailableUSB1 == false)
    {
        bRequested = true;
        m_pListTrackerAbstractor->RequestData(eUSB_FRONT, eGetAllSongsWithTitleCountFromVolumeQuery_Id, REQUID_LIST_USB1_CATEGORY_AVAILABLE);      // moved by sungha.choi 2013.07.23
    }
    if(bUSB2Mounted && m_bCategoryTabAvailableUSB2 == false)
    {
        bRequested = true;
        m_pListTrackerAbstractor->RequestData(eUSB_REAR, eGetAllSongsWithTitleCountFromVolumeQuery_Id, REQUID_LIST_USB2_CATEGORY_AVAILABLE);      // moved by sungha.choi 2013.07.23
    }
    
    if(bRequested && !m_categoryTabAvailableTimer.isActive())
    {
        MP_LOG<<"categoryTabAvailableTimer  - START" << LOG_ENDL;
        m_categoryTabAvailableTimer.start();
    }
}   
//}changed by junam

//{added by junam 2013.08.20 for quick bar
QVariantList CListModel::getAlphabeticList(int sortOrder)
{
    QVariantList varList;
    QStringList strList;

    if(m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2)
    {
        if(sortOrder >= 0 && sortOrder <= 1)
        {
            QString strKey =  QString("STR_ALPHABETIC_IPOD_LIST_ARRAY_%1").arg(sortOrder);
            strList = QApplication::translate("main", strKey.toAscii()).split(";");
        }
        else
        {
            strList = QApplication::translate("main", "STR_ALPHABETIC_IPOD_LIST_ARRAY_0").split(";");
        }
    }
    else
    {
        strList = QApplication::translate("main", "STR_ALPHABETIC_LIST_ARRAY").split(";");
    }

    m_letterList.clear(); // added by jaehwan 2013.11.11 for quick index table
    foreach(QString letter, strList)
    {
        varList.append(QVariant::fromValue(letter));
        m_letterList.append(letter); // added by jaehwan 2013.11.11 for quick index table
    }
    return varList;
}
//}added by junam

//{ added by jaehwan 2013.10.26 for ISV 90617, get letters for hidden quick scroll.
QVariantList CListModel::getHiddenAlphabeticList(int sortOrder)
{
    QVariantList varList;
    QStringList strList;

    if(m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2)
    {
        if(sortOrder >= 0 && sortOrder <= 1)
        {
            QString strKey =  QString("STR_ALPHABETIC_IPOD_HIDDEN_LIST_ARRAY_%1").arg(sortOrder);
            strList = QApplication::translate("main", strKey.toAscii()).split(";");
        }
        else
        {
            strList = QApplication::translate("main", "STR_ALPHABETIC_IPOD_HIDDEN_LIST_ARRAY_0").split(";");
        }
    }
    else
    {
        strList = QApplication::translate("main", "STR_ALPHABETIC_HIDDEN_LIST_ARRAY").split(";");
    }

    m_hiddenLetterList.clear(); // added by jaehwan 2013.11.11 for quick index table
    foreach(QString letter, strList)
    {
        varList.append(QVariant::fromValue(letter));
        m_hiddenLetterList.append(letter); // added by jaehwan 2013.11.11 for quick index table
    }
    return varList;
}
//}

//{added by jaehwan.lee 2013.10.23, modified by jaehwan 2013.11.12 for spec change
QString CListModel::getListIndexWithLetter(QString letter)
{
    if(letter.length() <= 0) // added by jaehwan 2013.10.31 to avoid crash
    {
        MP_LOG << "invalid letter letter = "<<letter << LOG_ENDL;
        return QString("");
    }
    wchar_t firstChar = letter.data()[0].unicode();
    QString popupLetter = isVisibleLetter(letter) ? letter : "";

    //{ modified by honggi.shin 2013.11.05, Redefine about touching '#' on quick scroll
    if(firstChar == '#')
    {
        //{removed by junam 2014.01.06 for unknown
        //{ added by honggi.shin 2013.11.01, To touch'#' should move index to 0 in usb1, usb2 and jukebox mode
        //if(m_displayMode == MP::USB1 || m_displayMode == MP::USB2 || m_displayMode == MP::JUKEBOX) {
        //    emit setSelectedAlphaOnTop(0);
        //    return popupLetter;
        //}
        //} honggi.shin 2013.11.01
        //}removed by junam

        //{ added by honggi.shin 2013.11.05, To touch'#' should move index to last point in ipod mode
        if( m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2 )
        {
            //{ added 2013.12.23, NA PUMWHAK spec change
            if (m_currentRequest.tabId == "Song")
            {
                for(int inx = m_listMetaInfo.itemQuickIndex.count()-1; inx >=0; inx--)
                {
                    if( m_listMetaInfo.itemQuickIndex.at(inx) >= 'A' && m_listMetaInfo.itemQuickIndex.at(inx) <= 'Z' && inx < m_listMetaInfo.itemQuickIndex.count()-1 )
                    {
                        emit setSelectedAlphaOnTop(inx+1);
                        return popupLetter;
                    }
                }
            }
            //} added 2013.12.23

            bool sharpAreaFlag = false;
            for(int i = m_listMetaInfo.itemQuickIndex.count()-1; i >= 0; i--)
            {
                if(sharpAreaFlag == false && m_listMetaInfo.itemQuickIndex.at(i) == '#') {
                    sharpAreaFlag = true;
                } else if(sharpAreaFlag == true && m_listMetaInfo.itemQuickIndex.at(i) != '#') {
                    emit setSelectedAlphaOnTop(i+1);
                    return popupLetter;
                }
            }

        }
        //} added by honggi.shin 2013.11.05
    }
    //} honggi.shin 2013.11.05

    bool firstMatchFail = false;
    int  lastIndexed = -1;

    for (int  i = 0; i < m_quickIndexTab.count();i++)
    {
        if (m_quickIndexTab.at(i).letter == firstChar )
        {
            if (m_quickIndexTab.at(i).index >= 0 ) //firt Match case;
            {
                emit setSelectedAlphaOnTop(m_quickIndexTab.at(i).index);
                QString result = QString::fromWCharArray(&m_quickIndexTab.at(i).letter , 1);
                return popupLetter;
           }
            else
            {
                //first match(same letter) fail, start second (next index) match
                firstMatchFail = true;
            }
        }
        else if (firstMatchFail) // try next match case;
        {
            if (m_quickIndexTab.at(i).index >= 0)
            {
                emit setSelectedAlphaOnTop(m_quickIndexTab.at(i).index);
                return popupLetter;
            }
        }
        else if (m_quickIndexTab.at(i).index >= 0 )
        {
            lastIndexed = m_quickIndexTab.at(i).index;
        }
    }

    // no match, last indexed row number
    if ( lastIndexed  >= 0 )
    {
        // find the first unindexed row.
        int findNextIndex = -1;
        for (int i = lastIndexed + 1 ; i < m_listMetaInfo.itemQuickIndex.count(); i++ )
        {
            if (m_listMetaInfo.itemQuickIndex.at(i) != m_listMetaInfo.itemQuickIndex.at(lastIndexed))
            {
                findNextIndex = i;
                break;
            }
        }
        if (findNextIndex >= 0 )
            emit setSelectedAlphaOnTop(findNextIndex);
        else
            emit setSelectedAlphaOnTop(lastIndexed);
    }
    else
    {
        emit setSelectedAlphaOnTop(0);
    }
    return popupLetter;
}
//} added by jaehwan

//{changed by junam 2013.09.13 for ITS_KOR_189335
bool CListModel::IsFileSupported(int Index)
{
    if (m_displayMode == MP::JUKEBOX || m_displayMode == MP::USB1 || m_displayMode == MP::USB2)
    {
        switch(m_currentCategoryState)
        {
        case FOLDER_DEPTH_0:
        case FOLDER_DEPTH_1:            
            if(getMetaInfo( Index, m_listMetaInfo.isFolder)) //added by junam for 2013.11.11 for ITS_KOR_208044
                return true;
            break;
        case SONG_DEPTH_0:
        case ALBUM_DEPTH_1:
        case ARTIST_DEPTH_2: //changed by junam 2014.01.03
        case GENRE_DEPTH_1:
            break;
        default:
            return true;
        }

        if( Index < m_listMetaInfo.itemSource.size())
        {
            return m_AudioController.IsFileSupported(m_listMetaInfo.itemSource.at(Index));
        }
    }
    return true;
}
//}changed by junam

// added by kihyung 2013.10.08 for ITS 0193747
void CListModel::setCopySpeed(int nSpeed)
{
    MP_LOG << nSpeed << LOG_ENDL;
    if(m_pFileOperationsHelper) {
        m_pFileOperationsHelper->setCopySpeed(nSpeed);
    }
}

// modified by ravikanth for ITS 0190283
void CListModel::deleteSelectedModelItems()
{
    MP_LOG << m_nTotalItemCount << m_listMetaInfo.isItemCheckBoxMarked.count() << LOG_ENDL;

    if(m_nTotalItemCount != m_currentDeletedFileCount)
    {
        beginRemoveRows(QModelIndex(), 0, m_nTotalItemCount - 1);
        for (int i = 0; i < m_listMetaInfo.isItemCheckBoxMarked.count();)
        {
            //MP_LOG << "i= "<<i<<" isItemCheckBoxMarked= "<<m_listMetaInfo.isItemCheckBoxMarked.at(i) << LOG_ENDL;
            //MP_LOG << m_listMetaInfo.isItemCheckBoxMarked.count() << LOG_ENDL;
            if (m_listMetaInfo.isItemCheckBoxMarked.at(i))
            {
                m_listMetaInfo.imageSource.removeAt(i);
                m_listMetaInfo.itemQuickIndex.removeAt(i); //added by jaehwan.lee to fix ITS 226004
                m_listMetaInfo.itemArtist.removeAt(i);
                m_listMetaInfo.itemName.removeAt(i);
                m_listMetaInfo.itemSource.removeAt(i);
                m_listMetaInfo.itemTitle.removeAt(i);
                m_listMetaInfo.isFolder.removeAt(i);
                m_listMetaInfo.isSelectable.removeAt(i);
                m_listMetaInfo.isItemCheckBoxMarked.removeAt(i);
            }
            else
            {
                i++;
            }
        }

        // modified by sangmin.seol 2013.12.03 for ITS_0212235 Fixed listview item count is set negative
        //m_nTotalItemCount -= m_currentDeletedFileCount;
        m_nTotalItemCount = m_listMetaInfo.itemTitle.size();

        endRemoveRows();
        setPlaybackIcon();  // { added by honggi.shin 2013.11.11 for ITS_0208058
    }
    else
    {
        clear();
    }
    updateCountInfoString(m_currentRequest.tabId, m_nTotalItemCount); // added by jaehwan.lee 2013.12.11 for ITS 214228
    m_currentDeletedFileCount = 0;
}

int CListModel::indexOfFile(QString file, QStringList &itemList)
{
    int idx = -1;
    MP_LOG << "currentPlayingFile =" << file << LOG_ENDL;

    if (!file.isEmpty() && !itemList.isEmpty())
    {
        if (itemList.at(0).startsWith("file://"))
        {
            if(!file.startsWith("file://"))
                file.prepend("file://");
        }
        else
        {
            if(file.startsWith("file://"))
                file.remove(0, 7);
        }

        if(m_displayMode == MP::IPOD1 || m_displayMode == MP::IPOD2)
        {
            quint64 currUID;
            if( CTrackerAbstractor::getIPodFileUID(file, currUID))
            {
                quint64 itemUID;
                for(int i = 0; i < itemList.size(); i++)
                {
                    if( CTrackerAbstractor::getIPodFileUID(itemList.at(i), itemUID) && currUID == itemUID)
                    {
                        idx = i;
                        break;
                    }
                }
            }
            else
            {
                MP_LOG<<"invaild current playing file : "<<file << LOG_ENDL;
            }
        }
        else
        {
            idx = itemList.indexOf(file);
        }

        if (idx < 0)
        {
            // modified by Dmitry 31.07.13 for ITS0181701
            int folderCnt = file.contains("rw_data") ? file.count("/")-5 : file.count("/")-4;
            // modified by Dmitry 31.07.13 for ITS0181701
            QString tmpPlayingPath = file;

            for(int i = 0;i<folderCnt;i++)
            {
                tmpPlayingPath = tmpPlayingPath.mid(0, tmpPlayingPath.lastIndexOf("/"));
                idx = itemList.indexOf(tmpPlayingPath);
                if(idx>-1)
                {
                    i+=folderCnt;
                }
            }
        }
    }
    return idx;
}

//{added by junam 2013.10.23 for add aritst depth
void CListModel::clearHistoryStack()
{
    m_currentRequest.depth = 0;
    m_currentRequest.queryStr.clear();
    m_lastRequestStack.clear();
    m_lastRequestIndexStack.clear(); //added by junam 2013.10.24
}
//}added by junam

bool CListModel::isFileOperationInProgress()
{
    return (FileOperationsHelper::operationUnknown != m_pFileOperationsHelper->currentOperation());
}

//{changed by junam 2013.11.01 for category disabled state
 void CListModel::onCategoryTabAvailableTimeOver()
 {
     MP_LOG<<" - TIMEOVER " << LOG_ENDL;
     onCategoryTabAvailable(m_AudioController.isUSBMounted(MP::USB1), m_AudioController.isUSBMounted(MP::USB2));
 }
//}changed by junam

//{ added by jaehwan 11.11 for quick scroll index table
void CListModel::makeQuckIndexTable()
{
    m_quickIndexTab.clear();
    foreach (QString letter, m_hiddenLetterList)
    {
        QUICK_INDEX_ITEM_T qIdxItem;
        qIdxItem.letter = wchar_t(letter.data()[0].unicode());
        qIdxItem.index = -1;
        for ( int i = 0 ; i < m_listMetaInfo.itemQuickIndex.count(); i++)
        {
            if ( qIdxItem.letter == m_listMetaInfo.itemQuickIndex.at(i))
            {
                qIdxItem.index = i;
                break;
            }
        }
        m_quickIndexTab.append(qIdxItem);
    }
}

bool CListModel::isVisibleLetter(QString letter)
{
    foreach(QString visibleLetter, m_letterList)
    {
        if (visibleLetter == letter)
            return true;
    }
    return false;
}
//} added by jaehwan

 // modified for ITS 0208724
 void CListModel::showCopyCompletePopup()
 {
     MP_HIGH << LOG_ENDL;
     emit finishEditMode();
     emit hidePopup(true); // modified by eugene.seo 2013.04.10
     emit signalShowPopup(LVEnums::POPUP_TYPE_COPY_COMPLETE, false, false);
 }
 
void CListModel::SetFileOperationModeType()
{
    MP_LOG<< m_AudioController.GetMediaMode() << LOG_ENDL;
    if(m_pFileOperationsHelper)
    {
        EOperationMediaType mediaType = eOperationMediaNone;
        if(m_AudioController.GetMediaMode() == MP::JUKEBOX)
        {
            mediaType = eOperationMediaJukeBox;
        }
        else if(m_AudioController.GetMediaMode() == MP::USB1 || m_AudioController.GetMediaMode() == MP::USB2)
        {
            mediaType = eOperationMediaUSB;
        }
        m_pFileOperationsHelper->setMediaType(mediaType);
    }
}

// modified to avoid list update in edit state on copy complete 
void CListModel::RequestListDataOnContentChanged()
{
    MP_LOG << m_bUpdateListOnEditExit << LOG_ENDL;
    if(m_bUpdateListOnEditExit)
    {
        if(m_AudioController.GetMediaMode() == MP::JUKEBOX)
            requestUpdateListData();
        m_bUpdateListOnEditExit = false;
    }
}

// { moved by cychoi 2014.03.07 for ITS 228996 CDDA list flickering on item selection
QString CListModel::onItemClick( int index )
{
    MP_LOG << LOG_ENDL; // added by eugeny.novikov 2012.10.18 for CR 14542

    if(m_AudioController.GetCurrentTrackIndex() != index)//{ added by yongkyun.lee 20130403 for : ISV 78542
    {
        m_AudioController.setScanMode(SCANOFF);// added by yongkyun.lee 20130702 for : ITS 177744
        m_AudioController.setCurrentIndex( index );
    }
    //{ added by cychoi 2013.10.11 for ITS 195149 Scan Off on same item click
    else
    {
        if(m_AudioController.getScanMode() != SCANOFF)
        {
            m_AudioController.setScanMode(SCANOFF);
        }
    }
    //} added by cychoi 2013.10.11
    emit closeList();

    return "";
}
// } moved by cychoi 2014.03.07

