#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H

#include <QObject>
#include <QtCore/QRegExp>
#include <QTime>

#include "DHAVN_AppFileManager_BaseMediaModel.h"
#include "DHAVN_AppFileManager_AudioModelItem.h"
#include "DHAVN_AppFileManager_CStateManager.h"
#include "DHAVN_TrackerAbstractor.h"
#include "DHAVN_AppFileManager_Logger.h"

class AudioModel : public BaseMediaModel
{
    Q_OBJECT
    Q_DISABLE_COPY(AudioModel)
    // USE_FM_LOG_UTIL
public:
    explicit AudioModel(EMediaDataRequestType requestType,
                        const QStringList& requestParameter,
                        const QString& rootFolder = "",
                        QObject *parent = 0);
    ~AudioModel();

    virtual void exec();
    virtual void cancel();
    virtual EMediaDataRequestType getRequestType();

signals:
    void audioListLoaded(const QList<AudioModelItem> audioList,
                         int errorCode);

private slots:
    void onRequestFinished(QVector<QStringList> dataList,
                           ETrackerAbstarctor_Query_Ids queryId);

private:
    EMediaDataRequestType requestType;
    QRegExp illegalCharacters;
    QString whitespace;

    QScopedPointer<CTrackerAbstractor> abstractor;
    QString baseFolder;
    QTime requestTimer;
    QStringList queryParameters;

private:
    void buildArtistSongsRequest(const QString& name, const QString& rootFolder,
                                 QSparqlQuery& result);
    void buildArtistAlbumsRequest(const QString& name, QSparqlQuery& result);
    QString hash(const QString &identifier) const;
};

#endif // ARTISTMODEL_H
