/*
 ******************************************************************
 *      COPYRIGHT  2013, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                                   *
 * PROGRAMMER : Radhakrushna Mohanty                              *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 1.0                                               *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : Pipeline handler .
 *----------------------------------------------------------------*
 */

#ifndef DHAVN_PIPELINEHANDLER_H
#define DHAVN_PIPELINEHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <DHAVN_LogUtil.h>
#include <DHAVN_AudioSourceType.h>

class CGSTAudioRouter;
class PipeLinehandler: public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    PipeLinehandler(QObject *parent , CGSTAudioRouter* inRouter );
    virtual ~PipeLinehandler();
    QString GetCurrentStateType(EAudioSource nType);

public Q_SLOTS: // METHODS
    void AquireAudioSource( const QString &inSrc, const QString& inRate,
                            const EAudioSource& inSrcType);
    void ReleaseAudioSource(const EAudioSource& inSrcType);

private:
    CGSTAudioRouter* m_pRouter;
    EAudioSource     m_eCurrentType;
};

#endif // DHAVN_PIPELINEHANDLER_H
