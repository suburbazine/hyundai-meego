#include "util/audio/DHAVN_AppMediaPlayer_CATreeModel.h"
#include "engine/DHAVN_AppMediaPlayer_EngineAudio.h"


CATreeItem::CATreeItem()
{
    /* Init item as a file by default */
    m_itemId = 0;
    m_bIsFolder = false;
    m_folderBlockCount = -1;
    m_fetchedBlockCount = 0;
    m_childItemCount = 0;
    m_currChildFolderCount = 0; // added by eugeny.novikov 2012.10.18 for CR 14542
    m_pParent = NULL;
    m_isRoot = false;// modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
}

CATreeItem::~CATreeItem()
{
    qDeleteAll( childs );
    m_pParent = NULL;
}

CATreeItem* CATreeItem::getChildByIndex( int index ) const
{
    if ( index >= 0 && index < childCount() )
        return childs[index];

    return NULL;
}

//{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
CATreeItem* CATreeItem::getSubFolderById(CATreeItem* subItem, int id ) 
{
    CATreeItem* tmp = NULL;
    foreach ( CATreeItem *item, subItem->childs )
    {
        if ( item && item->isFolder() )
        {
            if(item->itemId() == id)
            {
                return item;
            }
            if(item->folderCount() > 0)
            {
                tmp = item->getSubFolderById(item , id );
                if(tmp != NULL)
                    return tmp;
            }
        }
    }
    return NULL;
}
//} added by yongkyun.lee 20130411 

CATreeItem* CATreeItem::getChildFolderById( int id ) const
{
   //{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
   //foreach ( CATreeItem *item, childs )
   //{
   //    if ( item && item->isFolder() && item->itemId() == id )
   //        return item;
   //}
    CATreeItem* tmp=NULL; 
    foreach ( CATreeItem *item, childs )
    {
        if ( item && item->isFolder() )
        {
            if(item->itemId() == id)
            {
            return item;
    }
            if(item->folderCount() > 0)
            {
                tmp = item->getSubFolderById(item , id );
                if(tmp != NULL)
                    return tmp;
            }
        }
    }
//} added by yongkyun.lee 20130411
    return NULL;
}

//{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
CATreeItem* CATreeItem::getRootFolderById(CATreeItem* rootItem , int id ) 
 {
     CATreeItem* tmp=NULL;
     foreach ( CATreeItem *item, rootItem->childs )
     {
         if ( item && item->isFolder() )
         {
             if(item->itemId() == id)
             {
                 return item;
             }
             if(item->folderCount() > 0)
             {
                 tmp = item->getSubFolderById(item , id );
                 if(tmp != NULL)
                     return tmp;
             }
         }
     }
    return NULL;
}
//} added by yongkyun.lee 20130411

//{ added by yongkyun.lee 20130520 for : MP3CD Current item List
CATreeItem* CATreeItem::getSubPlayFolderById(CATreeItem* subItem, CAStatusResponse id ) 
{
    CATreeItem* tmp = NULL;
    foreach ( CATreeItem *item, subItem->childs )
    {
        if ( item )
        {
            if((item->itemId() == id.currentFolderNo) && item->isFolder())
            {
                return item;
            }
            if(item->folderCount() > 0)
            {
                tmp = item->getSubPlayFolderById(item , id );
                if(tmp != NULL)
                {  
                    return tmp;
                }
            }
        }
    }
    return NULL;
} 


CATreeItem* CATreeItem::getCurrPlayFolderById(CATreeItem* rootItem , CAStatusResponse id ) 
 {
     CATreeItem* tmp=NULL;

     if(id.currentFolderNo == 0 )
     {
         return rootItem;
     }
     
     foreach ( CATreeItem *item, rootItem->childs )
     {
         if ( item  )
         {
             if((item->itemId() == id.currentFolderNo) && item->isFolder())
             {
                 return item;
             }
             if(item->folderCount() > 0)
             {
                 tmp = item->getSubPlayFolderById(item , id );
                 if(tmp != NULL)
                 {
                     return tmp;
                 }
             }
         }
     }
       
    return NULL;
}
//} added by yongkyun.lee 20130520 

//{ added by yongkyun.lee 20130513 for : ITS 168193 / 167858 / 167821
CATreeItem* CATreeItem::getSubPlayById(CATreeItem* subItem, CAStatusResponse id ) 
{
    CATreeItem* tmp = NULL;
    foreach ( CATreeItem *item, subItem->childs )
    {
        if ( item )
        {
            //{ added by yongkyun.lee 20130515 for : NO CR 
            //if(item->itemId() == id.currentFileNo || item->itemId() == id.currentFolderNo)
            if((item->itemId() == id.currentFileNo && !item->isFolder())|| ((item->itemId() == id.currentFolderNo) && item->isFolder()))
            //} added by yongkyun.lee 20130515 
            {
                return item;
            }
            if(item->folderCount() > 0)
            {
                tmp = item->getSubPlayById(item , id );
                if(tmp != NULL)
                    return tmp;
            }
        }
    }
    return NULL;
} 

CATreeItem* CATreeItem::getCurrPlayById(CATreeItem* rootItem , CAStatusResponse id ) 
 {
     CATreeItem* tmp=NULL;
     foreach ( CATreeItem *item, rootItem->childs )
     {
         if ( item  )
         {
             //MP_LOG << "[MP] CATreeItem::getCurrPlayById() =" << item->itemId() << id.currentFileNo << id.currentFolderNo << LOG_ENDL;
             //{ added by yongkyun.lee 20130515 for : NO CR 
             //if(item->itemId() == id.currentFileNo || item->itemId() == id.currentFolderNo)
             if((item->itemId() == id.currentFileNo && !item->isFolder())|| ((item->itemId() == id.currentFolderNo) && item->isFolder()))
             //} added by yongkyun.lee 20130515 
             {
                 return item;
             }
             if(item->folderCount() > 0)
             {
                 tmp = item->getSubPlayById(item , id );
                 if(tmp != NULL)
                     return tmp;
             }
         }
     }
    return NULL;
}
//} added by yongkyun.lee 20130513 


void CATreeItem::append( DvdcDirTree *dirTreeChunk )
{

    QString resultText; //added by wonseok.heo for ITS 175667 2013.06.23

    //MP_LOG << "[MP] CATreeItem::append(): Append to folder =" << dirTreeChunk->curr_folder << LOG_ENDL;

    if ( isFolderFetched() )
    {
        //MP_LOG << "[MP] CATreeItem::append(): Current folder fully fetched, return" << LOG_ENDL;
        return;
    }

    m_childItemCount = dirTreeChunk->chld_folder_num + dirTreeChunk->chld_file_num;
    m_folderBlockCount = ( m_childItemCount % DIR_TREE_BLOCK_NUM == 0 ) ? m_childItemCount / DIR_TREE_BLOCK_NUM
                                                                        : m_childItemCount / DIR_TREE_BLOCK_NUM + 1;
    // { commented by kihyung 2012.10.08
    // MP_LOG << "[MP] CATreeItem::append(): m_childItemCount =" << m_childItemCount << LOG_ENDL;
    // MP_LOG << "[MP] CATreeItem::append(): m_folderBlockCount =" << m_folderBlockCount << LOG_ENDL;
    // } commented by kihyung

    DvdcDirEntry **entries = dirTreeChunk->entry;

    if ( !entries )
    {
        m_fetchedBlockCount = m_folderBlockCount;
        return;
    }

    for ( int i = 0; i < DIR_TREE_BLOCK_NUM; i++ )
    {
        if ( !entries[i] )
        {
            return;
        }
        
        // { commented by kihyung 2012.10.08
        // MP_LOG << "[MP] ----------------------------------------" << LOG_ENDL;
        // MP_LOG << "[MP] CATreeItem::append(): entries[i].type =" << entries[i]->type << LOG_ENDL;
        // } commented by kihyung
        
        if ( entries[i]->type == ITEM_UNDEFINED )
        {
            m_fetchedBlockCount = m_folderBlockCount;
            MP_LOG << "[MP] CATreeItem::append(): ITEM_UNDEFINED is met. Folder fully fetched" << LOG_ENDL;
            return;
        }
        else
        {
            CATreeItem *item = new CATreeItem();
            item->setItemId( entries[i]->file_folder_num );
            item->setParent( this );

            // { added by wonseok.heo for ITS 175667 2013.06.23
            //item->setItemName( charToUnicode( (char*)entries[i]->file_folder_name, dirTreeChunk->unicode ) );
            resultText = charToUnicode( (char*)entries[i]->file_folder_name, dirTreeChunk->unicode );
            if( resultText.size() >= 32 && (entries[i]->type == ITEM_FILE_MP3 || entries[i]->type == ITEM_FILE_WMA) )
            {
                if(entries[i]->type == ITEM_FILE_MP3)
                {
                    resultText = resultText + ".MP3";

                }else if(entries[i]->type == ITEM_FILE_WMA)
                {
                    resultText = resultText + ".WMA";
                }
            }
            item->setItemName( resultText );
            // } added by wonseok.heo for ITS 175667 2013.06.23

            item->setIsFolder( entries[i]->type == ITEM_FOLDER );

            /* Sort folder/file order in list
             * Insert new folder after folders and before file block in list
             * Always append a file to the end of the list */
           // if ( entries[i]->type == ITEM_FILE_MP3 || entries[i]->type == ITEM_FILE_WMA || childCount() == 0 )
            if ( (entries[i]->type == ITEM_FILE_MP3 || entries[i]->type == ITEM_FILE_WMA || childCount() == 0) && (entries[i]->type != ITEM_FOLDER) )  //modified by aettie.ji 2013.02.06 for ISV 68133
            {
                childs.append( item );
            }
            else if ( entries[i]->type == ITEM_FOLDER )
            {
                int j = 0;
                for ( j = 0; j < childCount(); j++ )
                {
                    //item->setFolderCmd(false);// deleted by yongkyun.lee 20130417 for : Next folder list
                    if ( !childs[j]->isFolder() )
                    {
                        //MP_LOG << "[MP] CATreeItem::append(): New folder is inserted to pos " << j << LOG_ENDL;
                        childs.insert( j, item );
                        //item->setFolderCmd(true);// deleted by yongkyun.lee 20130417 for : Next folder list
                        break;
                    }
                }

                if ( j == childCount() ) // rollback by cychoi 2013-07-16 for Prevent fix
                {
                    //MP_LOG << "[MP] CATreeItem::append(): Folders only in list, append to the end";
                    childs.append( item );
                }
                m_currChildFolderCount++; // added by eugeny.novikov 2012.10.18 for CR 14542
            }
            // { added by cychoi 2013-07-16 for Prevent fix
            else
            {
                delete item;
            }
            // } added by cychoi 2013-07-16

            // { commented by kihyung 2012.10.08
            // MP_LOG << "[MP] CATreeItem::append(): Id =" << item->itemId() << LOG_ENDL;
            // MP_LOG << "[MP] CATreeItem::append(): Name =" << item->itemName() << LOG_ENDL;
            // MP_LOG << "[MP] CATreeItem::append(): isFolder =" << item->isFolder() << LOG_ENDL;
            // } commented by kihyung
        }
    }

    m_fetchedBlockCount++;

    // { commented by kihyung 2012.10.08
    // MP_LOG << "[MP] CATreeItem::append(): Finish appending. m_fetchedBlockCount =" << m_fetchedBlockCount << LOG_ENDL;
    // } commented by kihyung
}

QString CATreeItem::charToUnicode( char *pIn, int bUnicode )
{
    if ( !pIn )
        return "";

    int readLength;
    QChar qChar[DIR_TREE_FILE_NAME_SIZE];
    QString resultStr;

    if( bUnicode == 1 )
    {
        unsigned char   p[2];
        unsigned short  *q = (unsigned short*)p;
        int     i, j = 0;

        for( i = 0; i < DIR_TREE_FILE_NAME_SIZE - 1; i++, j = j + 2 )
        {
            p[0] = pIn[j+1];
            p[1] = pIn[j];
            qChar[i] = *q;

            if( qChar[i] == 0 ) {
                break;
            }
        }

        qChar[DIR_TREE_FILE_NAME_SIZE - 1] = 0;
        readLength = i;
    }
    else
    {
        int i;
        for( i = 0; i < DIR_TREE_FILE_NAME_SIZE - 1; i++ )
        {
            qChar[i] = pIn[i];
            if( qChar[i] == 0 ) {
                break;
            }
        }
        qChar[i] = 0;

        readLength = i;
    }
    //MP_LOG << "CATreeItem::charToUnicode(): readLength =" << readLength << LOG_ENDL;
    resultStr.setUnicode( qChar, readLength );

    return resultStr;
}

/*******************************************************************************/

CATreeModel::CATreeModel( AppMediaPlayer_Controller_Audio *appControllerAudio, QObject *parent )
    : m_pControllerAudio( appControllerAudio ),
      m_pRootFolder( NULL ),
      m_pCurrentItem( NULL )
{
    Q_UNUSED( parent );

    m_bIsRequestInProgress = false;
    m_bIsTreeFullyFetched = false;
    m_pendingFolderRequest = -1;
    m_currentPlayingItem = -1; // added by eugeny.novikov 2012.10.18 for CR 14542
    m_PrevItem = NULL; // added by yongkyun.lee 20130613 for : ITS 83292
    //{ modified by yongkyun.lee 2013-09-07 for : NOCR MP3 List
    m_tmp_entryTree = -1;
    m_OldBlockNo = -1;
    m_OldFolderNo = -1;
    //} modified by yongkyun.lee 2013-09-07 

    QHash<int, QByteArray> roles = roleNames();
    roles.insert( NameRole, QByteArray( "itemViewTitle" ) );
    roles.insert( imageRole,  QByteArray( "itemViewImage" ) );
    roles.insert( urlRole,    QByteArray( "itemURL" ) );
//    roles.insert( activeRole, QByteArray( "selected" ) ); // removed by eugeny.novikov 2012.10.18 for CR 14542
    roles.insert( checkBoxMarked, QByteArray( "isCheckBoxMarked" ) );
    roles.insert( selectableRole, QByteArray( "isSelectable" ) );
    roles.insert( imageVisibleRole, QByteArray( "isImageVisible" ) );//added by junam 2012.10.12 for CR14040
    setRoleNames(roles);

    // { modified by kihyung 2013.08.08 for ITS 0183028
    connect(m_pControllerAudio, SIGNAL(mediaInfoChanged(int, QString, QString, QString, QString, QString, QString, QString, QString)),
                          this, SLOT(onMp3CdMediaInfoChanged()));
    // } modified by kihyung 2013.08.08 for ITS 0183028

    m_currentFocusItem = -1; // added by cychoi 2013-07-15 for Prevent fix
    // removed by cychoi 2014.04.06 for ITS 233500
}

CATreeModel::~CATreeModel()
{
    m_currentModel.clear();
    delete m_pRootFolder;
    m_pCurrentItem = NULL;
    m_pControllerAudio = NULL;
}

QVariant CATreeModel::data( const QModelIndex &index, int role ) const
{
    if ( !m_pCurrentItem || !index.isValid() || index.row() > rowCount() )
        return QVariant();

    CATreeItem *item = m_currentModel.at( index.row() );

    if ( !item )
        return QVariant();

    switch ( role )
    {
        case NameRole:
            return QVariant::fromValue( item->itemName() );

        case imageRole:
        {
            if (item->isFolder())
            {
                // { modified by dongjin 2012.08.29 for New UX
                //return "/app/share/images/music/icon_folder_n.png";
                    return "/app/share/images/music/ico_tab_list_folder.png";
                // } modified by dongjin
            }
            return QVariant();
        }

        case urlRole:
            return QVariant::fromValue(QString(""));

        // { removed by eugeny.novikov 2012.10.18 for CR 14542
//        case activeRole:
//            return QVariant::fromValue(false);
        // } removed by eugeny.novikov

        case checkBoxMarked:
            return QVariant::fromValue(false);

        case selectableRole:
            return QVariant::fromValue(true);
        // { added by junam 2012.10.12 for CR14040
        case imageVisibleRole:
            return QVariant::fromValue(item->isFolder());
        // } added by juanm

        default:
            return QVariant();
    }
}

int CATreeModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return m_currentModel.count();
}

void CATreeModel::appendChunkToModel( DvdcDirTree *dirTreeChunk )
{
    //MP_LOG << "[MP] CATreeModel::appendChunkToModel() is called" << LOG_ENDL;
    m_bIsRequestInProgress = false;

    if ( !dirTreeChunk )
        return;

    if ( m_pRootFolder == NULL )
    {
        m_pRootFolder = new CATreeItem();

        if ( !m_pRootFolder )
            return;
// modified by aettie 20130812 for ITS 183630
        QString rootFolder = QApplication::translate("main", QT_TR_NOOP(  "STR_MEDIA_ROOT_FOLDER"));
        m_pRootFolder->setIsFolder( true );
        m_pRootFolder->setRootFolder(true);// modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
        m_pRootFolder->setItemName(rootFolder);
        //m_pRootFolder->setItemName("Root");// added by yongkyun.lee 20130417 for : Next folder list
        m_pCurrentItem = m_pRootFolder;
    }

    CATreeItem *tempItem = NULL;

    /* Root folder on deck always have id 0 */
    //{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
    //tempItem = ( dirTreeChunk->curr_folder == 0 ) ? m_pRootFolder
    //                                              : m_pRootFolder->getChildFolderById( dirTreeChunk->curr_folder );
    tempItem = ( dirTreeChunk->curr_folder == 0 ) ? m_pRootFolder
                                                  : m_pRootFolder->getRootFolderById(m_pRootFolder , dirTreeChunk->curr_folder );
    m_pCurrentItem = tempItem;
    //} added by yongkyun.lee 20130411
    
     //{ modified by yongkyun.lee 2013-09-05 for : NOCR MP3 List 
     if( (m_OldBlockNo  == dirTreeChunk->block_no) &&
         (m_OldFolderNo == dirTreeChunk->curr_folder) )
     {
          MP_LOG << "CATreeModel::appendChunkToModel : file name =" << m_tmp_entryTree << dirTreeChunk->entry[0]->file_folder_num << LOG_ENDL;
         if(m_tmp_entryTree == dirTreeChunk->entry[0]->file_folder_num )
         {
            if ( !tempItem->isFolderFetched() )
            {
                MP_LOG << "CATreeModel::appendChunkToModel : requestChunk" << m_tmp_entryTree << m_OldBlockNo << m_OldFolderNo << LOG_ENDL;
                requestChunk( dirTreeChunk->curr_folder, dirTreeChunk->block_no + 1 );
            }
            else
            {
                MP_LOG << "CATreeModel::appendChunkToModel : fetchNextFolder" << m_tmp_entryTree << m_OldBlockNo << m_OldFolderNo << LOG_ENDL;
                fetchNextFolder(tempItem);
            }
            return;
         }
     }
     m_OldBlockNo    = dirTreeChunk->block_no    ;  
     m_OldFolderNo   = dirTreeChunk->curr_folder ;  
     m_tmp_entryTree = dirTreeChunk->entry[0]->file_folder_num;
     //} modified by yongkyun.lee 2013-09-05 
     
    if ( tempItem )
    {
        tempItem->append( dirTreeChunk );
        refreshCurrentModel();

        if ( m_pendingFolderRequest != -1 )
        {
            //MP_LOG << "[MP] CATreeModel::appendChunkToModel(): Send pending request" << LOG_ENDL;
            sendPendingRequest();
        }
        else
        {
            if ( !tempItem->isFolderFetched() )
            {
                //MP_LOG << "[MP] CATreeModel::appendChunkToModel(): Continue fetching current folder" << LOG_ENDL;
                requestChunk( dirTreeChunk->curr_folder, dirTreeChunk->block_no + 1 );
            }
            else
            {
                //MP_LOG << "[MP] CATreeModel::appendChunkToModel(): Fetch next unfetched folder" << LOG_ENDL;
                fetchNextFolder(tempItem);
            }
        }
    }
}

//{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
void CATreeModel::sendSubFolderRequest()
{
    CATreeItem *folder = NULL;
    folder = ( m_pendingFolderRequest == 0 ) ? m_pRootFolder
                                             : m_pRootFolder->getChildFolderById( m_pendingFolderRequest );

    if ( folder )
    {
        MP_LOG << "CATreeModel::sendSubFolderRequest(): folder = " << m_pendingFolderRequest << ", block = " << folder->fetchedBlockCount() << LOG_ENDL;
        requestChunk( m_pendingFolderRequest, folder->fetchedBlockCount() );
    }
    m_pendingFolderRequest = -1;
}
//} added by yongkyun.lee 20130411 


void CATreeModel::sendPendingRequest()
{
    CATreeItem *folder = NULL;
    folder = ( m_pendingFolderRequest == 0 ) ? m_pRootFolder
                                             : m_pRootFolder->getChildFolderById( m_pendingFolderRequest );

    if ( folder )
    {
        MP_LOG << "[MP] CATreeModel::sendPendingRequest(): folder = " << m_pendingFolderRequest << ", block = " << folder->fetchedBlockCount() << LOG_ENDL;
        requestChunk( m_pendingFolderRequest, folder->fetchedBlockCount() );
    }
    m_pendingFolderRequest = -1;
}

//{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
/*void CATreeModel::fetchNextFolder()
{
    MP_LOG << "[MP] CATreeModel::fetchNextFolder() is called" << LOG_ENDL;

    if ( m_bIsRequestInProgress || m_bIsTreeFullyFetched )
        return;

    if ( !m_pRootFolder->isFolderFetched() )
    {
        requestChunk( 0, m_pRootFolder->fetchedBlockCount() );
    }
    else
    {
        int i;
        for ( i = 0; i < m_pRootFolder->childCount(); i++ )
        {
            CATreeItem *child = m_pRootFolder->getChildByIndex( i );

            if ( !child  )
                break;

            if ( child->isFolder() && !child->isFolderFetched() )
            {
                requestChunk( child->itemId(), child->fetchedBlockCount() );
                break;
            }
        }

        if ( i == m_pRootFolder->childCount() )
        {
            MP_LOG << "[MP] CATreeModel::fetchNextFolder(): Directory tree has been fully fetched!" << LOG_ENDL;
            m_bIsTreeFullyFetched = true;
        }
    }
}
*/
void CATreeModel::fetchNextFolder(CATreeItem *tempItem)
{
    MP_LOG << "[MP] CATreeModel::fetchNextFolder() is called" << LOG_ENDL;

    if ( m_bIsRequestInProgress || m_bIsTreeFullyFetched )
        return;

    if ( !tempItem->isFolderFetched() )
    {
        requestChunk( tempItem->itemId(), tempItem->fetchedBlockCount() );
    }
    else
    {
        int i;
        for ( i = 0; i < tempItem->childCount(); i++ )
        {
            CATreeItem *child = tempItem->getChildByIndex( i );

            if ( !child  )
                break;

            if ( child->isFolder() && !child->isFolderFetched() )
            {
                requestChunk( child->itemId(), child->fetchedBlockCount() );
                break;
            }
            //{ deleted by yongkyun.lee 20130417 for : Next folder list
            /*else if ( child->isFolder() && child->getFolderCmd() )
            {
                requestChunk( child->itemId(), 0 );
                break;
            }*/
            //} deleted by yongkyun.lee 20130417
        }

            if ( i == tempItem->childCount() )
        {
            //{ modified by yongkyun.lee 20130417 for : Next folder list
            //MP_LOG << "[MP] CATreeModel::fetchNextFolder(): Directory tree has been fully fetched!" << LOG_ENDL;
            //m_bIsTreeFullyFetched = true;
            //m_pControllerAudio->setMP3ListReadState(m_bIsTreeFullyFetched);//added by yongkyun.lee 20130413 for : NO CR MP3 List 
              //{ added by yongkyun.lee 20130419 for : NO CR
			  // modified by aettie 20130812 for ITS 183630
              //if(tempItem->itemName()=="Root")
              if(tempItem->getRootFolder()) // modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
              {
                  //{ modified by yongkyun.lee 2013-09-07 for :   NOCR MP3 List 
                  m_tmp_entryTree = -1;
                  m_OldBlockNo = -1;
                  m_OldFolderNo = -1;
                  //} modified by yongkyun.lee 2013-09-07 
                  m_bIsTreeFullyFetched = true;
                  m_pControllerAudio->setMP3ListReadState(m_bIsTreeFullyFetched);//added by yongkyun.lee 20130413 for : NO CR MP3 List 
                  return;
              }
              //} added by yongkyun.lee 20130419 
            checkParentFolder(tempItem->parent());
            //} modified by yongkyun.lee 20130417 
         }
    }
}
//{ added by yongkyun.lee 20130411 


//{ added by yongkyun.lee 20130417 for : Next folder list
void CATreeModel::checkParentFolder( CATreeItem *pItem )
{
    int i=0;

    //{ added by yongkyun.lee 20130419 for : NO CR
    if(pItem == NULL)// root->parent() 
        return;
    //} added by yongkyun.lee 20130419 
    
    for(i=0; i< pItem->childCount() ;i++ )
    {
         CATreeItem *child = pItem->getChildByIndex( i );
         
        if(child->isFolder() &&  !child->isFolderFetched() )
         {
            //child->setFolderCmd(FOLDER_CMD_REQ);
            requestChunk( child->itemId(), 0 );
            break;
         }
    } 
	// modified by aettie 20130812 for ITS 183630  
    if((pItem->getRootFolder()) && i==pItem->childCount()) // modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    {
            //{ modified by yongkyun.lee 2013-09-07 for :   NOCR MP3 List 
            m_tmp_entryTree = -1;
            m_OldBlockNo = -1;
            m_OldFolderNo = -1;
            //} modified by yongkyun.lee 2013-09-07 
            m_bIsTreeFullyFetched = true;
            m_pControllerAudio->setMP3ListReadState(m_bIsTreeFullyFetched);//added by yongkyun.lee 20130413 for : NO CR MP3 List 
        return;
        }
    if ( i == pItem->childCount() )
    {
        checkParentFolder(pItem->parent());
    }
}
//} added by yongkyun.lee 20130417 


void CATreeModel::requestChunk( int folder, int block )
{
    //MP_LOG << "[MP] CATreeModel::requestChunk(): folder = " <<  folder << " block = " << block << LOG_ENDL;

    if ( m_pControllerAudio && !m_bIsRequestInProgress )
    {
        m_pControllerAudio->sendFolderChunkRequestOnDec( folder, block );
        m_bIsRequestInProgress = true;
    }
}

//{ added by yongkyun.lee 20130520 for : MP3CD Current item List
CATreeItem* CATreeModel::findItemModel()
{
    DeckController *deckController = m_pControllerAudio->GetControllers().GetDeckController();
    if (!deckController || !deckController->GetAudioInstance())
    {
        return NULL;
    }

    CAStatusResponse caStatus;
    int result = deckController->GetAudioInstance()->GetCAStatus(&caStatus);
    CATreeItem * PlayItem = m_pRootFolder->getCurrPlayFolderById( m_pRootFolder , caStatus );

    if(PlayItem != NULL )
    {
        m_pCurrentItem = PlayItem;
    }
    else
    {
        return NULL;
    }
    CATreeItem *item = m_pCurrentItem;
// modified by aettie 20130812 for ITS 183630
    if ( item->isFolder() && !item->getRootFolder() )// modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    {
        m_pCurrentItem = item;
        setPlaybackIcon(true);
    }
    else if (item->getRootFolder()) // modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    {
        m_pCurrentItem = m_pRootFolder;
        setPlaybackIcon(true);
    }
    
    return PlayItem;

}

int CATreeModel::ParentItem(CATreeItem *Item , int *val)
{
// modified by aettie 20130812 for ITS 183630
    if( Item->getRootFolder() ) // modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
        return 0;
    *val=+1;
    return ParentItem(Item->parent(), val);
    
}

int CATreeModel::getTreeDep()
{
    int retval = 0;
    DeckController *deckController = m_pControllerAudio->GetControllers().GetDeckController();
    if (!deckController || !deckController->GetAudioInstance())
    {
        return NULL;
    }

    CAStatusResponse caStatus;
    int result = deckController->GetAudioInstance()->GetCAStatus(&caStatus);
    CATreeItem * PlayItem = m_pRootFolder->getCurrPlayFolderById( m_pRootFolder , caStatus );

    retval = ParentItem(PlayItem, &retval);
    
    return retval;
}
//} added by yongkyun.lee 20130520 

// removed by cychoi 2014.04.06 for ITS 233500

void CATreeModel::refreshCurrentModel()
{
    //MP_LOG << "[MP] CATreeModel::refreshCurrentModel() is called" << LOG_ENDL;

    if ( !m_pCurrentItem )
        return;

    //MP_LOG << "[MP] CATreeModel::refreshCurrentModel(): modelCount =" << m_currentModel.count() << LOG_ENDL;
    //MP_LOG << "[MP] CATreeModel::refreshCurrentModel(): m_pCurrentItem->childCount() =" << m_pCurrentItem->childCount() << LOG_ENDL;

    /* Return if no dynamic additions to current model happened */
    if ( m_pCurrentItem->childCount() <= m_currentModel.count() )
        return;

    /* Chunks for root can contain both files and folders in random order.
     * So, we need to do total refresh for root always.
     * Subfolders can contain files only, just append added items to the end */
    if ( m_pCurrentItem->isRoot() )
    {
        clearModelData();
        fillModelData();
    }
    else
    {
        //MP_LOG << "ROWS START" << LOG_ENDL;
        beginInsertRows( QModelIndex(), m_currentModel.count(), m_pCurrentItem->childCount()-1 );

        for ( int i = m_currentModel.count(); i < m_pCurrentItem->childCount(); i++ )
        {
            m_currentModel.append( m_pCurrentItem->getChildByIndex( i ) );
        }
        endInsertRows();
        //MP_LOG << "ROWS END" << LOG_ENDL;
        // { modified by dongjin 2012.11.16 for CR14033
        //setPlaybackIcon(); // added by eugeny.novikov 2012.10.18 for CR 14542
        setPlaybackIcon(true);
        // } modified by dongjin
    }
    emit signalUpdateCountInfo(); // added by eugeny.novikov 2012.10.18 for CR 14542
    emit setCurrentPlayingItemPosition(); // // { modified by wonseok.heo for NOCR focus at list 2013.12.10
}

// { added by wonseok.heo 2013.08.15 for ITS 184020
void CATreeModel::listExit()
{
    m_PrevItem = NULL;
}
// } added by wonseok.heo 2013.08.15 for ITS 184020

void CATreeModel::fillModelData()
{
    //MP_LOG << "CATreeModel::fillModelData() is called" << LOG_ENDL;

    if ( !m_pCurrentItem )
        return;

    MP_LOG << "ROWS START : parent name =" << m_pCurrentItem->itemName() << LOG_ENDL;
    beginInsertRows( QModelIndex(), 0, m_pCurrentItem->childCount()-1 );



    for ( int i = 0; i < m_pCurrentItem->childCount(); i++ )
    {
        m_currentModel.append( m_pCurrentItem->getChildByIndex( i ) );
    }
    endInsertRows();
    MP_LOG << "ROWS END" << LOG_ENDL;
    
    // { modified by dongjin 2012.11.16 for CR14033
    //setPlaybackIcon(); // added by eugeny.novikov 2012.10.18 for CR 14542
    setPlaybackIcon(true);
    // } modified by dongjin
    emit setCurrentPlayingItemPosition(); // // { modified by wonseok.heo for NOCR focus at list 2013.12.10

    //{ modified by wonseok.heo for ITS 197007 2013.10.19
    if(isCurrItemRoot())
    {
        emit updateTextItemView("", 1);
    }
    else
    {
        emit updateTextItemView(m_pCurrentItem->itemName(), 1);
    }
    //} modified by wonseok.heo for ITS 197007 2013.10.19

}

void CATreeModel::clearModelData()
{
    MP_LOG << "CATreeModel::clearModelData() is called" << LOG_ENDL;
    if ( !m_currentModel.isEmpty() )
    {
        MP_LOG << "ROWS START" << LOG_ENDL;
        emit clearSelection(); // added by wonseok.heo CR list item selection crash 2013.08.14
        beginRemoveRows( QModelIndex(), 0, m_currentModel.count()-1 );
        m_currentModel.clear();
        endRemoveRows();
        MP_LOG << "ROWS END" << LOG_ENDL;
    }
}

// Called when disk has been ejected
void CATreeModel::releaseModelData()
{
    clearModelData();
    delete m_pRootFolder;
    m_pRootFolder = NULL;
    m_pCurrentItem = NULL;
    m_bIsTreeFullyFetched = false;
    m_bIsRequestInProgress = false;
    m_pendingFolderRequest = -1;
    //{ modified by yongkyun.lee 2013-09-07 for :   NOCR MP3 List 
    m_tmp_entryTree = -1;
    m_OldBlockNo = -1;
    m_OldFolderNo = -1;
    //} modified by yongkyun.lee 2013-09-07 
    m_pControllerAudio->setMP3ListReadState(m_bIsTreeFullyFetched);//{ added by yongkyun.lee 20130413 for : NO CR MP3 List 
}

QString CATreeModel::onItemClick( int index )
{
    MP_LOG << "[MP] CATreeModel::onItemClick(): index =" << index << LOG_ENDL;

    if ( !m_pCurrentItem )
        return "";

    CATreeItem *item = m_pCurrentItem->getChildByIndex( index );

    if ( !item )
        return "";

    if ( item->isFolder() )
    {
        clearModelData();
        m_pCurrentItem = item;
        fillModelData();

        MP_LOG << "[MP] CATreeModel::onItemClick(): m_bIsRequestInProgress =" << m_bIsRequestInProgress << LOG_ENDL;
        if ( !m_pCurrentItem->isFolderFetched() )
        {
            if ( m_bIsRequestInProgress )
            {
                MP_LOG << "[MP] CATreeModel::onItemClick(): Set pending request =" << m_pendingFolderRequest << LOG_ENDL;
                m_pendingFolderRequest = m_pCurrentItem->itemId();
            }
            else
            {
                requestChunk( m_pCurrentItem->itemId(), m_pCurrentItem->fetchedBlockCount() );
            }
        }

        if ( item->itemName() != "" )
        {
            return " > " + item->itemName();
        }
    }
    else if ( m_pControllerAudio )
    {
        // { modified by eugeny.novikov 2012.10.18 for CR 14542
        MP_LOG << "fileId =" << item->itemId() << LOG_ENDL;
        MP_LOG << "fileName =" << item->itemName() << LOG_ENDL;
        // } modified by eugeny.novikov
        m_pControllerAudio->GetDeviceHistory(MP::DISC)->ContentPlayed = MP::FOLDER; // added by junam 2012.09.24 for CR12977
        //{modified by aettie.ji 2013.01.31 for ISV 68623
        //m_pControllerAudio->setCurrentIndex( item->itemId() - 1 );
        if ((m_pControllerAudio->GetDeviceHistory(MP::DISC)->Index) != (item->itemId() - 1))
        {
            //m_pControllerAudio->sendClearTpMessage(); // commented by cychoi 2013.08.17 for No clear TP message from HMC request // added by wspark 2013.04.17
            m_pControllerAudio->setCurrentIndex( item->itemId() - 1 );
            m_pControllerAudio->clearMP3Info( ); // modified by yongkyun.lee 2013-08-22 for :31.6 Ver. Smoke Test 
            m_pControllerAudio->updateMP3Info(item->itemName(), item->itemId() - 1); // modified by cychoi 2013.08.17 for HMC request - Cluster display = OSD display // added by wonseok.heo 2013.05.19
        }
        // { added by wonseok.heo for ITS 190637 2013.09.21 Scan Off on same item click
        else
        {
            if(m_pControllerAudio->getScanMode() != SCANOFF)
            {
                m_pControllerAudio->setScanMode(SCANOFF);
            }
        }
        // } added by wonseok.heo for ITS 190637 2013.09.21
		//}modified by aettie.ji 2013.01.31 for ISV 68623
        // { removed by junam 2012.12.03 for CR16296
        // { added by eugeny.novikov 2012.10.18 for CR 14542
        //        clearModelData();
        //        m_pCurrentItem = m_pRootFolder;
        //        fillModelData();
        // } added by eugeny.novikov
        // } removed by junam

        emit closeList();

    }

    return "";
}

// { moved by cychoi 2014.03.07 for ITS 228996 CDDA list flickering on item selection
// { Add by Naeo 2012.07.09 for CR 10970
//QString CATreeModel::onItemClickCD( int index )
//{
//    MP_LOG << LOG_ENDL; // added by eugeny.novikov 2012.10.18 for CR 14542

//    if(m_pControllerAudio->GetCurrentTrackIndex() != index)//{ added by yongkyun.lee 20130403 for : ISV 78542
//    {
//        m_pControllerAudio->setScanMode(SCANOFF);// added by yongkyun.lee 20130702 for : ITS 177744
//        m_pControllerAudio->setCurrentIndex( index );
//    }
    //{ added by cychoi 2013.10.11 for ITS 195149 Scan Off on same item click
//    else
//    {
//        if(m_pControllerAudio->getScanMode() != SCANOFF)
//        {
//            m_pControllerAudio->setScanMode(SCANOFF);
//        }
//    }
    //} added by cychoi 2013.10.11
//    emit closeList();

//    return "";
//}
// } Add by Naeo
// } moved by cychoi 2014.03.07

bool CATreeModel::openParent()
{
    MP_LOG << "[MP] CATreeModel::openParent() is called" << LOG_ENDL;

    if ( m_pCurrentItem && !m_pCurrentItem->isRoot() )
    {
        clearModelData();
        m_PrevItem = m_pCurrentItem;// added by yongkyun.lee 20130613 for : ITS 83292
        m_pCurrentItem = m_pCurrentItem->parent();
        fillModelData();

        if ( !m_pCurrentItem->isFolderFetched() )
        {
            if ( m_bIsRequestInProgress )
            {
                MP_LOG << "[MP] CATreeModel::openParent(): Set pending request =" << m_pendingFolderRequest << LOG_ENDL;
                m_pendingFolderRequest = m_pCurrentItem->itemId();
            }
            else
            {
                requestChunk( m_pCurrentItem->itemId(), m_pCurrentItem->fetchedBlockCount() );
            }
        }
        return true;
    }

    return false;
}

//{ added by yongkyun.lee 20130520 for : MP3CD Current item List
bool CATreeModel::isCurrItemRoot()
{
    //{ modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    if( m_pCurrentItem->getRootFolder() )
    {
        return true;
    }
    //} modified by yongkyun.lee 2013-10-02 
    return false;
}
//} added by yongkyun.lee 20130520 

void CATreeModel::clear()
{
    /* Empty function to avoid error in currentLoaderTab.item.itemViewListModel.clear(); */
}

// { added by junam 2012.12.03 for CR16296
void CATreeModel::reset()
{
    if(m_pCurrentItem)
    {
        clearModelData();
        //{ added by yongkyun.lee 20130520 for : MP3CD Current item List
        //m_pCurrentItem = m_pRootFolder;
        m_pCurrentItem = findItemModel();
        //} added by yongkyun.lee 20130520 
        fillModelData();
    }
}
// } added by junam

//{ added by yongkyun.lee 20130417 for : Next folder list
//QList<TuneTrackInfo> trackInfoList;

bool CATreeModel::addTrackInfoListFromCATree( CATreeItem *subItem, int dep)
{
    MP_LOG << LOG_ENDL;

    // { modified by cychoi 2013-07-15 for Prevent fix
    //MP_LOG << "[MP] CATreeModel::addTrackInfoListFromCATree() Item ="<< dep <<":" <<   subItem->itemName() << LOG_ENDL;
    // } modified by cychoi 2013-07-15

    if ( subItem )
    {
        // { modified by cychoi 2013-07-15 for Prevent fix
        //MP_LOG << "[MP] CATreeModel::addTrackInfoListFromCATree() Item ="<< dep <<":" <<   subItem->itemName() << LOG_ENDL;
        // } modified by cychoi 2013-07-15

        QString folderName = subItem->itemName(); //added by wonseok.heo 2013.06.12 for ITS 146611

        TuneTrackInfo trackInfo;
        CATreeItem *ChildItem;
        CATreeItem *folderChildItem;
        int ChildsCount = subItem->childCount();


        for ( int i = 0; i < ChildsCount; i++ )
        {
            ChildItem = subItem->getChildByIndex(i);
            if ( ChildItem->isFolder() )
            {
                addTrackInfoListFromCATree(ChildItem , dep++ );
            }
            else
            {
                trackInfo.name = ChildItem->itemName();
                //trackInfo.folder = "ROOT FOLDER";//added by junam 2012.12.14 for CR16128
                //trackInfo.folder = "/..";//modified by aettie.ji 2013.01.31 for ISV 70943
                trackInfo.folder = folderName; //added by wonseok.heo 2013.06.12 for ITS 146611
                trackInfo.id = ChildItem->itemId();
                trackInfoList << trackInfo;
            }
            //MP_LOG << "[MP] CATreeModel::addTrackInfoListFromCATree():2 itemName =" << dep <<":"<<   ChildItem->itemName() << LOG_ENDL;

        }
        //MP_LOG << "[MP] CATreeModel::addTrackInfoListFromCATree(): trackInfoList.count() = " <<  trackInfoList.count() << LOG_ENDL;
      }
        return true;
}
//} added by yongkyun.lee 20130417

QList<TuneTrackInfo>
CATreeModel::getTrackInfoListFromCATree()
{
    //{ modified by yongkyun.lee 20130417 for : Next folder list
    //QList<TuneTrackInfo> trackInfoList;
    trackInfoList.clear();
    int dep=0;
    //{ modified by yongkyun.lee 20130417 

    //MP_LOG << "[MP] CATreeModel::getTrackInfoListFromCATree() is called" << LOG_ENDL;

    if ( m_pRootFolder )
    {
        TuneTrackInfo trackInfo;
        CATreeItem *rootChildItem;
        CATreeItem *folderChildItem;
        int rootChildsCount = m_pRootFolder->childCount();

        //MP_LOG << "[MP] CATreeModel::getTrackInfoListFromCATree(): rootChildsCount = " <<  rootChildsCount << LOG_ENDL;

        for ( int i = 0; i < rootChildsCount; i++ )
        {
            rootChildItem = m_pRootFolder->getChildByIndex(i);

            if ( rootChildItem->isFolder() )
            {
                addTrackInfoListFromCATree(rootChildItem,dep);
            }
            else
            {
                trackInfo.name = rootChildItem->itemName();
                QString rootFolder = QApplication::translate("main",QT_TR_NOOP(  "STR_MEDIA_ROOT_FOLDER"));
                trackInfo.folder = rootFolder;
                trackInfo.id = rootChildItem->itemId();
                trackInfoList << trackInfo;
            }
        }
        //MP_LOG << "[MP] CATreeModel::getTrackInfoListFromCATree(): trackInfoList.count() = " <<  trackInfoList.count() << LOG_ENDL;
    }

    return trackInfoList;

}

// { added by eugeny.novikov 2012.10.18 for CR 14542
void CATreeModel::onMp3CdMediaInfoChanged()
{
    if (m_pControllerAudio->playerMode() == MP::DISC && m_pControllerAudio->getDiscType() == MP::MP3_CD)
    {
        MP_LOG << LOG_ENDL;
        m_PrevItem = NULL;// added by yongkyun.lee 20130613 for : ITS 83292
        //{ added by yongkyun.lee 20130513 for : ITS 168193 / 167858 / 167821
        //setPlaybackIcon(false); // modified by dongjin 2012.11.16 for CR14033
        setPlaybackIcon(true); 
        //} added by yongkyun.lee 20130513 
    }
}
//{  added by wonseok.heo for NOCR List Focus of Disc 2013.12.30
bool CATreeModel::SubFolderCheck( CATreeItem *SubItem ,int idxID, int trackIndex )
{
    MP_LOG << "Folder name =" << SubItem->childs.count() << LOG_ENDL;
    for (int i = 0; i < SubItem->childs.count(); i++)
    {
        if(SubItem->childs.at(i)->isFolder())
        {
            if(SubFolderCheck( SubItem->childs.at(i) ,idxID, trackIndex))
            {
                MP_LOG << "sub Folder name =" <<SubItem->childs.at(i)->itemName() << LOG_ENDL;
                return true;
            }           
        }
        if ((SubItem->childs.at(i)->itemId() == trackIndex && !SubItem->childs.at(i)->isFolder()) )
        {
            MP_LOG << "sub Folder name =" <<SubItem->childs.at(i)->itemName() << "sub Folder itemId =" <<SubItem->childs.at(i)->itemId() << LOG_ENDL;
            return true;
        }
    }
    return false;
}

bool CATreeModel::FolderCheck(QString fName , int idxID, int trackIndex )
{
    MP_LOG << "Folder name =" <<idxID << trackIndex  << " ================== >>" << LOG_ENDL;
    MP_LOG << "Tree ID count=" <<m_currentModel.count()  << LOG_ENDL;

    for (int i = 0; i < m_currentModel.count(); i++)
    {
        if(m_currentModel.at(i)->isFolder())
        {
            if(SubFolderCheck(m_currentModel.at(i) ,idxID, trackIndex))
            {
                MP_LOG << "Folder name =" << m_currentModel.at(i)->itemName() << LOG_ENDL;
                m_currentPlayingItem = i;
                m_currentFocusItem = i ;
                emit currentPlayingItemChanged();
                emit setCurrentPlayingItemPosition();
                return true;
            }           
        }
        if ((m_currentModel.at(i)->itemId() == trackIndex && !m_currentModel.at(i)->isFolder()) )
        {
                MP_LOG << "File name =" << m_currentModel.at(i)->itemName() << LOG_ENDL;
                m_currentPlayingItem = i;
                m_currentFocusItem = i;
                emit currentPlayingItemChanged();
                emit setCurrentPlayingItemPosition();
                // removed by cychoi 2014.04.06 for ITS 233500
            return true;
        }
    }
    m_currentPlayingItem = -1;
    emit currentPlayingItemChanged();
    return false;
} // } added by wonseok.heo for NOCR List Focus of Disc 2013.12.30

//{ added by yongkyun.lee 20130415 for : Play icon on folder.
bool CATreeModel::isCurrentFolder(CATreeItem *PlayItem ,CATreeItem *ListItem )
{
    if(PlayItem == NULL || ListItem ==  NULL )
        return false;

    if(!PlayItem->isFolder() || !ListItem->isFolder() || ListItem->isRoot())
        return false;
    
    if(PlayItem->itemId() == ListItem->itemId())
        return true;
    else
        return isCurrentFolder(PlayItem->parent(), ListItem);
 }
//} added by yongkyun.lee 20130415 
void CATreeModel::setPlaybackIcon(bool isFocusChange) // modified by dongjin 2012.11.16 for CR14033
{
    m_currentPlayingItem = -1;
    DeckController *deckController = m_pControllerAudio->GetControllers().GetDeckController();

    if (!deckController || !deckController->GetAudioInstance())
    {
        MP_LOG << "DeckController or IAudioInterface is NULL! Return." << LOG_ENDL;
        emit currentPlayingItemChanged();
        return;
    }

    CAStatusResponse caStatus;
    int result = deckController->GetAudioInstance()->GetCAStatus(&caStatus);
    
    if(m_pRootFolder == NULL || result == 0 )  
        return;

    CATreeItem * PlayItem = m_pRootFolder->getCurrPlayById( m_pRootFolder , caStatus );
        
    m_currentFocusItem = -1;
    // { // { modified by wonseok.heo for NOCR focus at list 2013.12.10
//    if( m_PrevItem != NULL  )
//    {
//        for (int i = 0; i < m_currentModel.count(); i++)
//        {
//            if ((m_currentModel.at(i)->itemId()   == m_PrevItem->itemId()   )&&
//                (m_currentModel.at(i)->isFolder() == m_PrevItem->isFolder() )  )
//            {
//                m_currentFocusItem = i;
//                break;
//            }
//        }
//    }
    // } // { modified by wonseok.heo for NOCR focus at list 2013.12.10
        
    if(PlayItem==NULL) 
        return;

     for (int i = 0; i < m_currentModel.count(); i++)
     {
         if ((m_currentModel.at(i)->itemId() == caStatus.currentFileNo && !m_currentModel.at(i)->isFolder()) ||
             isCurrentFolder(PlayItem ,m_currentModel.at(i)) )
         {
             //{ added by yongkyun.lee 20130613 for : ITS 83292
             // { // { modified by wonseok.heo for NOCR focus at list 2013.12.10
//             if(m_PrevItem == PlayItem || m_PrevItem == NULL )
//                 m_currentFocusItem = i;
             // } // { modified by wonseok.heo for NOCR focus at list 2013.12.10
             //} added by yongkyun.lee 20130613 
             m_currentPlayingItem = i;
             break;
         }
     }

     //{ added by yongkyun.lee 2013-11-19 for : ISV 94814
     // { // { modified by wonseok.heo for NOCR focus at list 2013.12.10
//     if(m_currentFocusItem == -1 && m_currentPlayingItem != -1 )
//        m_currentFocusItem = m_currentPlayingItem;
     // } // { modified by wonseok.heo for NOCR focus at list 2013.12.10
     //} added by yongkyun.lee 2013-11-19 for : ISV 94814

     //{ modified by yongkyun.lee 2013-09-30 for : NO CR - MP3CD List crash
     //if(m_currentFocusItem == -1)
     //   m_currentFocusItem = 0;
     //} modified by yongkyun.lee 2013-09-30 
     emit currentPlayingItemChanged();
     // { modified by cychoi 2014.04.06 for ITS 233500 // { modified by wonseok.heo for NOCR focus at list 2013.12.10
     if (m_pControllerAudio->getCurrentAutoPlayItem() == -1)
     {
         emit setCurrentPlayingItemPosition();
     }
     // } modified by cychoi 2014.04.06 for ITS 233500 // { modified by wonseok.heo for NOCR focus at list 2013.12.10

}

QString CATreeModel::getListFolders() //HWS
{
    QString diskInfoString = "DiskInfo";
    return diskInfoString;

}
QString CATreeModel::getCompareFolders(int currentIndex, bool isListFocused)//HWS
{
    QString countInfoString;
    MP_LOG << "currentIndex" << currentIndex << LOG_ENDL;
    MP_LOG << "isListFocused" << isListFocused << LOG_ENDL;

    //{added by junam 2013.07.18 for null check
    if(m_pCurrentItem == NULL)
    {
        return countInfoString;
    }
    //}added by junam

    if (isListFocused &&
        currentIndex >= 0 && currentIndex < m_currentModel.count())
    {
        if (m_currentModel.at(currentIndex)->isFolder())
        {
            countInfoString = "icofolder2"; //QString::number(currentIndex + 1) + QString("/") +
                            //  QString::number(m_pCurrentItem->folderCount());
        }
    }
    else
    {
         if (m_pCurrentItem->fileCount() == 0 && m_pCurrentItem->folderCount() > 0)
        {
            countInfoString = "icofolder2";

        }
    }

    return countInfoString;

}


//HWS
QString CATreeModel::getListStatus()
{
    QString diskInfoString = "DiskInfo";
    return diskInfoString;
}

//HWS
QString CATreeModel::getCountInfoFirst(int currentIndex, bool isListFocused)
{
    QString countInfoString = "";
    MP_LOG << "currentIndex" << currentIndex << LOG_ENDL;
    MP_LOG << "isListFocused" << isListFocused << LOG_ENDL;

    //{added by junam 2013.07.18 for null check
    if(m_pCurrentItem == NULL)
    {
        return QString("0");
    }
    //}added by junam

    if (isListFocused &&
        currentIndex >= 0 && currentIndex < m_currentModel.count())
    {
        if (m_currentModel.at(currentIndex)->isFolder())
        {
            //countInfoString = QString::number(currentIndex + 1) + QString("/") +
                            //  QString::number(m_pCurrentItem->folderCount());
        }
        else
        {
            // countInfoString = QString::number(currentIndex + 1 - m_pCurrentItem->folderCount()) +
                            //   QString("/") + QString::number(m_pCurrentItem->fileCount());
        }
    }
    else
    {
        if (m_pCurrentItem->fileCount() > 0 && m_pCurrentItem->folderCount() > 0)
        {
                if (m_pCurrentItem->folderCount() == 1)
                {
                    if (m_pCurrentItem->fileCount() == 1)
                    {
                        countInfoString = QString::number(m_pCurrentItem->fileCount()) ;//  + QString(" File/") +
                        //QString::number(m_pCurrentItem->folderCount()) + QString(" 1Folder");
                    }
                    else
                    {
                        countInfoString = QString::number(m_pCurrentItem->fileCount());  // + QString(" Files/") +
                       // QString::number(m_pCurrentItem->folderCount()) + QString(" 2Folders");
                    }
                }
                else
                {
                    if (m_pCurrentItem->fileCount() == 1)
                    {
                        countInfoString = QString::number(m_pCurrentItem->fileCount());//   + QString(" File/") +
                        //   QString::number(m_pCurrentItem->folderCount()) + QString("3 Folders");
                    }
                    else
                    {
                        countInfoString = QString::number(m_pCurrentItem->fileCount());//   + QString(" Files/") +
                        //   QString::number(m_pCurrentItem->folderCount()) + QString(" 4Folders");
                    }
                }
        }
        else if (m_pCurrentItem->fileCount() == 0 && m_pCurrentItem->folderCount() > 0)
        {
                if (m_pCurrentItem->folderCount() == 1)
                {
                    //countInfoString = QString("Total ") + QString::number(m_pCurrentItem->folderCount()) + QString(" 5Folder");
                }
                else
                {
                    //countInfoString = QString("Total ") + QString::number(m_pCurrentItem->folderCount()) + QString("6 Folders");
                }
        }
        else if (m_pCurrentItem->fileCount() > 0 && m_pCurrentItem->folderCount() == 0)
        {
                if (m_pCurrentItem->fileCount() == 1)
                {
                //countInfoString = QString("Total ") + QString::number(m_pCurrentItem->fileCount()) + QString(" 7File");
                }
                else
                {
                //countInfoString = QString("Total ") + QString::number(m_pCurrentItem->fileCount()) + QString("8 Files");
                }
        }
        else
        {
            //countInfoString = QString("0");
        }
    }

    return countInfoString;
}
//}modified by HWS 2013.03.24 for New UX
QString CATreeModel::getCountInfo(int currentIndex, bool isListFocused)
{
    QString countInfoString = "0/0";// added by yongkyun.lee 20130411 for :  MP3 CD Folder List
    MP_LOG << "currentIndex" << currentIndex << LOG_ENDL;
    MP_LOG << "isListFocused" << isListFocused << LOG_ENDL;
    //{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
    if(m_pCurrentItem == NULL)
    {
        return countInfoString;
    }
    //} added by yongkyun.lee 20130411

    if (isListFocused &&
        currentIndex >= 0 && currentIndex < m_currentModel.count())
    {
        if (m_currentModel.at(currentIndex)->isFolder())
        {
            // { added by wonseok.heo for ITS M.E 211068 2013.11.26
            if(m_pControllerAudio->GetEngineMain().middleEast())
                countInfoString = QString::number(m_pCurrentItem->folderCount()) + QString("/") +
                                  QString::number(currentIndex + 1);
            else // } added by wonseok.heo for ITS M.E 211068 2013.11.26
                countInfoString = QString::number(currentIndex + 1) + QString("/") +
                                  QString::number(m_pCurrentItem->folderCount());
        }
        else
        {
            // { added by wonseok.heo for ITS M.E 211068 2013.11.26
            if(m_pControllerAudio->GetEngineMain().middleEast())
                countInfoString = QString::number(m_pCurrentItem->fileCount()) +
                                  QString("/") + QString::number(currentIndex + 1 - m_pCurrentItem->folderCount());
            else // } added by wonseok.heo for ITS M.E 211068 2013.11.26
                countInfoString = QString::number(currentIndex + 1 - m_pCurrentItem->folderCount()) +
                                  QString("/") + QString::number(m_pCurrentItem->fileCount());
        }
    }
    else
    {
        if (m_pCurrentItem->fileCount() > 0 && m_pCurrentItem->folderCount() > 0)
        {
        //{modified by aettie.ji 2013.02.06 No CR
            if (m_pCurrentItem->folderCount() == 1)
            {
                //{modified by HWS 2013.03.24 for New UX
                if (m_pCurrentItem->fileCount() == 1)
                {
                    countInfoString = QString::number(m_pCurrentItem->folderCount());// + QString(" 1Folder");
                }
                else
                {
                    countInfoString = QString::number(m_pCurrentItem->folderCount()); // + QString(" 2Folders");
                }
            }
            else
            {
                if (m_pCurrentItem->fileCount() == 1)
                {
                    countInfoString = QString::number(m_pCurrentItem->folderCount()); // + QString("3 Folders");
                }
                else
                {
                    countInfoString = QString::number(m_pCurrentItem->folderCount());// + QString(" 4Folders");
                }
            }
        }
        else if (m_pCurrentItem->fileCount() == 0 && m_pCurrentItem->folderCount() > 0)
        {
            if (m_pCurrentItem->folderCount() == 1)
            {
                countInfoString =  QString::number(m_pCurrentItem->folderCount()); // + QString(" 5Folder");
            }
            else
            {
                countInfoString =  QString::number(m_pCurrentItem->folderCount()); // + QString("6 Folders");
            }
        }
        else if (m_pCurrentItem->fileCount() > 0 && m_pCurrentItem->folderCount() == 0)
        {
            if (m_pCurrentItem->fileCount() == 1)
            {
                countInfoString =  QString::number(m_pCurrentItem->fileCount()); // + QString(" 7File");
            }
            else
            {
                countInfoString =  QString::number(m_pCurrentItem->fileCount());// + QString("8 Files");
            }
        //{modified by aettie.ji 2013.02.06 No CR
        }
        else
        {
            emit listDefaultFocus();// modified by yongkyun.lee 2013-08-19 for : NO CR MP3 list 0 - default focus
            countInfoString = QString("0");
        }
//}modified by HWS 2013.03.24 for New UX
    }

    return countInfoString;
}

// } added by eugeny.novikov

// added by yongkyun.lee 20130603 for : Folder name 
QString CATreeModel::getCurrentFolderName( ) 
{
    if(!m_bIsTreeFullyFetched)
        return "";
    DeckController *deckController = m_pControllerAudio->GetControllers().GetDeckController();
    if (!deckController || !deckController->GetAudioInstance())
    {
        MP_LOG << "DeckController or IAudioInterface is NULL! Return ""." << LOG_ENDL;
        return "";
    }
    CAStatusResponse caStatus;
    int result = deckController->GetAudioInstance()->GetCAStatus(&caStatus);
    
    CATreeItem * PlayItem = m_pRootFolder->getCurrPlayFolderById( m_pRootFolder , caStatus );
    if(PlayItem == NULL )
        return "";
		//added by aettie 20130812 for ITS 183630
    if( PlayItem->getRootFolder() ) // modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    {
        //modified by aettie 2013 08 05 for ISV NA 85625
        //        return "/.."; 
		// modified by aettie 20130812 for ITS 183630   
        //return "Root";    
        QString rootFolder = QApplication::translate("main",QT_TR_NOOP(  "STR_MEDIA_ROOT_FOLDER"));
        return rootFolder;
    }
    if( PlayItem->isFolder())
        return PlayItem->itemName();

    return "";
}
//} added by yongkyun.lee 20130603 

