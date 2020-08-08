#include "DHAVN_AppFileManager_AudioModel.h"

#include <QtSparql/QSparqlResultRow>

#include <QDebug>
#include <QDir>
#include <QtCore/QCryptographicHash>

#define MEDIA_ART_ILLEGAL_CHARACTERS \
    "\\(.*\\)|\\{.*\\}|\\[.*\\]|<.*>|[\\(\\)_\\{\\}\\[\\]\\!@#$\\^&\\*\\+\\=\\|\\\\/\"'\\?~`]"

AudioModel::AudioModel(EMediaDataRequestType requestType,
                       const QStringList& requestParameter,
                       const QString& rootFolder,
                       QObject *parent) :
    BaseMediaModel(parent)
  , illegalCharacters(QLatin1String(MEDIA_ART_ILLEGAL_CHARACTERS))
  , whitespace(QCryptographicHash::hash(" ", QCryptographicHash::Md5).toHex())
  , abstractor(new CTrackerAbstractor)
  , baseFolder(rootFolder)
  , queryParameters(requestParameter)
{
    // FM_LOG_INIT_V2("AppFileManager");
    this->requestType = requestType;
    connect(this->abstractor.data(), SIGNAL(RequesteComplete(
        QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
        SLOT(onRequestFinished(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
}

AudioModel::~AudioModel()
{

}

void AudioModel::exec()
{
    ETrackerAbstarctor_Query_Ids requestId;

    switch (requestType)
    {
    case eMusicAllTitleRequest:
        requestId = eSongsByFolderQuery_Id;
        break;
    case eMusicAllArtistRequest:
        requestId = eArtistByFolderQuery_Id;
        break;
    case eMusicAllAlbomRequest:
        requestId = eAlbumByFolderQuery_Id;
        break;
    case eMusicArtistsFileListRequest:
        requestId = eGetMultipleArtistSongsQuery_Id;
        break;
    case eMusicAlbumsFileListRequest:
        requestId = eGetMultipleAlbumSongsQuery_Id;
        break;
    default:
        return;
    }

    this->requestTimer.start();
    if (requestId != eGetMultipleArtistSongsQuery_Id &&
        requestId != eGetMultipleAlbumSongsQuery_Id)
    {
        this->abstractor->RequestData(this->baseFolder, requestId);
    }
    else
    {
        this->abstractor->RequestData(this->queryParameters, this->baseFolder,
                                      requestId);
    }
}

void AudioModel::cancel()
{
}

void AudioModel::onRequestFinished(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId)
{
    // MP_LOG << "AudioModel::onRequestFinished Select " << dataList.size() <<
    //  " elements for " << this->baseFolder << " with request type " <<
    //             this->requestType << LOG_ENDL;
    // MP_LOG  << "AudioModel::onRequestFinished request exec time is " <<
    //              this->requestTimer.elapsed() << LOG_ENDL;
    QList<AudioModelItem> items;

    for (int i = 0; i < dataList.size(); i++)
    {
        AudioModelItem item;
        QStringList columns = dataList.at(i);

        switch (queryId)
        {
        case eSongsByFolderQuery_Id:
            item.setItemName(columns.at(0));
            item.setItemPath(columns.at(1));
            break;
        case eArtistByFolderQuery_Id:
            item.setItemName(columns.at(0));
            break;
        case eAlbumByFolderQuery_Id:
            item.setItemName(columns.at(0));
            {
                QString fileName = QDir::homePath()
                        + QLatin1String("/.cache/media-art/album-")
                        + hash(columns.at(1))
                        + QLatin1Char('-')
                        + hash(columns.at(0))
                        + QLatin1String(".jpeg");
                if (QFile::exists(fileName))
                {
                    item.setItemCover(fileName);
                }
            }
            break;
        case eGetMultipleArtistSongsQuery_Id:
        case eGetMultipleAlbumSongsQuery_Id:
            item.setItemName(columns.at(0));
            item.setItemPath(columns.at(1));
            break;
        default:
            return;
        }

        items << item;
    }

    emit audioListLoaded(items, 0);
}

void AudioModel::buildArtistSongsRequest(const QString &name, const QString& rootFolder,
                                         QSparqlQuery& result)
{
    QString queryString = QString(
            "SELECT ?song ?title ?url "
            "WHERE {"
            " ?song nmm:performer [ nmm:artistName \"%1\" ] ; "
            "       nie:title ?title ; "
            "       nie:isStoredAs ?as . "
            " ?as nie:url ?url . FILTER(REGEX(?url, \"%2\")) . "
            "} "
            "ORDER BY ?song"
        ).arg(name).arg(rootFolder);
    result.setQuery(queryString);
}

void AudioModel::buildArtistAlbumsRequest(const QString &name, QSparqlQuery& result)
{
    QString queryString = QString(
            "SELECT ?album ?title "
            "WHERE {"
            " ?album a nmm:MusicAlbum; "
            "          nie:title ?title; "
            "          nmm:albumArtist ?artist . "
            " ?artist a nmm:Artist ; "
            "           nmm:artistName \"%1\" . "
            "} "
            "ORDER BY ?album"
        ).arg(name);
    result.setQuery(queryString);
}

EMediaDataRequestType AudioModel::getRequestType()
{
    return this->requestType;
}

QString AudioModel::hash(const QString &identifier) const
{
    if (identifier.isEmpty()) {
        return whitespace;
    } else {
        return QCryptographicHash::hash(
                identifier.toLower().remove(illegalCharacters).simplified().toUtf8(),
                QCryptographicHash::Md5).toHex();
    }
}
