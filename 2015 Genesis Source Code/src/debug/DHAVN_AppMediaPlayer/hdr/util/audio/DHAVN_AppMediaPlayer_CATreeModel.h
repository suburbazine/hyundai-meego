#ifndef DHAVN_MEDIAPLAYER_CATREEMODEL_H
#define DHAVN_MEDIAPLAYER_CATREEMODEL_H

#include <QtCore>
#include <QCoreApplication> //added by aettie 20130812 for ITS 183630
#include "DHAVN_AppMediaPlayer_Shared.h"
#include "mediaplayer_def.h"

enum ITEM_TYPE
{
    ITEM_UNDEFINED, // 0
    ITEM_FOLDER,    // 1
    ITEM_FILE_MP3,  // 2
    ITEM_FILE_WMA   // 3
};

typedef struct
{
    QString name;
    QString folder;  //added by junam 2012.12.14 for CR16128
    int id;
} TuneTrackInfo;

class AppMediaPlayer_Controller_Audio;

class CATreeItem
{
public:
    CATreeItem();
    ~CATreeItem();

    int itemId() const           { return m_itemId; }
    QString itemName() const     { return m_itemName; }
    bool isFolder() const        { return m_bIsFolder; }
    bool isFolderFetched() const  { return ( m_folderBlockCount == m_fetchedBlockCount ); }
    int fetchedBlockCount() const { return m_fetchedBlockCount; }
    bool isRoot() const          { return ( m_pParent == NULL ); }
    //{ modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    void setRootFolder(bool val) { m_isRoot = val; }
    bool getRootFolder() const   { return m_isRoot; }
    //} modified by yongkyun.lee 2013-10-02 
    int childCount() const       { return childs.count(); }
    // { added by eugeny.novikov 2012.10.18 for CR 14542
    int folderCount() const      { return m_currChildFolderCount; }
    int fileCount() const        { return (childs.count() - m_currChildFolderCount); }
    // } added by eugeny.novikov
    CATreeItem* parent() const   { return m_pParent; }
    int folderBlockCount() const { return m_folderBlockCount; }// added by yongkyun.lee 20130411 for :  MP3 CD Folder List

    void setItemId( int id ) { m_itemId = id; }
    void setIsFolder( bool isFolder ) { m_bIsFolder = isFolder; }
    void setParent( CATreeItem *parent ) { m_pParent = parent; }
    void setItemName( QString str ) { m_itemName = str; }

    CATreeItem* getChildByIndex( int index ) const;
    CATreeItem* getChildFolderById( int id ) const;
    //{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
    CATreeItem* getSubFolderById(CATreeItem* subItem, int id ) ;
    CATreeItem* getRootFolderById(CATreeItem* rootItem , int id ) ;
    //{ added by yongkyun.lee 20130513 for : ITS 168193 / 167858 / 167821
    CATreeItem* getSubPlayById(CATreeItem* subItem, CAStatusResponse id ) ;
    CATreeItem* getCurrPlayById(CATreeItem* rootItem , CAStatusResponse id ) ;
    //{ added by yongkyun.lee 20130520 for : MP3CD Current item List
    CATreeItem* getCurrPlayFolderById(CATreeItem* rootItem , CAStatusResponse id ) ;
    CATreeItem* getSubPlayFolderById(CATreeItem* subItem, CAStatusResponse id ) ;
    //} added by yongkyun.lee 20130520 
    //} added by yongkyun.lee 20130513 
    //{ deleted by yongkyun.lee 20130417 for : Next folder list
    //void setFolderCmd( bool val ){subFoldercmd = val;}
    //bool getFolderCmd(  ){subFoldercmd ;}
    //} deleted by yongkyun.lee 20130417  
    //} added by yongkyun.lee 20130411
    void append( DvdcDirTree *dirTreeChunk );
    QList<CATreeItem*> childs; // added by wonseok.heo for NOCR List Focus of Disc 2013.12.30

private:
    QString charToUnicode( char *pIn, int bUnicode );

private:
    int m_itemId;
    QString m_itemName;
    bool m_bIsFolder;
    int m_folderBlockCount;
    int m_fetchedBlockCount;
    int m_childItemCount;
    int m_currChildFolderCount; // added by eugeny.novikov 2012.10.18 for CR 14542
    //bool subFoldercmd;// deleted by yongkyun.lee 20130417 for : Next folder list
    CATreeItem *m_pParent;
    bool m_isRoot;// modified by yongkyun.lee 2013-10-02 for : nocr - list Title 
    //QList<CATreeItem*> childs; // removed by wonseok.heo for NOCR List Focus of Disc 2013.12.30
};


class CATreeModel : public QAbstractListModel
{
    Q_OBJECT

public:

    CATreeModel( AppMediaPlayer_Controller_Audio *appControllerAudio, QObject *parent = 0 );
    ~CATreeModel();

    QVariant data( const QModelIndex &index, int role ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;

    void appendChunkToModel( DvdcDirTree *dirTreeChunk );
    void releaseModelData();
    QList<TuneTrackInfo> getTrackInfoListFromCATree();
    //{ added by yongkyun.lee 20130417 for : Next folder list    
    bool addTrackInfoListFromCATree( CATreeItem *subItem, int dep);
    void checkParentFolder( CATreeItem *pItem );
    //} added by yongkyun.lee 20130417 
    Q_INVOKABLE QString getCountInfo(int currentIndex, bool isListFocused); // added by eugeny.novikov 2012.10.18 for CR 14542
//{added by HWS 2013.03.24 for New UX
	Q_INVOKABLE QString getCurrentFolderName(); // added by Yongkyun.lee 2013.06.03
	Q_INVOKABLE QString getCountInfoFirst(int currentIndex, bool isListFocused); //HWS
    Q_INVOKABLE QString getListStatus(); //HWS
    Q_INVOKABLE QString getListFolders(); //HWS
    Q_INVOKABLE QString getCompareFolders(int currentIndex, bool isListFocused); //HWS
//}added by HWS 2013.03.24 for New UX
	Q_INVOKABLE QString onItemClick( int index );
    //Q_INVOKABLE QString onItemClickCD( int index );    // moved by cychoi 2014.03.07 for ITS 228996 CDDA list flickering on item selection // Add by Naeo 2012.07.09 for CR 10970
    Q_INVOKABLE bool openParent();
    Q_INVOKABLE void clear();
    Q_INVOKABLE void reset(); //added by junam 2012.12.03 for CR16296
    //{ added by yongkyun.lee 20130520 for : MP3CD Current item List
    Q_INVOKABLE int getTreeDep();
    Q_INVOKABLE bool isCurrItemRoot();
    CATreeItem* findItemModel();
    int ParentItem(CATreeItem *Item , int *val);
    //} added by yongkyun.lee 20130520 
   // Q_INVOKABLE void centerCurrentPlayingItemPosition(int currentIndex, bool isListFocused, int listFocusIdx); //deleted by aettie.ji 2013.02.26 for ISV 62706

// { added by eugeny.novikov 2012.10.18 for CR 14542
    Q_PROPERTY(int currentPlayingItem READ getCurrentPlayingItem NOTIFY currentPlayingItemChanged);
    int getCurrentPlayingItem() { return m_currentPlayingItem; }
	
    //{ added by yongkyun.lee 20130613 for : ITS 83292
    Q_PROPERTY(int currentFocusItem READ getCurrentFocusItem NOTIFY currentFocusItemChanged);
    int getCurrentFocusItem() { return m_currentFocusItem; }
    //} added by yongkyun.lee 20130613 
	
    bool isCurrentFolder(CATreeItem *PlayItem ,CATreeItem *ListItem );// added by yongkyun.lee 20130415 for : Play icon on folder.
    // removed by cychoi 2014.04.06 for ITS 233500
    void listExit(); // added by wonseok.heo 2013.08.15 for ITS 184020
    bool FolderCheck(QString fName , int idxID, int trackIndex ); //added by wonseok.heo for NOCR List Focus of Disc 2013.12.30
    bool SubFolderCheck( CATreeItem *SubItem ,int idxID, int trackIndex ); //added by wonseok.heo for NOCR List Focus of Disc 2013.12.30

public slots:
    void onMp3CdMediaInfoChanged();
// } added by eugeny.novikov

Q_SIGNALS:
    void closeList();
    // { added by eugeny.novikov 2012.10.18 for CR 14542
    void currentPlayingItemChanged();
    void currentFocusItemChanged();// added by yongkyun.lee 20130613 for : ITS 83292
    void setCurrentPlayingItemPosition();
    void signalUpdateCountInfo();
    // } added by eugeny.novikov
    void listDefaultFocus();// modified by yongkyun.lee 2013-08-19 for : NO CR MP3 list 0 - default focus

    void updateTextItemView(QString title, int historyStack);//changed by junam 2013.09.06 for ITS_KOR_188332

    void clearSelection(); // added by wonseok.heo CR list item selection crash 2013.08.14

private:
    void requestNextChunk( int folder, int block );
    //{ added by yongkyun.lee 20130411 for :  MP3 CD Folder List
    //void fetchNextFolder();
    void fetchNextFolder(CATreeItem *tempItem);
    void sendSubFolderRequest();
    //{ added by yongkyun.lee 20130411
    void sendPendingRequest();
    void refreshCurrentModel();
    void clearModelData();
    void fillModelData();
    void requestChunk( int folder, int block );
    // { modified by dongjin 2012.11.16 for CR14033
    //void setPlaybackIcon(); // added by eugeny.novikov 2012.10.18 for CR 14542
    void setPlaybackIcon(bool isFocusChange);
    // } modified by dongjin

private:
    Q_DISABLE_COPY( CATreeModel );

    enum CATreeModelRoles
    {
        NameRole = Qt::UserRole + 1,
        imageRole,
        urlRole,
//        activeRole, // removed by eugeny.novikov 2012.10.18 for CR 14542
        checkBoxMarked,
        selectableRole,
        imageVisibleRole //added by junam 2012.10.12 for CR14040
    };
    QList<TuneTrackInfo> trackInfoList;//{ added by yongkyun.lee 20130417 for : Next folder list
    bool m_bIsRequestInProgress;
    int  m_pendingFolderRequest;
    bool m_bIsTreeFullyFetched;

    AppMediaPlayer_Controller_Audio *m_pControllerAudio;

    CATreeItem *m_pRootFolder;
    CATreeItem *m_pCurrentItem;
    CATreeItem *m_PrevItem;   // added by yongkyun.lee 20130613 for : ITS 83292
    CATreeItem  m_touchItem; //modified by wonseok.heo fo new UX 2013.07.10
    QList<CATreeItem*> m_currentModel;

    //{ modified by yongkyun.lee 2013-09-07 for : NOCR MP3 List 
    int m_tmp_entryTree;
    int m_OldBlockNo   ;
    int m_OldFolderNo  ;
    //} modified by yongkyun.lee 2013-09-07 
    // removed by cychoi 2014.04.06 for ITS 233500
    int m_currentFocusItem;   // added by yongkyun.lee 20130613 for : ITS 83292
    int m_currentPlayingItem; // added by eugeny.novikov 2012.10.18 for CR 14542
};

#endif // DHAVN_MEDIAPLAYER_CATREEMODEL_H
