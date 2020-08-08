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
#include "DHAVN_PipelineHandler.h"
#include <DHAVN_GSTAudioRouter_Def.h>

class CGSTAudioRouter;

PipeLinehandler::PipeLinehandler( QObject *parent , CGSTAudioRouter* inRouter ):
    QObject(parent)
  ,m_pRouter(inRouter)
  ,m_eCurrentType(ENone)
{
    LOG_INIT_V2("Media_IPodController");
}

PipeLinehandler::~PipeLinehandler()
{
    // destructor
}

void PipeLinehandler::AquireAudioSource( const QString &inSrc,  const QString& inRate, const EAudioSource& inSrcType )
{
    LOG_HIGH << "PipeLinehandler::AquireAudioSource() IN, source = " << inSrc << ", samplerate = " << inRate << ", SourceType = " << GetCurrentStateType(inSrcType) << LOG_ENDL;

    QString strCurSrc  = m_pRouter->GetCardSrc();
    QString strCurRate = m_pRouter->GetRate();

    if( inSrcType == m_eCurrentType && !strCurSrc.compare(inSrc) && !strCurRate.compare(inRate) && m_pRouter->GetPipeState() == CGSTAudioRouter::ePlay )
    {
        LOG_HIGH << "PipeLinehandler::AquireAudioSource() same samplerate, same source" << LOG_ENDL;
        return;
    }

    if( m_pRouter->GetPipeState() == CGSTAudioRouter::ePlay ) m_pRouter->stop();

    m_pRouter->setSampleRate(inRate);
    m_pRouter->setSource(inSrc);

    int  count = 5;
    bool isStarted = false;

    while(count--)
    {
        isStarted = m_pRouter->start();
        if(isStarted)
            break;
        m_pRouter->stop();
        usleep(300000);
    }

    if(isStarted)
    {
        m_eCurrentType = inSrcType ;
        LOG_TRACE << "   started the routing, m_eCurrentType = " << GetCurrentStateType(m_eCurrentType) <<  LOG_ENDL;
    }
    else
    {
        LOG_TRACE << "   failed to start" << LOG_ENDL;
    }

    LOG_HIGH << "PipeLinehandler::AquireAudioSource() OUT, m_eCurrentType = " << GetCurrentStateType(m_eCurrentType) << LOG_ENDL;
}

void PipeLinehandler::ReleaseAudioSource(const EAudioSource& inSrcType)
{
    LOG_HIGH << "PipeLinehandler::ReleaseAudioSource() IN, SourceType = " << GetCurrentStateType(inSrcType) << ", m_eCurrentType = " << GetCurrentStateType(m_eCurrentType) << LOG_ENDL;

    if(inSrcType == m_eCurrentType || inSrcType == EAllSource)
    {
        m_eCurrentType = ENone;
        m_pRouter->stop();        
    }
    else
    {
        LOG_TRACE << "   source mismatch , Don't Release " << LOG_ENDL;
    }

    LOG_HIGH << "PipeLinehandler::ReleaseAudioSource() OUT, m_eCurrentType = " << GetCurrentStateType(m_eCurrentType) << LOG_ENDL;
}

QString PipeLinehandler::GetCurrentStateType(EAudioSource nType)
{
    QString strReturnString;

    switch( nType )
    {
        case ENone          : strReturnString = "None";      break;
        case EOther         : strReturnString = "Other";     break;
        case EIpodFront     : strReturnString = "IPodFront"; break;
        case EIpodRear      : strReturnString = "IPodRear";  break;
        case EPandoraFront  : strReturnString = "PandoraFront"; break;
        case EPandoraRear   : strReturnString = "PandoraRear";  break;
        case EAhaFront      : strReturnString = "AhaFront";  break;
        case EAhaRear       : strReturnString = "AhaRear" ;  break;
        case EAllSource     : strReturnString = "AllSource"; break;
        default :             strReturnString = "Abnormal Type"; break;
    }
    return strReturnString;
}

//End of file
