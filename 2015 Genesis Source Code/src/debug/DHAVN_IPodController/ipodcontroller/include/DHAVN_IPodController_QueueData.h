#ifndef DHAVN_IPODCONTROLLER_QUEUEDATA_H
#define DHAVN_IPODCONTROLLER_QUEUEDATA_H
class RequestDataElement
{
public:
    RequestDataElement(int queryId=-1, uint reqUID=0, int playlistIndex=-1, int startIndex=-1, int nCount=-1)
    {
        m_queryId = queryId;
        m_reqUID = reqUID;
        m_playlistIndex = playlistIndex;
        m_startIndex = startIndex;
        m_nCount = nCount;
    }

     int m_queryId;
     uint m_reqUID;
     int m_playlistIndex;
     int m_startIndex;
     int m_nCount;
};

class CIpodControllerQueueData
{
public:
    CIpodControllerQueueData(bool prevForced = false, int repeatMode = -1, int randomMode = -1, struct PlayList* list = NULL, int currentIndex = -1, int position = 0)
    {
        m_List = list;
        m_PrevForced = prevForced;
        m_RepeatMode = repeatMode;
        m_RandomMode = randomMode;
        m_CurrentIndex = currentIndex;
        m_NextLevel = 0;
        m_position = position;  // added by sungha.choi 2013.04.19


        //Added for static test.
        m_RequestData.m_nCount = 0;
        m_RequestData.m_playlistIndex = 0;
        m_RequestData.m_queryId = 0;
        m_RequestData.m_reqUID = 0;
        m_RequestData.m_startIndex = 0;
    }
    CIpodControllerQueueData(RequestDataElement element)
    {
        m_RequestData.m_nCount = element.m_nCount;
        m_RequestData.m_playlistIndex = element.m_playlistIndex;
        m_RequestData.m_queryId = element.m_queryId;
        m_RequestData.m_reqUID = element.m_reqUID;
        m_RequestData.m_startIndex = element.m_startIndex;

        //Added for static test.
        m_List = NULL;
        m_PrevForced = false;
        m_RepeatMode = 0;
        m_RandomMode = 0;
        m_CurrentIndex = 0;
        m_NextLevel = 0;
        m_position = 0;
    }

    void clearQueueData()
    {
        m_List = NULL;
        m_PrevForced = false;
        m_RepeatMode = -1;
        m_RandomMode = -1;
        m_CurrentIndex = -1;
        m_NextLevel = 0;
        m_position = 0;     // added by sungha.choi 2013.04.19
    }
    struct PlayList* m_List;
    bool m_PrevForced;
    int m_RepeatMode;
    int m_RandomMode;
    int m_CurrentIndex;
    int m_NextLevel;
    int m_position; // added by sungha.choi 2013.04.19
    RequestDataElement m_RequestData;
};
#endif // DHAVN_IPODCONTROLLER_QUEUEDATA_H
