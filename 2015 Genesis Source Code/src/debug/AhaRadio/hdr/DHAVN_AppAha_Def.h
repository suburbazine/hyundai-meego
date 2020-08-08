#ifndef DHAVN_APPAHA_DEF_H
#define DHAVN_APPAHA_DEF_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include "ahapredefinedvalues.h"

const QString c_IPhone = "IPhone";
const QString c_SmartPhone = "Smart";

class TrackInfo : public QObject
{
   Q_OBJECT

   Q_PROPERTY( qint16  duration READ getDuration )
   Q_PROPERTY( qint16  elapsed READ getElapsed )
   Q_PROPERTY( quint8  ratingStatus READ getRating )
   Q_PROPERTY( bool    allowRating READ getAllowRating )
   Q_PROPERTY( bool    allowLike READ getAllowLike )
   Q_PROPERTY( bool    allowDislike READ getAllowDislike )
   Q_PROPERTY( bool    allowSkip READ getAllowSkip )
   Q_PROPERTY( bool    allowSkipBack READ getAllowSkipBack )
   Q_PROPERTY( bool    allowTimeShift READ getAllowTimeShift )
   Q_PROPERTY( bool    allowCall READ getAllowCall )
   Q_PROPERTY( bool    allowNavigate READ getAllowNavigate )
   Q_PROPERTY( bool    allowBookmark READ getAllowBookmark )
   Q_PROPERTY( bool    allowExplain READ getAllowExplain )
   Q_PROPERTY( QString title READ getTitle )
   Q_PROPERTY( QString artist READ getArtist )
   Q_PROPERTY( QString album READ getAlbum )
   Q_PROPERTY( QString albumArtUrl READ getAlbumArtUrl )
   Q_PROPERTY( QString station READ getStationName )
   Q_PROPERTY( QString stationArt READ getStationArt WRITE  setStationArt)
   Q_PROPERTY( quint32 stationToken READ getStationToken)

public:
   void SetTrackProperties(const AhaTSTrackInfo& inTrackInfo)
   {
    nDuration = inTrackInfo.nDuration;
    nElapsed = inTrackInfo.nElapsed;
    nRating = inTrackInfo.nRating;
    bAllowRating = inTrackInfo.bAllowRating;
    bAllowLike = inTrackInfo.bAllowLike;
    bAllowDislike = inTrackInfo.bAllowDislike;
    bAllowSkip = inTrackInfo.bAllowSkip;
    bAllowSkipBack = inTrackInfo.bAllowSkipBack;
    bAllowNavigate = inTrackInfo.bAllowNavigate;
    bAllowCall = inTrackInfo.bAllowCall;
    bAllowTimeShift = inTrackInfo.bAllowTimeShift;
    bAllowBookmark = inTrackInfo.bAllowBookmark;
    bAllowExplain = inTrackInfo.bAllowExplain;
    Title = inTrackInfo.Title;
    Album = inTrackInfo.Album;
    Artist = inTrackInfo.Artist;
    AlbumArtUrl = inTrackInfo.AlbumArtUrl;
    StationName = inTrackInfo.CurrentStation;
    StationToken = inTrackInfo.StationToken;
   }

   //hsryu_0326_like_dislike
   void ClearTrackProperty()
   {
       nRating = false;
       bAllowRating = false;
       bAllowLike = false;
       bAllowDislike = false;
       bAllowSkip = false;
       bAllowSkipBack = false;
       bAllowNavigate = false;
       bAllowTimeShift = false;
       bAllowBookmark = false;
       bAllowExplain = false;
/*       Title.clear();
       Artist.clear();
       Album.clear();
       AlbumArtUrl.clear();
       StationName.clear();*/
       StationToken = 0;
   }

   void ClearStringData()
   {
      Title.clear();
      Artist.clear();
      Album.clear();
      AlbumArtUrl.clear();
      //StationName.clear();
   }

   qint16 getDuration()const {return nDuration;}
   qint16 getElapsed()const {return nElapsed;}
   quint8 getRating()const {return nRating;}
   bool getAllowRating()const {return bAllowRating;}
   bool getAllowLike()const {return bAllowLike;}
   bool getAllowDislike()const {return bAllowDislike;}
   bool getAllowSkip()const {return bAllowSkip;}
   bool getAllowSkipBack()const {return bAllowSkipBack;}
   bool getAllowTimeShift() const {return bAllowTimeShift;}
   bool getAllowCall() const {return bAllowCall;}
   bool getAllowNavigate() const {return bAllowNavigate;}
   bool getAllowBookmark()const {return bAllowBookmark;}
   bool getAllowExplain()const {return bAllowExplain;}
   QString getTitle()const {return Title;}
   QString getArtist()const {return Artist;}
   QString getAlbum()const {return Album;}
   //hsryu_0228 remove LF,space at front,end
   void setTitle(QString inTitle){ Title.clear(); Title = inTitle;}
   void setArtist(QString inArtist){ Artist.clear(); Artist = inArtist;}
   void setAlbum(QString inAlbum){ Album.clear(); Album = inAlbum;}
   void setStationName(QString inStationName){ StationName.clear(); StationName = inStationName; }

   QString getAlbumArtUrl()const {return AlbumArtUrl;}
   QString getStationName()const {return StationName;}
   QString getStationArt(){return StationArt;}
   void setStationArt(QString inStationArt){ StationArt = inStationArt;}
   quint32 getStationToken(){return StationToken;}

private:
   /** Track info properties */
   qint16  nDuration;   /** Duration of current station (Seconds)*/
   qint16  nElapsed;    /** Current position of track (Seconds)*/
   quint8  nRating;     /** 1 for Thumb up, 2 for Thumb Down, 0 for no rating*/
   bool    bAllowRating; /** Is it allowed to rate this track */
   bool    bAllowLike;  /** Is it allowed to like this track */
   bool    bAllowDislike;   /** Is it allowed to dislike this track */
   bool    bAllowSkip;   /** Is it allowed to skip this track */
   bool    bAllowSkipBack; /** is skipback to previous track allow or not*/
   bool    bAllowTimeShift; /** is time shift allow or not*/
   bool    bAllowNavigate;
   bool    bAllowCall;
   bool    bAllowBookmark;/** Is it allowed to bookmark this track */
   bool    bAllowExplain; /** Is it allowed to explain this track */
   QString Title;         /** Track title */
   QString Artist;        /** Artist for track */
   QString Album ;        /** Track Album    */
   QString AlbumArtUrl ;  /** The url of album art where received image will be saved */
   QString StationName ;  /** The active station name for the current track */
   QString StationArt; /** The active station art */
   quint32 StationToken; /** The active station token */
};

#endif // DHAVN_APPAHA_DEF_H
