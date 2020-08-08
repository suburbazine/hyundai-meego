/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IpodController                                    *
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
#include <DHAVN_IPodController_returnIndexedPlayingTrackInfo.h>
#include <QByteArray>
#include <DHAVN_IPodController_CommonVars.h>

using namespace ExtendedLingo;

CReturnIndexedPlayingTrackInfo::CReturnIndexedPlayingTrackInfo( bool isTidEnabled)
    :CIAPPacket(0x04,0x000D,-1, isTidEnabled), m_TrackDuration(0),m_infoType(-1),m_Genre(""),m_Composer("")
{
    m_artworkFormatId = -1;
    m_artworkcount = 0;
    m_TrackTypeInfo = MUSICTRACKS; //CR 15209 added by jungae 2012.10.29, Static_Test 0134443

}

QByteArray CReturnIndexedPlayingTrackInfo::marshall()
{
        QByteArray command;
        return command;
}

int CReturnIndexedPlayingTrackInfo::getTrackDuration()
{
    return m_TrackDuration;
}

int CReturnIndexedPlayingTrackInfo::getInfoType()
{
    return m_infoType;
}

QString CReturnIndexedPlayingTrackInfo::getGenre()
{
    return m_Genre.fromUtf8(m_Genre.toAscii().data());
}

QString CReturnIndexedPlayingTrackInfo::getComposer()
{
    return m_Composer.fromUtf8(m_Composer.toAscii().data());
}

int CReturnIndexedPlayingTrackInfo::getTypeInfo()
{
    return m_TrackTypeInfo;
}

int CReturnIndexedPlayingTrackInfo::getArtworkFormatId()
{
    return m_artworkFormatId;
}

int CReturnIndexedPlayingTrackInfo::getArtworkFormatCount()
{
    return m_artworkcount;
}

void  CReturnIndexedPlayingTrackInfo::unmarshall(QByteArray returnCommand)
{
    //qDebug() << "inCReturnIndexedPlayingTrackInfo  unmarshall\n";
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
		    ((int)returnCommand[2] == 0x04) &&
		    ((int)returnCommand[3] == 0x00) &&
		    ((int)returnCommand[4] == 0x0D) ){
	    int nStartIndex = -1;
	    int nEndIndex = -1;

            int nStartIndex_Typeinfo = -1;
            int nEndIndex_Typeinfo = -1;

            if( m_isTidEnabled )
	    {
		    m_TransactionID = 0x0000;
		    m_TransactionID = m_TransactionID | returnCommand[5];
		    m_TransactionID = m_TransactionID <<8;
		    m_TransactionID = m_TransactionID | returnCommand[6];
		    m_infoType = (int)(0x00 | returnCommand[7]);
		    nStartIndex = 12;
		    nEndIndex = 16;

                    nStartIndex_Typeinfo = 11;
                    nEndIndex_Typeinfo = 10;
	    }
	    else
	    {
		    nStartIndex = 10;
		    nEndIndex = 14;

                    nStartIndex_Typeinfo = 9;
                    nEndIndex_Typeinfo = 8;

		    m_infoType = (int)(0x00 | returnCommand[5]);
	    }

        switch(m_infoType)
        {
        case 0x00:
            {
                int nTrackInfoDetails = 0; 
                for(int i = nStartIndex;i<nEndIndex;i++)
                {
                         nTrackInfoDetails <<= 8;     nTrackInfoDetails |= (int)returnCommand[i] & 0xFF;
                }
                m_TrackDuration =  nTrackInfoDetails;                

                        if (((int)returnCommand[nStartIndex_Typeinfo] & 0x01)==0x01){                        
                            //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: Value of Capability info=%d",(int)returnCommand[nStartIndex_Typeinfo]);
                            m_TrackTypeInfo=AUDIOBOOK;
                        }
                        else if(((int)returnCommand[nStartIndex_Typeinfo] & 0x10)==0x10){                      
                            //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: Value of Capability info=%d",(int)returnCommand[nStartIndex_Typeinfo]);
                            m_TrackTypeInfo=PODCAST;
                        }
                        else if(((int)returnCommand[nStartIndex_Typeinfo] & 0x80)==0x80){   // For video Podcast                            
                            //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: Value of Capability info=%d",(int)returnCommand[nStartIndex_Typeinfo]);
                            m_TrackTypeInfo=PODCAST;
                        }
                        else if(((int)returnCommand[nEndIndex_Typeinfo] & 0x40)==0x40){
                            //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: Value of Capability info=%d",(int)returnCommand[nStartIndex_Typeinfo]);
                            m_TrackTypeInfo=ITUNESU;
                        }
                        else{
                            //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: Type of song is MUSICTRACKS : Pradeep");
                            m_TrackTypeInfo=MUSICTRACKS;
                        }


                break;
            }
          case 0x06:
	    {
                    if( m_isTidEnabled )
			    m_Composer = QString(returnCommand.mid(8,-1)); // bytearray from 12 to the end
		    else
			    m_Composer = QString(returnCommand.mid(6,-1)); // bytearray from 12 to the end
		    break;
            }
          case 0x05:
	    {
                    if( m_isTidEnabled )
			    m_Genre = QString(returnCommand.mid(8,-1)); // bytearray from 12 to the end
		    else
			    m_Genre = QString(returnCommand.mid(6,-1)); // bytearray from 12 to the end
		    break;
            }
        case 0x07:
            {
                    int dataSize = 0 | returnCommand[1];
                    int i = 6;
                    int k = 0;
                    dataSize -= 4;
                    if( m_isTidEnabled  ){
                        i = 8;
                        dataSize -= 2;
                    }
                    k = dataSize/4;// Give Number of format-count records.
                    //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo: size=%d, k=%d",dataSize,k);
                    for(; k > 0 ; k--){
                        //m_artworkFormatId = 0 | ((0x000000FF && returnCommand[i])<<8) | ((0x000000FF && returnCommand[i+1]));
//                        m_artworkFormatId = 0| returnCommand[i];
//                        m_artworkFormatId = m_artworkFormatId << 8;
//                        m_artworkFormatId = m_artworkFormatId | returnCommand[i+1];
                        //DBGLOGFun("CArtworkFetchTaskHandler::CReturnIndexedPlayingTrackInfo:m_artworkFormatId=0x%x, %x, %x",m_artworkFormatId,(int)returnCommand[i],(int)returnCommand[1+1]);

                        //CR 16144,15669 :  Changed for proper artwork fetching S

                        //there is a bug when we calculate the format id.

                        int nArtworkFormatId;
                        nArtworkFormatId = (0x000000FF & returnCommand[i]);
                        nArtworkFormatId = nArtworkFormatId<<8;
                        nArtworkFormatId = nArtworkFormatId | (0x000000FF &returnCommand[i+1]);
                        m_artworkFormatId = nArtworkFormatId;

                        /*
                        QString string = QString::number((int)returnCommand[i+1], 16);
                        QString subString = string.mid((string.length()-2),2);
                        subString = ((QString::number((int)returnCommand[i], 16)).append(subString));
                        // Converting a hex string back to a number
                        bool ok;
                        m_artworkFormatId = QString(subString).toInt(&ok ,16);
                        */
                        //CR 16144,15669 :  Changed for proper artwork fetching E

                        m_artworkcount = 0 | ((0x00FF && returnCommand[i+2])<<8) | ((0x00FF && returnCommand[i+3]));

                        if(m_artworkcount > 0){
                            break;
                        }
                        i +=4;
                    }

                    break;

            }
        default:
            {
                break;
            }
        }
    //qDebug() <<  "\n\n m_TrackDuration =  " << m_TrackDuration;
  }
}
