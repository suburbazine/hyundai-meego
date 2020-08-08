#include "deckcontroller.h"
#include "deckmanager.h"
#include "cddamanager.h"
#include "videocdmanager.h"
#include "dvdmanager.h"
#include "compaudiomanager.h"
#include "util.h"

DeckController* DeckController::m_pInstance = 0;

DeckController* DeckController::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new DeckController();
    }

    return m_pInstance;
}

DeckController::~DeckController()
{
    MC_LOG << "~DeckController()" << LOG_ENDL;

    m_pDeckMngr->CloseDeckProtocol();
    //DeckManager::DeleteInstance();
}

IMediaInterface* DeckController::GetMediaInstance()
{
    if(!m_pDeckMngr) m_pDeckMngr = DeckManager::GetInstance();
    
    return m_pDeckMngr->GetMediaInstance();
}

IAudioInterface* DeckController::GetAudioInstance()
{
    if(!m_pDeckMngr) m_pDeckMngr = DeckManager::GetInstance();
    
    return m_pDeckMngr->GetAudioInstance();
}

IVideoInterface* DeckController::GetVideoInstance()
{
    if(!m_pDeckMngr) m_pDeckMngr = DeckManager::GetInstance();
    
    return m_pDeckMngr->GetVideoInstance();
}

void DeckController::HandleModeChanged(int nMode)
{
    switch(nMode) 
    {
    case SCANOFF:
    case SCANFILE:
    case SCANFOLDER:
    case SCANALL:
        m_pDeckMngr->SetScanMode(nMode);
        emit OnModeChanged(nMode);
        break;
        
    case RANDOMOFF:
    case RANDOMFILE:
    case RANDOMFOLDER:
        m_pDeckMngr->SetRandomMode(nMode);
        emit OnModeChanged(nMode);
        break;
        
    case REPEATFILE:
    case REPEATFOLDER:
    case REPEATALL:
        m_pDeckMngr->SetRepeatMode(nMode);
        emit OnModeChanged(nMode);
        break;
        
    default:
        break;
    }
}

DeckController::DeckController(QObject *parent) : QObject(parent),
m_pDeckMngr(0)    
{
    CDDAManager *pCDDAMngr = CDDAManager::GetInstance();
    connect(pCDDAMngr, SIGNAL(OnPositionChanged(int)),                  this, SIGNAL(OnPositionChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnDurationChanged(int)),                  this, SIGNAL(OnDurationChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnStateChanged(int)),                     this, SIGNAL(OnStateChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnDiscStateChanged(int)),                 this, SIGNAL(OnDiscStateChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnScreenStateChanged(int)),               this, SIGNAL(OnScreenStateChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnError(int)),                            this, SIGNAL(OnError(int)));
    connect(pCDDAMngr, SIGNAL(OnScanCompleted()),                       this, SIGNAL(OnScanCompleted()));
    connect(pCDDAMngr, SIGNAL(OnTrackChanged(int)),                     this, SIGNAL(OnTrackChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnModeChanged(int)),                      this, SLOT(HandleModeChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnCurrentMediaStatusChanged(int)),        this, SIGNAL(OnCurrentMediaStatusChanged(int)));
    connect(pCDDAMngr, SIGNAL(OnTOCReceived(DvdcTOC*)),                 this, SIGNAL(OnTOCReceived(DvdcTOC*)));
    connect(pCDDAMngr, SIGNAL(OnTOC2Received(DvdcTOC2*)),               this, SIGNAL(OnTOC2Received(DvdcTOC2*)));
    connect(pCDDAMngr, SIGNAL(OnCDTextReceived(DvdcCDText*)),           this, SIGNAL(OnCDTextReceived(DvdcCDText*)));
    connect(pCDDAMngr, SIGNAL(OnCDTextInfoReceived(DvdcCdTextInfo*)),   this, SIGNAL(OnCDTextInfoReceived(DvdcCdTextInfo*)));
    connect(pCDDAMngr, SIGNAL(OnDirTreeReceived(DvdcDirTree*)),         this, SIGNAL(OnDirTreeReceived(DvdcDirTree*)));
    connect(pCDDAMngr, SIGNAL(OnTagReceived(DvdcTagInfo*)),             this, SIGNAL(OnTagReceived(DvdcTagInfo*)));
    connect(pCDDAMngr, SIGNAL(OnTag2Received(DvdcTag2Info*)),           this, SIGNAL(OnTag2Received(DvdcTag2Info*)));
    connect(pCDDAMngr, SIGNAL(OnFileNameReceived(DvdcFileName*)),       this, SIGNAL(OnFileNameReceived(DvdcFileName*)));
    connect(pCDDAMngr, SIGNAL(OnFolderNameReceived(DvdcFolderName*)),   this, SIGNAL(OnFolderNameReceived(DvdcFolderName*)));
    connect(pCDDAMngr, SIGNAL(OnGraceNoteTOCReceived(int,void*)),       this, SIGNAL(OnGraceNoteTOCReceived(int,void*)));
    connect(pCDDAMngr, SIGNAL(OnTrackBufferred(int,int,int,int)),       this, SIGNAL(OnTrackBufferred(int,int,int,int)));
    connect(pCDDAMngr, SIGNAL(OnNormalPlay()),                          this, SIGNAL(OnNormalPlay()));
    connect(pCDDAMngr, SIGNAL(OnRealSoundChanged()),                    this, SIGNAL(OnRealSoundChanged())); // added by cychoi 20103.06.11
    connect(pCDDAMngr, SIGNAL(OnUserTrackChanged(int)),                 this, SIGNAL(OnUserTrackChanged(int))); // added by cychoi 20103.07.20 for add new signal
    connect(pCDDAMngr, SIGNAL(OnAutoTrackChanged(int)),                 this, SIGNAL(OnAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    VideoCDManager *pVideoCDMngr = VideoCDManager::GetInstance();
    connect(pVideoCDMngr, SIGNAL(OnPositionChanged(int)),               this, SIGNAL(OnPositionChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnDurationChanged(int)),               this, SIGNAL(OnDurationChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnStateChanged(int)),                  this, SIGNAL(OnStateChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnDiscStateChanged(int)),              this, SIGNAL(OnDiscStateChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnScreenStateChanged(int)),            this, SIGNAL(OnScreenStateChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnError(int)),                         this, SIGNAL(OnError(int)));
    connect(pVideoCDMngr, SIGNAL(OnScanCompleted()),                    this, SIGNAL(OnScanCompleted()));
    connect(pVideoCDMngr, SIGNAL(OnTrackChanged(int)),                  this, SIGNAL(OnTrackChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnModeChanged(int)),                   this, SLOT(HandleModeChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnCurrentMediaStatusChanged(int)),     this, SIGNAL(OnCurrentMediaStatusChanged(int)));
    connect(pVideoCDMngr, SIGNAL(OnTOCReceived(DvdcTOC*)),              this, SIGNAL(OnTOCReceived(DvdcTOC*)));
    connect(pVideoCDMngr, SIGNAL(OnTOC2Received(DvdcTOC2*)),            this, SIGNAL(OnTOC2Received(DvdcTOC2*)));
    connect(pVideoCDMngr, SIGNAL(OnTrackBufferred(int,int,int,int)),    this, SIGNAL(OnTrackBufferred(int,int,int,int)));
    connect(pVideoCDMngr, SIGNAL(OnNormalPlay()),                       this, SIGNAL(OnNormalPlay()));
    connect(pVideoCDMngr, SIGNAL(OnRealSoundChanged()),                 this, SIGNAL(OnRealSoundChanged())); // added by cychoi 20103.06.11
    connect(pVideoCDMngr, SIGNAL(OnUserTrackChanged(int)),              this, SIGNAL(OnUserTrackChanged(int))); // added by cychoi 20103.07.20 for add new signal
    //connect(pVideoCDMngr, SIGNAL(OnAutoTrackChanged(int)),              this, SIGNAL(OnAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    DVDManager *pDVDMngr = DVDManager::GetInstance();
    connect(pDVDMngr, SIGNAL(OnPositionChanged(int)),                   this, SIGNAL(OnPositionChanged(int)));
    connect(pDVDMngr, SIGNAL(OnDurationChanged(int)),                   this, SIGNAL(OnDurationChanged(int)));
    connect(pDVDMngr, SIGNAL(OnStateChanged(int)),                      this, SIGNAL(OnStateChanged(int)));
    connect(pDVDMngr, SIGNAL(OnDiscStateChanged(int)),                  this, SIGNAL(OnDiscStateChanged(int)));
    connect(pDVDMngr, SIGNAL(OnScreenStateChanged(int)),                this, SIGNAL(OnScreenStateChanged(int)));
    connect(pDVDMngr, SIGNAL(OnError(int)),                             this, SIGNAL(OnError(int)));
    connect(pDVDMngr, SIGNAL(OnScanCompleted()),                        this, SIGNAL(OnScanCompleted()));
    connect(pDVDMngr, SIGNAL(OnTrackChanged(int)),                      this, SIGNAL(OnTrackChanged(int)));
    connect(pDVDMngr, SIGNAL(OnModeChanged(int)),                       this, SLOT(HandleModeChanged(int)));
    connect(pDVDMngr, SIGNAL(OnCurrentMediaStatusChanged(int)),         this, SIGNAL(OnCurrentMediaStatusChanged(int)));
    connect(pDVDMngr, SIGNAL(OnTitleGroupChanged(int)),                 this, SIGNAL(OnTitleGroupChanged(int)));
    connect(pDVDMngr, SIGNAL(OnTotalSubtitleChanged(int)),              this, SIGNAL(OnTotalSubtitleChanged(int)));
    connect(pDVDMngr, SIGNAL(OnTotalAudioChanged(int)),                 this, SIGNAL(OnTotalAudioChanged(int)));
    connect(pDVDMngr, SIGNAL(OnTotalAngleChanged(int)),                 this, SIGNAL(OnTotalAngleChanged(int)));
    connect(pDVDMngr, SIGNAL(OnTotalAudioLangReceived(int*,int*,int*,int)),  this, SIGNAL(OnTotalAudioLangReceived(int*,int*,int*,int)));
    connect(pDVDMngr, SIGNAL(OnTotalSubtitleLangReceived(int*, int)),   this, SIGNAL(OnTotalSubtitleLangReceived(int*, int)));
    connect(pDVDMngr, SIGNAL(OnAudioLangChanged(int)),                  this, SIGNAL(OnAudioLangChanged(int)));
    connect(pDVDMngr, SIGNAL(OnSubtitleLangChanged(int)),               this, SIGNAL(OnSubtitleLangChanged(int)));
    connect(pDVDMngr, SIGNAL(OnAngleChanged(int)),                      this, SIGNAL(OnAngleChanged(int)));
    connect(pDVDMngr, SIGNAL(OnSearchCompleted(int, int, int)),         this, SIGNAL(OnSearchCompleted(int, int, int)));
    connect(pDVDMngr, SIGNAL(OnDVDInitCompleted(int)),                  this, SIGNAL(OnDVDInitCompleted(int)));
    connect(pDVDMngr, SIGNAL(OnTrackBufferred(int,int,int,int)),        this, SIGNAL(OnTrackBufferred(int,int,int,int)));
    connect(pDVDMngr, SIGNAL(OnNormalPlay()),                           this, SIGNAL(OnNormalPlay()));    
    connect(pDVDMngr, SIGNAL(OnRealSoundChanged()),                     this, SIGNAL(OnRealSoundChanged())); // added by cychoi 20103.06.11
    connect(pDVDMngr, SIGNAL(OnUserTrackChanged(int)),                  this, SIGNAL(OnUserTrackChanged(int))); // added by cychoi 20103.07.20 for add new signal
    //connect(pDVDMngr, SIGNAL(OnAutoTrackChanged(int)),                  this, SIGNAL(OnAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    CompAudioManager *pCAMngr = CompAudioManager::GetInstance();
    connect(pCAMngr, SIGNAL(OnPositionChanged(int)),                    this, SIGNAL(OnPositionChanged(int)));
    connect(pCAMngr, SIGNAL(OnDurationChanged(int)),                    this, SIGNAL(OnDurationChanged(int)));
    connect(pCAMngr, SIGNAL(OnStateChanged(int)),                       this, SIGNAL(OnStateChanged(int)));
    connect(pCAMngr, SIGNAL(OnDiscStateChanged(int)),                   this, SIGNAL(OnDiscStateChanged(int)));
    connect(pCAMngr, SIGNAL(OnScreenStateChanged(int)),                 this, SIGNAL(OnScreenStateChanged(int)));
    connect(pCAMngr, SIGNAL(OnError(int)),                              this, SIGNAL(OnError(int)));
    connect(pCAMngr, SIGNAL(OnScanCompleted()),                         this, SIGNAL(OnScanCompleted()));
    connect(pCAMngr, SIGNAL(OnTrackChanged(int)),                       this, SIGNAL(OnTrackChanged(int)));
    connect(pCAMngr, SIGNAL(OnTrackUpdated(int)),                       this, SIGNAL(OnTrackUpdated(int))); // added by cychoi 2013.11.17 for ITS 208698 MP3 Play Info on Cluster 
    connect(pCAMngr, SIGNAL(OnModeChanged(int)),                        this, SLOT(HandleModeChanged(int)));
    connect(pCAMngr, SIGNAL(OnCurrentMediaStatusChanged(int)),          this, SIGNAL(OnCurrentMediaStatusChanged(int)));
    connect(pCAMngr, SIGNAL(OnTOCReceived(DvdcTOC*)),                   this, SIGNAL(OnTOCReceived(DvdcTOC*)));
    connect(pCAMngr, SIGNAL(OnTOC2Received(DvdcTOC2*)),                 this, SIGNAL(OnTOC2Received(DvdcTOC2*)));
    connect(pCAMngr, SIGNAL(OnCDTextReceived(DvdcCDText*)),             this, SIGNAL(OnCDTextReceived(DvdcCDText*)));
    connect(pCAMngr, SIGNAL(OnCDTextInfoReceived(DvdcCdTextInfo*)),     this, SIGNAL(OnCDTextInfoReceived(DvdcCdTextInfo*)));
    connect(pCAMngr, SIGNAL(OnDirTreeReceived(DvdcDirTree*)),           this, SIGNAL(OnDirTreeReceived(DvdcDirTree*)));
    connect(pCAMngr, SIGNAL(OnTagReceived(DvdcTagInfo*)),               this, SIGNAL(OnTagReceived(DvdcTagInfo*)));
    connect(pCAMngr, SIGNAL(OnTag2Received(DvdcTag2Info*)),             this, SIGNAL(OnTag2Received(DvdcTag2Info*)));
    connect(pCAMngr, SIGNAL(OnAllTagReceived(DvdcAllTagInfo*)),         this, SIGNAL(OnAllTagReceived(DvdcAllTagInfo*)));
    connect(pCAMngr, SIGNAL(OnFileNameReceived(DvdcFileName*)),         this, SIGNAL(OnFileNameReceived(DvdcFileName*)));
    connect(pCAMngr, SIGNAL(OnFolderNameReceived(DvdcFolderName*)),     this, SIGNAL(OnFolderNameReceived(DvdcFolderName*)));
    connect(pCAMngr, SIGNAL(OnDirTreeCompleted()),                      this, SIGNAL(OnDirTreeCompleted()));
    connect(pCAMngr, SIGNAL(OnTrackBufferred(int,int,int,int)),         this, SIGNAL(OnTrackBufferred(int,int,int,int)));
    connect(pCAMngr, SIGNAL(OnNormalPlay()),                            this, SIGNAL(OnNormalPlay()));        
    connect(pCAMngr, SIGNAL(OnRealSoundChanged()),                      this, SIGNAL(OnRealSoundChanged())); // added by cychoi 20103.06.11
    connect(pCAMngr, SIGNAL(OnUserTrackChanged(int)),                   this, SIGNAL(OnUserTrackChanged(int))); // added by cychoi 20103.07.20 for add new signal
    connect(pCAMngr, SIGNAL(OnAutoTrackChanged(int)),                   this, SIGNAL(OnAutoTrackChanged(int))); // added by cychoi 2014.04.06 for ITS 233500

    m_pDeckMngr = DeckManager::GetInstance();
    connect(m_pDeckMngr, SIGNAL(OnDiscStateChanged(int)),               this, SIGNAL(OnDiscStateChanged(int)));
    connect(m_pDeckMngr, SIGNAL(OnError(int)),                          this, SIGNAL(OnError(int)));
    connect(m_pDeckMngr, SIGNAL(OnDeckPriorityLangChanged()),           this, SIGNAL(OnDeckPriorityLangChanged()));
    
    m_pDeckMngr->OpenDeckProtocol();
}
