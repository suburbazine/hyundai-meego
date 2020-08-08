#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#define USE_QELAPASED_TIMER         0
#define USE_PERFORMANCE_TEST        0
#define PT_LOG_FILE_NAME            "/log_data/log/pt_ipod.xls"

#if USE_QELAPASED_TIMER
#include <QElapsedTimer>
#endif

#if USE_PERFORMANCE_TEST
#define PT_RESET_TIME                            \
{                                               \
    Performance::Reset();                       \
}
#define PT_START_TIME(INDEX, MSG)               \
{                                               \
    Performance::StartTime(INDEX, MSG);         \
}
#define PT_END_TIME(INDEX, MSG)                 \
{                                               \
    Performance::EndTime(INDEX, MSG);           \
}
#define PT_CALL_COUNT(INDEX)                    \
{                                               \
    Performance::m_pCallCount[INDEX]++;         \
}
#define PT_PRINT_TIME(FILENAME)                 \
{                                               \
    Performance::PrintTime(FILENAME);           \
}
#else
#define PT_RESET_TIME                \
{                                   \
    ;                               \
}
#define PT_START_TIME(INDEX, MSG)   \
{                                   \
    ;                               \
}
#define PT_END_TIME(INDEX, MSG)     \
{                                   \
    ;                               \
}
#define PT_CALL_COUNT(INDEX)        \
{                                   \
    ;                               \
}
#define PT_PRINT_TIME(FILENAME)     \
{                                   \
    ;                               \
}
#endif

enum PT_OBJECT_TYPE
{
    PT_OBJ_DEVICE_DETECT,
    PT_OBJ_IDENTIFICATION,
    PT_OBJ_AUTHENTICATION,
    PT_OBJ_AUTHENTICATION_NONIDPS,
    PT_OBJ_ENTER_REMOTEUI_MODE,
    PT_OBJ_METADATA_FETCHING_UID,
    PT_OBJ_METADATA_FETCHING_INDEX,
    PT_OBJ_TRACKER_UPDATE_METADATA,
    PT_OBJ_MASTERTABLE_UPDATE_METADATA,
    PT_OBJ_COVERART_FETCHING_UPDATE_TRACKER,
    PT_OBJ_GRACENOTE,
    PT_OBJ_ETC,
    PT_OBJ_SIZE
};

class Performance
{
public:
    static void     Reset();
    static void     StartTime(unsigned int nIndex, const char *pMsg = 0);
    static double   EndTime(unsigned int nIndex, const char *pMsg = 0);
    static void     PrintTime(const char *pFileName = 0);

private:
    static int              m_bFirstPrint;
    #if USE_QELAPASED_TIMER
    static QElapsedTimer    m_pTimer[PT_OBJ_SIZE];
    #else
    static double           m_pStartTime[PT_OBJ_SIZE];
    #endif
    static double           m_pTotalTime[PT_OBJ_SIZE];
    static int              m_pCallCount[PT_OBJ_SIZE];
};

#endif
