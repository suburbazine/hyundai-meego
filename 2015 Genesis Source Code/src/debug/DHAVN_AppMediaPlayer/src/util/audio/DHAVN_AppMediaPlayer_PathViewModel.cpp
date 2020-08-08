#include <DHAVN_TrackerAbstractor.h>
#include "util/audio/DHAVN_AppMediaPlayer_PathViewModel.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h" //added by junam 08.22 for CR9771
#include "util/audio/DHAVN_AppMediaPlayer_Utils.h" //added by junam 2013.11.06 for coverflow update

const int REQUEST_RANGE_MAX  = 15;
const int REQUEST_RANGE_MIN  = 4;
const int REQUEST_COUNT_MAX = 9;// chage 6->9 by junam 2012.07.26 for CR 11105

PathViewModel::PathViewModel( QMap<QString, QString> & imageMap, QObject *parent ) : //changed by junam 2013.10.28 for 2nd depth coverart
    m_imageMap(imageMap), //added by junam 2013.10.28 for 2nd depth coverart
    m_requestCoverTimer(this),// added by junam 2012.10.25 for CR14451
    //{ added by junam 2012.11.12 for CR15447
    m_requestCoverTimeOver(this),
    m_requestCoverEnalbeTimer(this)
    // } added by junam
{
    Q_UNUSED( parent );

    QHash<int, QByteArray> roles = roleNames();
    roles.insert(AlbumNameRole, QByteArray("albumTitle"));
    roles.insert(CoverURIRole, QByteArray("image"));
    //roles.insert(CoverIndexRole, QByteArray("index"));//removed by junam 2013.03.14 for cover art image loading
    roles.insert(AlbumAlphaRole, QByteArray("alphaIndex"));
    roles.insert(AlbumPlayStateRole, QByteArray("playState"));
    roles.insert(FolderNameRole, QByteArray("folderTitle"));
    roles.insert(FileNameRole, QByteArray("fileTitle"));
    roles.insert( firstCharRole, QByteArray( "firstChar" ) ); //added by junam 08.22 for CR9771
    roles.insert( artistRole, QByteArray( "artist" ) ); //added by aettie 2013.03.11 for ISV 61689 

    setRoleNames(roles);

    m_requestCoverTimer.setSingleShot(true);
    //{ changed by junam 2012.11.12 for CR15447
    //connect(&m_requestCoverTimer, SIGNAL(timeout()), this, SIGNAL(requestCover()));
    connect(&m_requestCoverTimer, SIGNAL(timeout()), this, SLOT(requestCoverSlot()));
    m_requestCoverTimeOver.setSingleShot(true);
    m_requestCoverEnalbeTimer.setSingleShot(true);
    //} changed by junam

    //m_bFirstEntry = true;//removed by junam 2013.11.06 for coverflow update
}

QVariant PathViewModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() || index.row() > rowCount() )
        return QVariant();

    // MP_LOG << index << role << LOG_ENDL;
    
    switch ( role )
    {
    case AlbumNameRole:
        return QVariant::fromValue(m_albumName[index.row()]);

    case CoverURIRole:
        //{changed by junam 2013.10.28 for 2nd depth coverart
        //Q_ASSERT( index.row() < m_coverURI.size());
        //if(m_coverURI[index.row()].isEmpty())
        //    return QVariant::fromValue(LOADING_COVER);
        //else
        //    return QVariant::fromValue(m_coverURI[index.row()]);
        if(index.row()<m_filePath.size())
        {
            return m_imageMap.value(m_filePath.at(index.row()));
        }
        //}changed by junam
    case AlbumPlayStateRole:
        return QVariant::fromValue(0);

    case FolderNameRole:
        return QVariant::fromValue(QString(""));

        //{changed by junam 2013.11.16 for coverflow click
    case FileNameRole:
        if(index.row() >= m_filePath.size())
            return QVariant::fromValue(QString(""));

        return QVariant::fromValue(m_filePath.at(index.row()));
        //}changed by junam

    // { change by junam for 2012.08.23 CR9771
    case firstCharRole:
        if( index.row() < m_firstChar.size())
        {
            wchar_t wChar = 0;
            wChar = m_firstChar.at(index.row());

            if( index.row() < 0)
                return QVariant::fromValue(QString("")); //changed by juanm for remove warning

            if( index.row() == 0 || wChar != m_firstChar.at(index.row()-1))
                return QVariant::fromValue(QString::fromWCharArray(&wChar , 1));
        }
        return QVariant::fromValue(QString("")); //changed by juanm for remove warning
        // } change by junam
    case artistRole:
        return QVariant::fromValue(m_artistName[index.row()]); //added by aettie 2013.03.11 for ISV 61689 
    default:
        break;  // modified by sungha.choi 2013.07.15 for static test
    }
    return QVariant();  //added by junam for CR9771
}

bool
PathViewModel::setData (const QModelIndex &index, const QVariant &value, int role )
{
    if (!index.isValid() || index.row() > rowCount() )
    {
        MP_LOG<<"[ERROR]valid="<<index.isValid()<<", row="<<index.row()<<", rowCount="<<rowCount() << LOG_ENDL;
        return false;
    }

    // MP_LOG << index << role << LOG_ENDL;

    switch (role)
    {
    case CoverURIRole:
    {
        //{changed by junam 2013.10.28 for 2nd depth coverart
        //Q_ASSERT(index.row() < m_coverURI.size());
        //m_coverURI[index.row()] = value.toString();
        if(index.row()<m_filePath.size())
        {
            QString imageSource = value.toString();
            m_imageMap.insert(m_filePath.at(index.row()), imageSource);
            emit dataChanged(index, index);
        }
        //}changed by junam
        break;
    }
    default:
        MP_LOG<<"[ERROR] Un handled role" << LOG_ENDL;
        return false;
    }
    return true;
}

int PathViewModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return m_albumName.count();
}
//{removed by junam 2013.11.06 for coverflow update
//void PathViewModel::fillModelData(const QStringList& albums, const QStringList& artists, const QStringList& filePaths)
//{
//    // { modified by kihyung 2013.1.8
//    MP_LOG << "is called before m_albumName.count = " << m_albumName.count() << LOG_ENDL;

//    if(albums.count() <= 0) {
//        MP_LOG << "albums.count() is 0. returned" << LOG_ENDL;
//        return;
//    }


//    beginInsertRows(QModelIndex(), 0, albums.count() - 1); //added by junam for 2012.12.06 flow view focus
//        m_albumName  = albums;
//        m_artistName = artists;
//        m_filePath   = filePaths;

//        //m_coverURI.clear(); //removed by junam 2013.10.28 for 2nd depth coverart
//        //m_isRequested.clear(); // removed by junam 2012.10.25 for CR14451
//        m_firstChar.clear();//added by junam 08.22 for CR9771
//        for (int n = 0; n < albums.count(); n++)
//        {
//            //m_coverURI.append (QString()); //removed by junam 2013.10.28 for 2nd depth coverart
//            //m_isRequested.append(false);// removed by junam 2012.10.25 for CR14451
//            m_firstChar.append( QChar::toUpper((ushort)getFirstChar(albums.at(n).trimmed())));//added by junam 08.22 for CR9771
//        }
//    endInsertRows(); //added by junam for 2012.12.06 flow view focus

//}

//void PathViewModel::appendModelData( const QStringList& albums, const QStringList& artists, const QStringList& filePaths )
//{
//    MP_LOG << " is called before m_albumName.count = " << m_albumName.count() << LOG_ENDL;
//    MP_LOG << " is called albums.count = " << albums.count() << LOG_ENDL;

//    if (m_albumName.count() >  albums.count())
//    {
//        MP_LOG<<"ERROR: Cannot append.." << LOG_ENDL;
//        return;
//    }

//    if (m_albumName.count() ==  albums.count())
//    {
//        MP_LOG<<"No need append" << LOG_ENDL;
//        return;
//    }

//    MP_LOG << "ROWS START" << LOG_ENDL;
//    m_tMutex.lock();
//    for ( int idx = 0; idx < albums.count(); idx++)
//    {
//        if ( m_albumName.size() <= idx || m_albumName.at(idx) != albums.at(idx) )
//        {
//            beginInsertRows( QModelIndex(), idx, idx );
//                m_albumName.insert(idx, albums.at(idx));
//                m_artistName.insert( idx, artists.at(idx));
//                m_filePath.insert( idx, filePaths.at(idx));
//                //m_coverURI.insert( idx, QString()); //removed by junam 2013.10.28 for 2nd depth coverart
//                //m_isRequested.insert( idx, false);// removed by junam 2012.10.25 for CR14451
//                m_firstChar.insert( idx, QChar::toUpper((ushort)getFirstChar(albums.at(idx).trimmed())));//added by junam 08.22 for CR9771
//            endInsertRows();
//        }
//    }
//    m_tMutex.unlock();
//    MP_LOG << "ROWS END" << LOG_ENDL;

//    MP_LOG << " is called m_albumName.count = " << m_albumName.count() << LOG_ENDL;
//}
//} removed by junam

//{added by junam 2013.11.06 for coverflow update
void PathViewModel::replaceModelData(QVector<QStringList> &fileList)
{
    int idx;

    checkMediaArt(fileList); //added by junam for coverart loading performance

    if(fileList.isEmpty())
    {
        MP_LOG<<"Empty size = "<<fileList.size() << LOG_ENDL;
    }
    else if(m_albumName.size() < fileList.size())
    {
        MP_LOG<<"Increase = "<<m_albumName.size()<<" -> "<<fileList.size() << LOG_ENDL;
        for(idx = 0; idx < m_albumName.size(); idx++)
        {
            m_albumName.replace(idx, fileList.at(idx).at(0));
            m_artistName.replace(idx, fileList.at(idx).at(1));
            m_filePath.replace(idx, fileList.at(idx).at(2));
            // modified by jaehwan to fix Pre G5 Test 'μ' (Mu) grouped to 'M' issue.
            m_firstChar.replace(idx, getIndexChar(fileList.at(idx).at(0)));
        }
        emit dataChanged( this->index(0), this->index(m_albumName.size()-1));

        beginInsertRows(QModelIndex(), m_albumName.size(), fileList.count() - 1);
        for( ; idx < fileList.size(); idx++)
        {
            m_albumName.append(fileList.at(idx).at(0));
            m_artistName.append(fileList.at(idx).at(1));
            m_filePath.append(fileList.at(idx).at(2));
            m_firstChar.append(getIndexChar(fileList.at(idx).at(0)));
        }
        endInsertRows();
    }
    else if(m_albumName.size() > fileList.size())
    {
        MP_LOG<<"Decrease = "<<m_albumName.size()<<" -> "<<fileList.size() << LOG_ENDL;
        for(idx = 0; idx < fileList.size(); idx++)
        {
            m_albumName.replace(idx, fileList.at(idx).at(0));
            m_artistName.replace(idx, fileList.at(idx).at(1));
            m_filePath.replace(idx, fileList.at(idx).at(2));
            m_firstChar.replace(idx, getIndexChar(fileList.at(idx).at(0)));
        }
        emit dataChanged(this->index(0), this->index(fileList.count()-1));

        beginRemoveRows(QModelIndex(), fileList.count(), m_albumName.size() - 1);
        for( ; idx < m_albumName.size(); idx++)
        {
            m_albumName.removeLast();
            m_artistName.removeLast();
            m_filePath.removeLast();
            m_firstChar.removeLast();
        }
        endRemoveRows();
    }
    else
    {
        MP_LOG<<"Same size = "<<m_albumName.size()<<" and "<<fileList.size() << LOG_ENDL;
        for(idx = 0; idx < fileList.size(); idx++)
        {
            m_albumName.replace(idx, fileList.at(idx).at(0));
            m_artistName.replace(idx, fileList.at(idx).at(1));
            m_filePath.replace(idx, fileList.at(idx).at(2));
            m_firstChar.replace(idx, getIndexChar(fileList.at(idx).at(0)));
        }
        emit dataChanged(this->index(0), this->index(fileList.count()-1));
    }

    m_requestCoverTimer.start(1000);
}
//}changed by junam

//{added by junam for coverart loading performance
void PathViewModel::checkMediaArt(QVector<QStringList> &fileList)
{
    MP_LOG<<" enter = "<<fileList.size() << LOG_ENDL;
    QString imageSource;
    foreach( QStringList list, fileList)
    {
         if( list.size() >= 3 && !m_imageMap.contains(list.at(2)))
         {
             imageSource = CTrackerAbstractor::GetMediaArt( list.at(1), list.at(0));
             if(!imageSource.isEmpty() && QFile(QString(imageSource).remove( "file://" )).size() > 0)
             {
                 m_imageMap.insert( list.at(2), imageSource);
             }
         }
    }
    MP_LOG<<" leave " << LOG_ENDL;
}
//}added by junam

// { added by junam for CR12458
void
PathViewModel::onComponentCompleted(int idx)
{
    if ( ( idx < 0 ) || ( idx >= m_albumName.count()))
    {
        MP_LOG<<"invalid range : "<< m_albumName.count() << LOG_ENDL;
        return;
    }
    //{changed by junam 2013.10.28 for 2nd depth coverart
    QString itemSource = m_filePath.at(idx);
    //if (m_coverURI.at(idx).isEmpty())
    if(!itemSource.isEmpty() && !m_imageMap.contains(itemSource))
        m_indexODCA.append(idx);
    //}changed by junam
    m_requestCoverTimer.start(1000);
    // } changed by junam
}

void
PathViewModel::onComponentDestruction(int idx)
{
    //MP_LOG << idx << m_indexODCA.size() << LOG_ENDL;
    int removeIndex = m_indexODCA.indexOf(idx);
    if(removeIndex >= 0)
    {
        m_indexODCA.removeAt(removeIndex);
    }
}
// } added by junam 

//{ added by junam 2012.11.12 for CR15447
void
PathViewModel::requestCoverSlot(void)
{
    if(m_requestCoverTimeOver.isActive() ||
            m_requestCoverEnalbeTimer.isActive())
    {
        m_requestCoverTimer.start(2000);
    }
    else
    {
        MP_LOG << "ROWS" << LOG_ENDL;
        emit requestCover();
    }
}
// } added by junam
 
// { added by junam 2012.10.25 for CR14451
bool
PathViewModel::getOnDemandCoverArt( QStringList& albums, QStringList& artists, QStringList& files, int fetchCount)
{
    //{ added by junam 2012.11.12 for CR15447
    if(m_requestCoverTimeOver.isActive() ||
            m_requestCoverEnalbeTimer.isActive())
    {
        MP_LOG<<"Album is changing. let's skip.." << LOG_ENDL;
        return false;
    }
    // } added by junam

    MP_LOG << LOG_ENDL;

    while(!m_indexODCA.isEmpty())
    {
        int idx = m_indexODCA.at(0);
        m_indexODCA.removeFirst();
        //{changed by junam 2013.11.06 for coverflow update
        if(idx >= m_albumName.size() || idx < 0 )
            continue;

        //if(m_coverURI[idx].isEmpty() == true)
        if(!m_imageMap.contains(m_filePath.at(idx))) //}changed by junam
        {
            albums.append(m_albumName.at(idx));
            artists.append(m_artistName.at(idx));
            files.append(m_filePath.at(idx));

            if(albums.size() >= fetchCount)
                break;
        }
    }

    //{ changed by junam 2012.11.12 for CR15447
    //return (albums.isEmpty() == false);
    if(albums.isEmpty())
        return false;

    m_requestCoverTimeOver.start(5000);    
    return true;
    // } changed by junam
}
// } added by junam


// { added by junam 2012.10.25 for CR14451
void
PathViewModel::fillAlbumCover(const QStringList& albums, const QStringList& covers)
{
    int indexCover=0, idx;

    // MP_LOG << LOG_ENDL;

    if(albums.size() != covers.size())
    {
        MP_HIGH<<"size is differ : album = "<< albums.size() <<" cover = "<<covers.size() << LOG_ENDL;
        m_requestCoverTimer.start(100);
        return;
    }

    m_requestCoverTimeOver.stop(); // changed by junam 2012.11.12 for CR15447

    foreach(QString album, albums)
    {
        idx = m_albumName.indexOf(album);        

        if(idx < 0)
        {
            MP_LOG<<"ERROR: Cannot find such album: "<<album << LOG_ENDL;
        }
        else if ( covers.at(indexCover).compare ("No CoverArt Found") == 0)
        {
            MP_LOG<<"skip["<<idx <<"]  = "<<covers.at(indexCover) << LOG_ENDL;
        }
        else if( false == isCoverFileSizeValid((QString)covers.at(indexCover)) )
        {
            MP_LOG<<"invalid["<<idx<<"]="<<covers.at(indexCover) << LOG_ENDL;
            //m_coverURI[idx] = ""/*DEFALUT_COVER*/; //removed by junam 2013.10.28 for 2nd depth coverart
        }
        else
        {
            MP_LOG<<"add["<<idx<<"]="<<covers.at(indexCover) << LOG_ENDL;
            setData( index( idx, 0), QVariant::fromValue( covers.at(indexCover) ), CoverURIRole);
        }

        indexCover++;
    }

    m_requestCoverTimer.start(100);

}
// } added by junam

void PathViewModel::clearModelData()
{
    //m_bFirstEntry = true; //removed by junam 2013.11.06 for coverflow update
    MP_LOG << "start" << LOG_ENDL;

    if(m_albumName.isEmpty()) {
        MP_LOG << "ROWS m_albumName.isEmpty() RETURN" << LOG_ENDL;
        return;
    }

    /*
    MP_LOG << "==============================" << LOG_ENDL;
    MP_LOG << "m_albumName.size() :" << m_albumName.size() << LOG_ENDL;
    MP_LOG << "m_artistName.size():" << m_artistName.size() << LOG_ENDL;
    MP_LOG << "m_filePath.size()  :" << m_filePath.size() << LOG_ENDL;
    MP_LOG << "m_coverURI.size()  :" << m_coverURI.size() << LOG_ENDL;
    MP_LOG << "m_firstChar.size() :" << m_firstChar.size() << LOG_ENDL;
    MP_LOG << "m_indexODCA.size() :" << m_indexODCA.size() << LOG_ENDL;
    MP_LOG << "==============================" << LOG_ENDL;    
    */

    //{added by junam 2013.03.18 for coverflow refresh
    emit setHighlightCover(-1,true);
    //m_isNonameAlbumExist = false;//removed by junam 2013.06.19 for not using code
    //}added by junam

    MP_LOG << "ROWS START"; // << QThread::currentThreadId() << LOG_ENDL; //removed by sangmin.seol 2014.06.02 NOCR remove QThread Log.
    m_tMutex.lock();
    beginRemoveRows(QModelIndex(), 0, m_albumName.size() - 1);
        m_albumName.clear();
        m_artistName.clear();
        m_filePath.clear();
        //m_coverURI.clear(); //removed by junam 2013.10.28 for 2nd depth coverart
        m_firstChar.clear();//added by junam 08.22 for CR9771
    //endResetModel();
    endRemoveRows();
    
    m_indexODCA.clear();  //added by junam 2012.08.07 for CR 11105
    m_tMutex.unlock();
    MP_LOG << "ROWS END" << LOG_ENDL;

    //m_bFirstEntry = true; //removed by junam 2013.08.18 for empty flowview
}

QString
PathViewModel::getAlbumName( int index ) const
{
    MP_LOG << LOG_ENDL;
    
    QString res;

    if ( ( index >= 0 ) && ( index < m_albumName.count() ) )
       res = m_albumName[ index ];

    return res;
}
//added by aettie 2013.03.11 for ISV 61689 
QString
PathViewModel::getArtistName( int index ) const
{
    MP_LOG << LOG_ENDL;
    
    QString res;

    if ( ( index >= 0 ) && ( index < m_artistName.count() ) )
       res = m_artistName[ index ];

    return res;
}

//{added by junam 2013.11.18 for coverflow click
QString PathViewModel::getFilePath( int index ) const
{
    if ( index >= 0 && index < m_filePath.count() )
       return m_filePath.at(index);

    return QString();
}
//}added by junam

int PathViewModel::getAlbumIndex (QString albumName) const
{
    MP_LOG << LOG_ENDL;

    //{added by junam 2013.11.05 for ITS_NA_206156
    if(albumName.isEmpty())
        return m_albumName.indexOf(QString("Unknown Albums"));
    //}added by junam

    return m_albumName.indexOf(albumName);
}

// { removed by junam 2013.11.06 for coverflow update
//wchar_t PathViewModel::getFirstChar(QString name)
//{
//    static const wchar_t koreanTable[19] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141,
//            0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E };

//    QString encName = AppMediaPlayer_Controller_Audio::encodeString(name);

//    if (encName.isEmpty())
//        return 0;

//    int nKeyCode = encName.at(0).unicode();
//    int UniValue = 0;
//    if(( nKeyCode >= 0x3130 && nKeyCode <= 0x318F) || (nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF ))
//    {
//        if( nKeyCode >= 0x3130 && nKeyCode <= 0x318F)
//            UniValue = nKeyCode - 0x3130;
//        else if (nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF )
//            UniValue = nKeyCode - 0xAC00;

//		// modified by eugene 2012.10.29 for static analysis defect #7793
//		//else
//        //    return nKeyCode;

//        int final_con = UniValue % 28;
//        int initial_con = ( ( UniValue - final_con ) / 28 ) / 21;
//        if (initial_con == 1 || initial_con == 4 || initial_con == 8 ||
//            initial_con == 10 || initial_con == 13)
//        {
//            initial_con--;
//        }
//		if( initial_con < 19)//added by edo.lee 2012.10.29 STATIC TEST
//	        return koreanTable[initial_con];
//    }
//    return nKeyCode;
//}
// } removed by junam

// { added by wspark 2012.09.20 for CR13633
bool PathViewModel::isCoverFileSizeValid(QString coverURI)
{
    MP_LOG << LOG_ENDL;
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

// added by Dmitry 02.05.13
void PathViewModel::stopRequestCoverTimer()
{
    if (m_requestCoverTimer.isActive())
       m_requestCoverTimer.stop();
}
// added by Dmitry 02.05.13

wchar_t PathViewModel::getIndexChar(QString title)
{
    return CTrackerAbstractor::GetIndexChar(title, false, (AppEngineBase::GetLanguage() == LANGUAGE_ZH_CMN));
}
