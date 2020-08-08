/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_retiPodOptionsForLingo.h>
#include <iostream>
#include <DHAVN_IPodController_CommonVars.h>


using namespace GeneralLingo;

CRetiPodOptionsForLingo::CRetiPodOptionsForLingo(int transIdArgs,bool isTidEnabled) : m_TransId(transIdArgs),m_LingoType(0),m_iTunesTaggingSupport(false),m_isLogoSupported(false)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    m_isTidEnabled = isTidEnabled;
    m_ExtendedLingoVersion = 0.0;
    m_iOSAppCommunicationSupported = false;
    ////LOG_TRACE << "in CRetiPodOptionsForLingo constructor " << LOG_ENDL;
}



QByteArray CRetiPodOptionsForLingo::marshall()
{
    return NULL;
}

void  CRetiPodOptionsForLingo::unmarshall(QByteArray returnCommand)
{

    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x00) &&
            ((int)returnCommand[3] == 0x4C) )
    {
        quint64 dOptionBits = 0 ;
        if( m_isTidEnabled )
        {
            m_TransactionID = 0;
            int nStartIndex = 4;
            for(int i = nStartIndex;i < nStartIndex + 2;i++)
            {
                m_TransactionID <<= 8;     m_TransactionID |= (int)returnCommand[i] & 0xFF;
            }

            m_LingoType = (int)returnCommand[6];

            nStartIndex = 7;
            for(int i = nStartIndex;i< nStartIndex+8;i++)
            {
                dOptionBits <<= 8;
                dOptionBits |= (int)returnCommand[i] & 0xFF;
            }
        }
        else
        {
            int nStartIndex;
            m_LingoType = (int)returnCommand[4];
            nStartIndex = 5;
            for(int i = nStartIndex;i< nStartIndex+8;i++)
            {
                dOptionBits <<= 8;     dOptionBits |= (int)returnCommand[i] & 0xFF;
            }
        }

        switch(m_LingoType)
        {
        case 0x00: //general lingo
            if(dOptionBits & 0x2000 ) // 13 bit in general lingo must be set to support the ios app commincation
            {
                m_iOSAppCommunicationSupported = true;
            }
            else
            {
                m_iOSAppCommunicationSupported = false;
            }
            break;
        case 0x0C:
            if(dOptionBits & 0x0001 )
            {
                m_iTunesTaggingSupport = true;
            }
            break;
        case 0x04:
            if( (dOptionBits & 16 ) == 16)
            {
                m_isLogoSupported = true;
            }
            // to get the lingo protocol version we are making the commands as the base
            //like if the GetDBTrackInfo is supported then device version is >= 1.13
            // if GetArtWorkTimes is supported then device version is >=1.14
            if((dOptionBits & 128) && (dOptionBits & 2))//both are supported
            {
                m_ExtendedLingoVersion = 1.14;
            }
            else if(dOptionBits & 2)// only GetDBTrackInfo is supported
            {
                m_ExtendedLingoVersion = 1.13;
            }
            else //non is supported
            {
                m_ExtendedLingoVersion = 1.12;
            }
            break;
        }
    }
}

int CRetiPodOptionsForLingo::getLingoType()
{
    return m_LingoType;
}

bool CRetiPodOptionsForLingo::isiTunesTaggingSupported()
{
    return m_iTunesTaggingSupport;
}
bool CRetiPodOptionsForLingo::isLogoSupported()
{
    return m_isLogoSupported;
}

double CRetiPodOptionsForLingo::getExtendedLingoVersion()
{
    return m_ExtendedLingoVersion;
}

bool CRetiPodOptionsForLingo::isiOSAppCommunicationSupported()
{
    return m_iOSAppCommunicationSupported;
}
