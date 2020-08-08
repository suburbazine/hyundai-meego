#include "performance.h"
#include <stdio.h>
#include "util.h"

QElapsedTimer Performance::m_pTimer[PT_OBJ_SIZE] = {QElapsedTimer(), };
double Performance::m_pTotalTime[PT_OBJ_SIZE] = {0.0, };
int Performance::m_pCallCount[PT_OBJ_SIZE] = {0, };

void Performance::Reset()
{
    for(int i = 0; i < PT_OBJ_SIZE; i++) {
        m_pTotalTime[i] = 0.0;
    }
}

void Performance::StartTime(unsigned int nIndex, const char *pMsg)
{
    if(pMsg) {
        //MC_LOG << "===========================" << LOG_ENDL;
        MC_LOG << "    Start " << pMsg << LOG_ENDL;
        //MC_LOG << "===========================" << LOG_ENDL;
    }
    m_pTimer[nIndex].start();    
}

double Performance::EndTime(unsigned int nIndex, const char *pMsg)
{
    double fElapsed = (double)m_pTimer[nIndex].elapsed();

    fElapsed = fElapsed / 1000.0;
    m_pTotalTime[nIndex] += fElapsed;

    if(pMsg) {
        //MC_LOG << "===========================" << LOG_ENDL;
        MC_LOG << "    End " << pMsg << ": " << fElapsed << "s" << LOG_ENDL;
        //MC_LOG << "===========================" << LOG_ENDL;
    }

    return fElapsed;
}

void Performance::PrintTime(const char *pFileName)
{
    if(pFileName)
    {
        FILE *pFile;

        pFile = fopen(pFileName, "a+");
        if(pFile == 0) {
            MC_LOG << "Performance::Print() " << pFileName << " file not open" << LOG_ENDL;
            return;
        }

        for(int i = 0; i < PT_OBJ_SIZE; i++) {
            if(i < PT_OBJ_SIZE-1) 
                fprintf(pFile, "%.6f\t", m_pTotalTime[i]);
            else
                fprintf(pFile, "%.6f\n", m_pTotalTime[i]);
        }

        fclose(pFile);
    }
    else 
    {
        //MC_LOG << "=========================" << LOG_ENDL;
        for(int i = 0; i < PT_OBJ_SIZE; i++) 
        {
            MC_LOG << "[" << i << "/" << PT_OBJ_SIZE << "]: " << m_pTotalTime[i] << LOG_ENDL;
        }
        //MC_LOG << "=========================" << LOG_ENDL;
    }
}
