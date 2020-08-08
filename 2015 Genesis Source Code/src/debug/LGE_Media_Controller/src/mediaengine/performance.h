#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#include <QElapsedTimer>

#define USB_PERFORMANCE_TEST        1
#define PT_LOG_FILE_NAME            "/home/meego/pt_log.xls"

#if USB_PERFORMANCE_TEST
#define PT_REST_TIME                            \
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
#define PT_REST_TIME                \
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
    PT_OBJ_NAME_0,
    PT_OBJ_NAME_1,
    PT_OBJ_NAME_UPDATE_STORAGE,
    PT_OBJ_NAME_USB_CONNECT,
    PT_OBJ_NAME_4,
    PT_OBJ_NAME_5,
    PT_OBJ_NAME_6,
    PT_OBJ_NAME_7,
    PT_OBJ_NAME_8,
    PT_OBJ_NAME_9,
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
    static QElapsedTimer    m_pTimer[PT_OBJ_SIZE];
    static double           m_pTotalTime[PT_OBJ_SIZE];
    static int              m_pCallCount[PT_OBJ_SIZE];
};

#endif
