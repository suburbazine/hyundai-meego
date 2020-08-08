#include "DHAVN_EventLoop.h"
#include <stdio.h>


CIpodEventLoop::CIpodEventLoop(int nExitCode)
{
    m_nExitCode = nExitCode;
}

int CIpodEventLoop::nGetExitCode()
{
    return m_nExitCode;
}

void CIpodEventLoop::mainLoopQuitSlot(int nExitCode)
{
    if(m_nExitCode == nExitCode || nExitCode == ExitCode_AllExit)
    {
        exit(0);
    }
    if(nExitCode == IPOD_ERROR_TIMEOUT)
    {
        exit(0);
    }
}

