/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
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
#include <DHAVN_IPodController_playControl.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

CPlayControl::CPlayControl(QString act,int transId,  bool isTidEnabled,bool isNano1GFlag)
    :CIAPPacket(0x04,0x0029,transId, isTidEnabled),m_Action(act)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    isNano1GPodcastFlag = isNano1GFlag;
}

QByteArray CPlayControl::marshall()
{
	QByteArray command;
#ifdef DBUGLOGS
	//LOG_TRACE << "In CPlayControl::marshall " << LOG_ENDL;
#endif


	//Packing format =  Big Endian
	if(!m_Action.isEmpty()){
		int i = -1;
		//    command.resize(10);
		command[++i] = 0x55;
		command[++i] = 0x06;
		command[++i] = 0x04;
		command[++i] = 0x00;
		command[++i] = 0x29;
		if( m_isTidEnabled )
		{
			command[++i] = ((m_TransactionID & 0xff00) >> 8);
			command[++i] = m_TransactionID & 0x00ff;
		}
		if( (!m_Action.compare("Play",Qt::CaseInsensitive))
				|| (!m_Action.compare("Pause",Qt::CaseInsensitive)) )
			command[++i] = 0x01 ;
		else if(!m_Action.compare("Stop",Qt::CaseInsensitive))
			command[++i] = 0x02;
		else if(!m_Action.compare("Next",Qt::CaseInsensitive))
                {
                    if(isNano1GPodcastFlag)
                    {
                            command[++i] = 0x03;
                    }
                    else
                    {
                            command[++i] = 0x08;
                    }
                }
		else if(!m_Action.compare("Previous",Qt::CaseInsensitive))
		{
			//CR 15854: Playcontrol command 09 for previous is not working for nano1G.
			//So sending 04 deprecated command to fix the issue.
			if(isNano1GPodcastFlag)
			{
				command[++i] = 0x04;
			}
			else
			{
				command[++i] = 0x09; //CR 13730: ATS Warning fix  0x04; Deprecated in R45 Spec
			}
		}
		else if(!m_Action.compare("FastForward",Qt::CaseInsensitive))
			command[++i] = 0x05;
		else if(!m_Action.compare("Rewind",Qt::CaseInsensitive))
			command[++i] = 0x06;
		else if(!m_Action.compare("StopSeek",Qt::CaseInsensitive))
			command[++i] = 0x07;
		else if(!m_Action.compare("ResumeiPod",Qt::CaseInsensitive))
			command[++i] = 0x0E;

		command[1] = i-1;
		command.append(CIpodControllerGlobal::checkSum(command, i));
	}
	m_CommandBytes = command;
	return command;
}

void  CPlayControl::unmarshall(QByteArray )
{
}




