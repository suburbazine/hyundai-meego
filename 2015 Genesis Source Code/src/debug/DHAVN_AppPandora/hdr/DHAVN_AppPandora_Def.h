#ifndef DHAVN_APPPANDORA_DEF_H
#define DHAVN_APPPANDORA_DEF_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <DHAVN_Pndr_Defs.h>
#include "DHAVN_AppPandoraDeviceListInfo.h"

const QString c_IPhone = "IPhone";
const QString c_SmartPhone = "Smart";


#define STATION_ART_PATH  "/app/data/AppPandora/StationArt/StationArt"
#define STATION_ART_TYPE_JPEG  ".jpeg"
#define STATION_ART_TYPE_PNG  ".png"
#define STATION_ART_TYPE_RGB565  ".bmp"

class TrackInfo : public QObject
{
   Q_OBJECT

   Q_PROPERTY( qint16  duration READ getDuration )
   Q_PROPERTY( qint16  elapsed READ getElapsed )
   Q_PROPERTY( quint8  ratingStatus READ getRating )
   Q_PROPERTY( bool    allowRating READ getAllowRating )
   Q_PROPERTY( bool    sharedStation READ isSharedStation )//modified by yongkyun.lee 2014-08-20 for : ITS 245654
   Q_PROPERTY( bool    allowSkip READ getAllowSkip )
   Q_PROPERTY( bool    allowBookmark READ getAllowBookmark )
   Q_PROPERTY( bool    allowExplain READ getAllowExplain )
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   Q_PROPERTY( bool    isAD READ getIsAD )
   Q_PROPERTY( bool    isTrackBookmarked READ getIsTrackBookmarked )
   Q_PROPERTY( bool    isArtistBookmarked READ getIsArtistBookmarked )
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   Q_PROPERTY( QString title READ getTitle )
   Q_PROPERTY( QString artist READ getArtist )
   Q_PROPERTY( QString album READ getAlbum )
   Q_PROPERTY( QString albumArtUrl READ getAlbumArtUrl )
   Q_PROPERTY( QString station READ getStationName )
   Q_PROPERTY( QString stationArt READ getStationArt WRITE  setStationArt)
   Q_PROPERTY( quint32 stationToken READ getStationToken)

public:
   void SetTrackProperties(const TSTrackInfo& inTrackInfo)
   {
    nDuration = inTrackInfo.nDuration;
    nElapsed = (inTrackInfo.nElapsed >= 0) ?inTrackInfo.nElapsed : 0 ;
    nRating = inTrackInfo.nRating;
    bAllowRating = inTrackInfo.bAllowRating;
    bAllowSkip = inTrackInfo.bAllowSkip;
    bAllowBookmark = inTrackInfo.bAllowBookmark;
    bAllowExplain = inTrackInfo.bAllowExplain;
    //added by jyjeon 2014-04-01 for Pandora Link v3.
    bAd = inTrackInfo.bAd;
    bTrackBookmarked = inTrackInfo.bTrackBookmarked;
    bArtistBookmarked = inTrackInfo.bArtistBookmarked;
    //added by jyjeon 2014-04-01 for Pandora Link v3.
    bShared = inTrackInfo.bShared;
    Title = inTrackInfo.Title;
    Album = inTrackInfo.Album;
    Artist = inTrackInfo.Artist;
    AlbumArtUrl = inTrackInfo.AlbumArtUrl;
    StationName = inTrackInfo.CurrentStation;
    StationToken = inTrackInfo.StationToken;
    AlbumArtLength = inTrackInfo.nAlbumArtLength;
   }

   qint16 getDuration()const {return nDuration;}
   qint16 getElapsed()const {return nElapsed;}
   quint8 getRating()const {return nRating;}
   bool getAllowRating()const {return bAllowRating;}
   bool getAllowSkip()const {return bAllowSkip;}
   bool getAllowBookmark()const {return bAllowBookmark;}
   bool getAllowExplain()const {return bAllowExplain;}
   bool isSharedStation()const{return bShared;}
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   bool getIsAD()const {return bAd;}
   bool getIsTrackBookmarked()const {return bTrackBookmarked;}
   bool getIsArtistBookmarked()const {return bArtistBookmarked;}
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   QString getTitle()const {return Title;}
   QString getArtist()const {return Artist;}
   QString getAlbum()const {return Album;}
   QString getAlbumArtUrl()const {return AlbumArtUrl;}
   QString getStationName()const {return StationName;}
   QString getStationArt(){return StationArt;}
   void setStationArt(QString inStationArt){ StationArt = inStationArt;}
   quint32 getStationToken(){return StationToken;}
   void setElapseTime( const qint16  inElapsed){ nElapsed = inElapsed; }
   void setTrackRating( const quint8  inRating){ nRating = inRating; }
   void setTrackBookmark(const quint8 inBookmark){bTrackBookmarked = inBookmark; } // added by wonseok.heo for ITS 266666

   void setStation(QString inStation){StationName.clear();StationName.append(inStation);}

private:
   /** Track info properties */
   qint16  nDuration;   /** Duration of current station (Seconds)*/
   qint16  nElapsed;    /** Current position of track (Seconds)*/
   quint8  nRating;     /** 1 for Thumb up, 2 for Thumb Down, 0 for no rating*/
   bool    bAllowRating; /** Is it allowed to rate this track */
   bool    bAllowSkip;   /** Is it allowed to skip this track */
   bool    bAllowBookmark;/** Is it allowed to bookmark this track */
   bool    bAllowExplain; /** Is it allowed to explain this track */
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   bool    bAd;       /** Is it AD aduio */
   bool    bTrackBookmarked;       /** Is it track bookmarked */
   bool    bArtistBookmarked;       /** Is it artist bookmarked */
   //added by jyjeon 2014-04-01 for Pandora Link v3.
   bool    bShared;       /** Is it shared station */
   QString Title;         /** Track title */
   QString Artist;        /** Artist for track */
   QString Album ;        /** Track Album    */
   QString AlbumArtUrl ;  /** The url of album art where received image will be saved */
   QString StationName ;  /** The active station name for the current track */
   QString StationArt; /** The active station art */
public:
   quint32 StationToken; /** The active station token */
   qint32  AlbumArtLength;    /**  ALbum art length field */
};

#endif // DHAVN_APPPANDORA_DEF_H
