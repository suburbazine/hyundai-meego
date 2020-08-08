#ifndef DHAVN_APPMEDIAPLAYER_PATHVIEWMODEL_H
#define DHAVN_APPMEDIAPLAYER_PATHVIEWMODEL_H

#include <QtCore>
#include <QMutex>

#define NONAME_ALBUM "Unknown"

//{changed by junam 2013.05.18 for ISV_KR83983
//#define LOADING_COVER QString("/app/share/images/music/coverflow_default_n.png")
#define LOADING_COVER QString("/app/share/images/music/coverflow_default_s.png")
//}chagned by junam
#define DEFALUT_COVER QString("/app/share/images/music/coverflow_default_n.png")


class PathViewModel : public QAbstractListModel
{
    Q_OBJECT

public:

    PathViewModel( QMap<QString, QString> & imageMap, QObject *parent = 0 ); //changed by junam 2013.10.28 for 2nd depth coverart

    QVariant data( const QModelIndex &index, int role ) const;
    bool setData (const QModelIndex &index, const QVariant &value, int role );
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;

    //void fillModelData( const QStringList& albums, const QStringList& artists, const QStringList& filePaths); //removed by junam 2013.11.06 for coverflow update
    void fillAlbumCover(const QStringList& albums, const QStringList& covers);

    bool getOnDemandCoverArt( QStringList& albums, QStringList& artists, QStringList& files, int fetchCount); //add fetchCount by junam 2012.10.25 for CR14451
	
    //void appendModelData( const QStringList& albums, const QStringList& artists, const QStringList& filePaths); //removed by junam 2013.11.06 for coverflow update
    void replaceModelData(QVector<QStringList> &fileList); //added by junam 2013.11.06 for coverflow update
    void checkMediaArt(QVector<QStringList> &fileList); //added by junam for coverart loading performance

    Q_INVOKABLE void clearModelData();

    Q_INVOKABLE QString getAlbumName( int index ) const;
    //Q_INVOKABLE QString getAlbumCover( int index ) const; //removed by junam 2013.10.28 for 2nd depth coverart
    Q_INVOKABLE QString getArtistName( int index ) const; //added by aettie 2013.03.11 for ISV 61689 
    Q_INVOKABLE QString getFilePath( int index ) const; //added by junam 2013.11.18 for coverflow click

    // { added by junam for CR12458
    Q_INVOKABLE void onComponentCompleted(int idx);
    Q_INVOKABLE void onComponentDestruction(int idx);
    // } added by junam

    int getAlbumIndex (QString albumName) const;

    //static wchar_t getFirstChar(QString name); //removed by junam 2013.11.06 for coverflow update
    bool isCoverFileSizeValid(QString); // added by wspark 2012.09.20 for CR13633
    // { added by junam 2012.11.12 for CR15447
    void requestCoverEnable(bool bEnable){ if(bEnable) m_requestCoverEnalbeTimer.start(5000);\
                                           else m_requestCoverEnalbeTimer.stop();}
    // } added by junam
    Q_INVOKABLE bool isBusy() const { return m_requestCoverEnalbeTimer.isActive();}//added by junam 2013.03.22 for ISV76946
    void stopRequestCoverTimer(); // added by Dmitry 02.05.13

    //{removed by junam 2013.11.06 for coverflow update
    //bool firstEntry() const {return m_bFirstEntry;}
    //void firstEntrySetter( bool bFirst ){ m_bFirstEntry = bFirst;}
    //Q_PROPERTY (bool firstEntry READ firstEntry WRITE firstEntrySetter);
    //}removed by junam

    bool isEmpty() const { return m_albumName.isEmpty();} //added by junam 2013.08.18 for empty flowview

signals:
    void requestCover();//added by junam 2012.10.25 for CR14451
    void setHighlightCover (int coverIndex, bool isScroll); //added by junam 2013.03.18 for coverflow refresh
private slots:
    void requestCoverSlot(); // added by junam 2012.11.12 for CR15447
private:
    Q_DISABLE_COPY( PathViewModel );

    enum PathViewModelRoles
    {
        AlbumNameRole = Qt::UserRole + 1,
        CoverURIRole,
        //CoverIndexRole, //removed by junam 2013.03.14 for cover art image loading
        AlbumAlphaRole,
        AlbumPlayStateRole,
        FolderNameRole,
        FileNameRole,
        firstCharRole,  //added by junam for CR9771
        artistRole //added by aettie 2013.03.11 for ISV 61689 
    };

    QStringList         m_albumName;
    QStringList         m_artistName;
    QStringList         m_filePath;
    //QStringList         m_coverURI; //removed by junam 2013.10.28 for 2nd depth coverart
    //QList<bool>       m_isRequested;//removed by junam 2012.11.02 for unusing code.
    QList<wchar_t>      m_firstChar;//added by junam for CR9771
    //bool                m_isNonameAlbumExist;//removed by junam 2013.06.19 for not using code

    QList<int>          m_indexODCA;    
    QTimer              m_requestCoverTimer;//added by junam 2012.10.25 for CR14451
    // { added by junam 2012.11.12 for CR15447
    QTimer              m_requestCoverTimeOver;
    QTimer              m_requestCoverEnalbeTimer;
    // } added by junam
    QMutex              m_tMutex; // added by kihyung 2013.2.12 for MEDIA HK Crash
    //bool                    m_bFirstEntry; //removed by junam 2013.11.06 for coverflow update
    QMap<QString, QString> & m_imageMap; //added by junam 2013.10.28 for 2nd depth coverart
    wchar_t getIndexChar(QString title);
};

#endif // DHAVN_APPMEDIAPLAYER_PATHVIEWMODEL_H
