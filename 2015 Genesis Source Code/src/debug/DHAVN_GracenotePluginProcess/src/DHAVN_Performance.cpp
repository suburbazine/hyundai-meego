#include "DHAVN_Performance.h"
#include <stdio.h>
#include <sys/time.h>
#include <qdebug.h>

int Performance::m_bFirstPrint = true;
#if USE_QELAPASED_TIMER
QElapsedTimer Performance::m_pTimer[PT_OBJ_SIZE] = {QElapsedTimer(), };
#else
double Performance::m_pStartTime[PT_OBJ_SIZE] = {0,0, };
#endif
double Performance::m_pTotalTime[PT_OBJ_SIZE] = {0.0, };
int Performance::m_pCallCount[PT_OBJ_SIZE] = {0, };
int Performance::m_FileCount = 0;

char *g_ppPTObjName[PT_OBJ_SIZE] = {    
    "TrackerMining",
    "UpdateFilesInUSB",
    "UpdateFilesInJukebox",
    "UpdateMLDBForJukeBox",
    "UpdateMLDBForUSB",
    "CheckIfExist",
    "ParseFilePath",
    "ExtractID3Tag",
    "TextIDLookup",
    "MLDBPopulate",
    "CreateMusicData",
    "ExtractXSampa",
    "TrackerUpdate",
    "UpdateMLDBForIPOD",
    "ReadDataFromIPODDB",
    "Etc"
}; 


void Performance::Reset()
{
    for(int i = 0; i < PT_OBJ_SIZE; i++) {
        m_pTotalTime[i] = 0.0;
    }
}

void Performance::StartTime(unsigned int nIndex, const char *pMsg)
{
    if(pMsg) {
        //qDebug("===========================");
        qDebug("    Start %s", pMsg);
        //qDebug("===========================");
    }
    
    #if USE_QELAPASED_TIMER
    m_pTimer[nIndex].start();    
    #else
    timeval tim;
    gettimeofday(&tim, NULL);
    m_pStartTime[nIndex] = (double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
    #endif
}

double Performance::EndTime(unsigned int nIndex, const char *pMsg)
{
    #if USE_QELAPASED_TIMER
    double fElapsed = (double)m_pTimer[nIndex].elapsed();
    fElapsed = fElapsed / 1000.0;
    m_pTotalTime[nIndex] += fElapsed;
    #else
    timeval tim;
    gettimeofday(&tim, NULL);
    double fElapsed = (double)tim.tv_sec + (double)tim.tv_usec / 1000000.0 - m_pStartTime[nIndex];
    #endif
    m_pTotalTime[nIndex] += fElapsed;

    if(pMsg) {
        //qDebug("===========================");
        qDebug("    End %s: %.6fs", pMsg, fElapsed);
        //qDebug("===========================");
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
            qDebug("Performance::Print() %s file not open", pFileName);
            return;
        }



        if(m_bFirstPrint == true) {
            //if(m_FileCount > 0)
            //{
                fprintf(pFile, "%s\t", "Updated FileCount");
           // }
            m_bFirstPrint = false;
            for(int i = 0; i < PT_OBJ_SIZE; i++) {
                if(i < PT_OBJ_SIZE-1) 
                    fprintf(pFile, "%s\t", g_ppPTObjName[i]);
                else
                    fprintf(pFile, "%s\n", g_ppPTObjName[i]);
            }
        }

        if(m_FileCount >= 0)
        {
            fprintf(pFile, "%d\t", m_FileCount);
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
        qDebug("=========================");
        for(int i = 0; i < PT_OBJ_SIZE; i++) 
        {
            qDebug("[%d/%d]: %.6f", i, PT_OBJ_SIZE, m_pTotalTime[i]);
        }
        qDebug("=========================");
    }
}

void Performance::SetFileCount(int count)
{
    m_FileCount = count;
}
