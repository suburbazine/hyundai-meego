
#include<PipeLineThread.h>
#include <DHAVN_GSTAudioRouterInterface.h>
#include <QFile>
#include <QTextStream>

CPipeLineThread::CPipeLineThread():
    m_Process(NULL)
   ,m_pAudioRouter(NULL)
  ,m_eCurrentSource(ENone)
{
    LOG_INIT_V2("Pandora");

    connect(this , SIGNAL(startPipeLine()),this,SLOT(startPipe()),Qt::QueuedConnection);
    connect(this,SIGNAL(stopPipeLine()),this,SLOT(stop()),Qt::QueuedConnection);

    m_pAudioRouter = new GSTAudioRouterDBus("org.lge.GSTAudioRouterDBus", "/",
                                            QDBusConnection::sessionBus(), 0);

}

CPipeLineThread::~CPipeLineThread()
{

    stop();
    if(m_Process){
        delete m_Process;
        m_Process = NULL;
    }
    delete m_pAudioRouter;
    m_pAudioRouter = NULL;
}
void CPipeLineThread::SetSourceType(USB_PORT_T port)
{
    if( eUSB_PORT1 == port){
        m_eCurrentSource = EPandoraFront;
    }else{
         m_eCurrentSource = EPandoraRear;
    }
}

void CPipeLineThread::HandlePipeLine(EStates inState , QString inSrc,int inRate)
{    
    QFile sampleRateFile;
    switch(inState)
    {
    case EStart:
        LOG_INFO<<"CPipeLineThread::HandlePipeLine Start pipeline "<<LOG_ENDL;
        m_SampleRate.clear();
        m_SampleRate = m_SampleRate.sprintf("%d",inRate);
        m_Src.clear();
        m_Src = inSrc;
        if( m_eCurrentSource == EPandoraFront)
        {
            sampleRateFile.setFileName("/app/data/AppMedia/share/sampleRate_1.txt");
        }
        else if (m_eCurrentSource == EPandoraRear)
        {
            sampleRateFile.setFileName("/app/data/AppMedia/share/sampleRate_2.txt");
        }
        else
        {
            LOG_CRITICAL<<"Error , Pipeline Source could not set "<<LOG_ENDL;
            // return;
        }
        if(QFile::exists(sampleRateFile.fileName()) && sampleRateFile.open(QIODevice::ReadOnly))
        {
            QTextStream textStream (&sampleRateFile);
            textStream >> m_SampleRate;
            sampleRateFile.close();
        }
        else{
            LOG_CRITICAL<<"Error , Audio sample rate file is not present "<<LOG_ENDL;
        }
        emit startPipeLine();
    break;
    case EReSet:
        LOG_INFO<<"CPipeLineThread::HandlePipeLine restart pipeline "<<LOG_ENDL;
        m_SampleRate.clear();
        m_SampleRate = m_SampleRate.sprintf("%d",inRate);
        m_Src.clear();
        m_Src = inSrc;
        emit startPipeLine();
    break;
    case EStop:
        LOG_INFO<<"CPipeLineThread::HandlePipeLine Stop pipeline "<<LOG_ENDL;
        emit stopPipeLine();
    break;
    default:
    break;
  }
}

void CPipeLineThread::run()
{
   // LOG_INFO<<"CPipeLineThread::run "<<LOG_ENDL;
    this->exec();
}
void CPipeLineThread::stop()
{
    m_pAudioRouter->ReleaseAudioSource(m_eCurrentSource);
#if 0
    LOG_INFO<<"CPipeLineThread::stop() "<<LOG_ENDL;
    if(m_Process && (m_Process->state() == QProcess::Running ))
    {

        m_Process->closeWriteChannel();
        m_Process->terminate();
        if(false == m_Process->waitForFinished(2000))
        {
            if(m_Process && m_Process->state() == QProcess::Running)
            {
                m_Process->kill();
            }
        }
        delete m_Process;
        m_Process = NULL;
    }
#endif
}

void CPipeLineThread::startPipe()
{
    LOG_HIGH << "Start Audio Pipe m_Src :" << m_Src <<" , m_SampleRate : "<<m_SampleRate << LOG_ENDL;

    m_pAudioRouter->AquireAudioSource(m_Src,m_SampleRate , m_eCurrentSource);
}

QStringList CPipeLineThread::GetArguments()
{
   // LOG_TRACE << "CPipeLineThread::GetArguments() IN" << LOG_ENDL;

    QStringList list;


    if( m_Src.contains( "plughw:1", Qt::CaseInsensitive ) != true &&
        m_Src.contains( "plughw:2", Qt::CaseInsensitive ) != true &&
        m_Src.contains( "plughw:3", Qt::CaseInsensitive ) != true ) {

        LOG_TRACE << "CPipeLineThread::GetArguments() Source port is not here, return NULL" << LOG_ENDL;
        return list;
    }

    QString     src;
    src.append("src=");
    src.append(m_Src);

    QString sampleRate;
    sampleRate.append("rate=");
    sampleRate.append(m_SampleRate);

    list.append(src);
    list.append(sampleRate);

   // LOG_TRACE << "list items are : " << list[0] << " , " << list[1] << LOG_ENDL;
    return list;
}

void CPipeLineThread::stoppedSlot( int exitCode, QProcess::ExitStatus exitStatus )
{
    LOG_TRACE << "stoppedSlot() IN, exitCode = " << exitCode << ", ExitStatus = " << exitStatus << LOG_ENDL;

#if 0
    bool retValue   = false;

    disconnect(m_Process,0,this,0);
    if( exitStatus == QProcess::NormalExit ) {
        LOG_TRACE << "stoppedSlot(), GSTAudioRouterWrapper is terminated in normail case" << LOG_ENDL;
    }
    else if( exitStatus == QProcess::CrashExit ) {

        LOG_TRACE << "stoppedSlot() GSTAudioRouterWrapper Crashed!!!!!!!! retry" << LOG_ENDL;

            if( m_Process != NULL ) {

                m_Process->close();
                delete m_Process;
                m_Process = NULL;
                //startPipe();
        }
    }
#endif
    LOG_TRACE << "stoppedSlot() OUT"<< LOG_ENDL;
}
