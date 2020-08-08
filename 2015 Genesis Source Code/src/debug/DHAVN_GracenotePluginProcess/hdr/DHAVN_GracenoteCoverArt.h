#ifndef DHAVN_GRACENOTECOVERART_H
#define DHAVN_GRACENOTECOVERART_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <gn_textid_file_data.h>
#include <gn_textid_lookup.h>
#include <gn_abs_types.h>

#include <DHAVN_LogUtil.h>

// Forward declaration
class QImage;

class CGracenoteCoverArt : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CGracenoteCoverArt(QObject* parent = 0);
    ~CGracenoteCoverArt();


    void RetrieveCoverArtForMP(QStringList metaData);
    void RetrieveMultipleCoverArts();
    void SetAlbumArtistForCA(QStringList albumList, QStringList artistList);
    int GetAlbumListCount();
    void SaveImageToCache(const QImage& source, QString fileName);
	//added by aettie 2013.03.19 for gracenote issue
    void SetMetadataForCA(QStringList mataDataList);
    QStringList GetMetadataForCA();

private:
    bool SetID3Information(QStringList id3List, gn_pfile_data_t &fileData);
    void PerformLookup(gn_pfile_data_t &fileData);

    void RetrieveCoverArt(gn_textid_presult_data_t resultData);

    void InsertCoverArt(QImage &qimage);    

    void LogError(gn_error_t error);
    bool CheckIfCoverArtExists(QStringList list);

    QString encodeString(gn_uchar_t* inputString);
    bool isUtf8(const char * str );

Q_SIGNALS:
    void coverArtRetrieved(bool status, QString imagePath = QString());

private:
    QString m_Album;
    QString m_Artist;
    QStringList m_AlbumList;
    QStringList m_ArtistList;
    QStringList m_Metadata; //added by aettie 2013.03.19 for gracenote issue

};

#endif // DHAVN_GRACENOTECOVERART_H
