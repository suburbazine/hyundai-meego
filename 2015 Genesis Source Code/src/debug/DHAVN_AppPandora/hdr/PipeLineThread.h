#ifndef PIPELINETHREAD_H
#define PIPELINETHREAD_H

#include <QThread>
#include <QString>
#include <cstdlib>
#include <QProcess>
#include <QTime>
#include <DHAVN_LogUtil.h>
#include <DHAVN_AudioSourceType.h>
#include <uistate_manager_def.h>

class GSTAudioRouterDBus;
class CPipeLineThread : public QThread
{
    Q_OBJECT
private:
    USE_LOG_UTIL

public:
    enum EStates
    {
        None =0 ,
        EStart,
        EStop,
        EPlay,
        EPause,
        EReSet
    };

    CPipeLineThread();
    ~CPipeLineThread();


    QStringList	GetArguments();

    void HandlePipeLine(EStates inState , QString inSrc,int inRate);
    void SetSourceType(USB_PORT_T port);
protected:
    virtual void run();
private slots:
    void play(){}
    void pause(){}
    void stop();
    void startPipe();
    void stoppedSlot( int, QProcess::ExitStatus );

signals:
    void startPipeLine();
    void stopPipeLine();

private:
    QProcess* m_Process;
    QString m_SampleRate;
    QString m_Src;
    GSTAudioRouterDBus* m_pAudioRouter;
    EAudioSource m_eCurrentSource;

};
#endif // PIPELINETHREAD_H
