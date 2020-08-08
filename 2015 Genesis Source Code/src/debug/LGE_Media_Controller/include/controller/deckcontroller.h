#ifndef DECKCONTROLLER_H
#define DECKCONTROLLER_H

#include <QObject>
#include "imediainterface.h"
#include "iaudiointerface.h"
#include "ivideointerface.h"
#include "mediaplayer_def.h"

class DeckManager;

class DeckController : public QObject
{
    Q_OBJECT
public:
    static DeckController*  GetInstance();
    ~DeckController();

    IMediaInterface* GetMediaInstance();
    IAudioInterface* GetAudioInstance();
    IVideoInterface* GetVideoInstance();

public slots:
    void            HandleModeChanged(int nMode);
    
signals:   
    ///////////////////////////////////////////////////////////
    // FOR IMediaInferface    
    void            OnPositionChanged(int);
    void            OnDurationChanged(int);
    void            OnStateChanged(int);
    void            OnDiscStateChanged(int);
    void            OnScreenStateChanged(int);
    void            OnError(int);
    void            OnScanCompleted();
    void            OnTrackChanged(int index);
    void            OnTrackUpdated(int index); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    void            OnModeChanged(int index);
    void            OnCurrentMediaStatusChanged(int);
    void            OnTitleGroupChanged(int index);
    void            OnTotalSubtitleChanged(int nSubtitleNum);
    void            OnTotalAudioChanged(int nAudioNum);
    void            OnTotalAngleChanged(int nAngleNum);
    void            OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize);
    void            OnNormalPlay();
    void            OnRealSoundChanged(); // added by cychoi 20103.06.11
    void            OnUserTrackChanged(int nTrackIndex); // added by cychoi 20103.07.20 for add new signal
    void            OnAutoTrackChanged(int nTrackIndex); // added by cychoi 2014.04.06 for ITS 233500

    ///////////////////////////////////////////////////////////
    // FOR IAudioInterface or IVideoInterface
    void            OnTOCReceived(DvdcTOC *pTOC);
    void            OnTOC2Received(DvdcTOC2 *pTOC2);
    void            OnCDTextReceived(DvdcCDText *pCDText);
    void            OnCDTextInfoReceived(DvdcCdTextInfo *pCDTextInfo);
    void            OnDirTreeReceived(DvdcDirTree *pDirTree);
    void            OnTagReceived(DvdcTagInfo *pTag);
    void            OnTag2Received(DvdcTag2Info *pTag2);
    void            OnAllTagReceived(DvdcAllTagInfo *pAllTag);
    void            OnFileNameReceived(DvdcFileName *pFileName);
    void            OnFolderNameReceived(DvdcFolderName *pFolderName);
    void            OnTotalAudioLangReceived(int *pAudioCodeList, int *pAudioAttrList, int *pAudioChnlList, int nAudioCodeNum);
    void            OnTotalSubtitleLangReceived(int *pSubtitleCodeList, int nSubtitleCodeNum);
    void            OnAudioLangChanged(int nAudioNum);
    void            OnSubtitleLangChanged(int nSubtitleCode);
    void            OnAngleChanged(int nAngleNum);
    void            OnSearchCompleted(int nTitleNo, int nChapterNo, int nChapterSize);
    void            OnDVDInitCompleted(int bSuccess);
    void            OnGraceNoteTOCReceived(int nSize, void *pTocData);
    void            OnDeckPriorityLangChanged();
    void            OnDirTreeCompleted();

private:
    DeckController(QObject *parent = 0);

    static DeckController   *m_pInstance;
    DeckManager             *m_pDeckMngr;
};

#endif // DECKCONTROLLER_H
