#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#define USE_QELAPASED_TIMER         0
//To enable creation of pt_gpp.xls define USE_PERFORMANCE_TEST as 1
#define USE_PERFORMANCE_TEST        0 
#define PT_LOG_FILE_NAME            "/log_data/log/pt_gpp.xls"

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
#define PT_SET_FILECOUNT(COUNT)               \
{                                               \
     Performance::SetFileCount(COUNT);         \
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
#define PT_SET_FILECOUNT(COUNT)    \
{                                  \
    ;                              \
}
#endif

enum PT_OBJECT_TYPE
{    
    PT_OBJ_TRACKER_MINING,
    PT_OBJ_UPDATE_FILES_IN_USB,
    PT_OBJ_UPDATE_FILES_IN_JUKEBOX,
    PT_OBJ_UPDATE_MLDB_JB,
    PT_OBJ_UPDATE_MLDB_USB,
    PT_OBJ_CHECK_IF_EXISTS_IN_MLDB,
    PT_OBJ_PARSE_FILE_PATH,
    PT_OBJ_EXTRACT_ID3_INFO,
    PT_OBJ_TEXTID_LOOKUP,
    PT_OBJ_MLDB_POPULATE,
    PT_OBJ_CREATE_MUSIC_DATA,
    PT_OBJ_EXTRACT_XSAMPA,
    PT_OBJ_TRACKER_UPDATE,    
    PT_OBJ_UPDATE_MLDB_IPOD,
    PT_OBJ_READ_DATA_IPOD,
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
    static void SetFileCount(int count);

private:
    static int              m_bFirstPrint;
    #if USE_QELAPASED_TIMER
    static QElapsedTimer    m_pTimer[PT_OBJ_SIZE];
    #else
    static double           m_pStartTime[PT_OBJ_SIZE];
    #endif
    static double           m_pTotalTime[PT_OBJ_SIZE];
    static int              m_pCallCount[PT_OBJ_SIZE];
    static int m_FileCount;
};

#endif
