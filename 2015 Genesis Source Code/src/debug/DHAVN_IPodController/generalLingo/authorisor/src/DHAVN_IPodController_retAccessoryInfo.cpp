
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
#include <DHAVN_IPodController_retAccessoryInfo.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace GeneralLingo;

CRetAccessoryInfo::CRetAccessoryInfo(int transIdArgs,int infoType,SAccessoryInfo info) : m_TransId(transIdArgs),m_InfoType(infoType),m_Info(info)
{
//    LOG_INIT_V2(gControllerLogName);
//    //LOG_TRACE << "CRetAccessoryInfo::CRetAccessoryInfo" << LOG_ENDL;
}



QByteArray CRetAccessoryInfo::marshall()
{
	switch(m_InfoType)
	{
		case CAPABILITIES:
			{
				QByteArray command;

				//Packing format =  Big Endian

//				command.resize(11);

				command[0] = 0x55;
				command[1] = 0x07;
				command[2] = 0x00;
				command[3] = 0x28;
				command[4] = 0x00; // acc info type = 0x00
				command[5] = 0x00;//31-24
                                command[6] = 0x04;//23-16
                                command[7] = 0x00;//
                                command[8] = 0xF3;
				command[9] = CIpodControllerGlobal::checkSum(command,8);

				m_CommandBytes = command;
				break;
			}
		case ACCESSORYNAME:
			{
				QByteArray command;

				//Packing format =  Big Endian

//				command.resize(11);

				command[0] = 0x55;
				command[1] = 0x00;//dummy value
				command[2] = 0x00;
				command[3] = 0x28;
				command[4] = 0x01; // acc info type = 0x01

				QByteArray str = m_Info.m_StringToSend.toUtf8();
				if(str.length() > 63)
				{
					str.chop(64);
				}
				str.append('\0');
				command.append(str);
				//command.append('\0');

				command[1] = 3 + str.length();
				int nChecksumIndex = str.length()+5;
				command[nChecksumIndex] = CIpodControllerGlobal::checkSum(command,(nChecksumIndex-1) ) ;

				m_CommandBytes = command;
				break;
			}
        case ACCESSORYFIRMWAREVERSION:
            {
                                QByteArray command;

                                //Packing format =  Big Endian

//				command.resize(11);

                                command[0] = 0x55;
                                command[1] = 0x06;
                                command[2] = 0x00;
                                command[3] = 0x28;
                                command[4] = 0x04; // acc info type = 0x00
                                command[5] = 0x00;//31-24
                                command[6] = 0x00;//23-16
                                command[7] = 0x01;//15-8
                                command[8] = CIpodControllerGlobal::checkSum(command,7);

                                m_CommandBytes = command;
                                break;

                            }
        case ACCESSORYHARDWAREVERSION:
            {
                                QByteArray command;

                                //Packing format =  Big Endian

//				command.resize(11);

                                command[0] = 0x55;
                                command[1] = 0x06;
                                command[2] = 0x00;
                                command[3] = 0x28;
                                command[4] = 0x05; // acc info type = 0x00
                                command[5] = 0x00;//31-24
                                command[6] = 0x00;//23-16
                                command[7] = 0x01;//15-8
                                command[8] = CIpodControllerGlobal::checkSum(command,7);

                                m_CommandBytes = command;
                                break;

                            }
		case ACCESSORYMANUFACTURER:
			{
				QByteArray command;

				//Packing format =  Big Endian

//				command.resize(11);

				command[0] = 0x55;
				command[1] = 0x00;//dummy value
				command[2] = 0x00;
				command[3] = 0x28;
				command[4] = 0x06; // acc info type = 0x01

				QByteArray str = m_Info.m_StringToSend.toUtf8();
				if(str.length() > 63)
				{
					str.chop(64);
				}
				str.append('\0');
				command.append(str);
				//command.append('\0');

				command[1] = 3 + str.length();
				int nChecksumIndex = str.length()+5;
				command[nChecksumIndex] = CIpodControllerGlobal::checkSum(command,(nChecksumIndex-1) ) ;

				m_CommandBytes = command;
				break;
			}
		case ACCESSORYMODELNUMBER:
			{
				QByteArray command;

				//Packing format =  Big Endian

//				command.resize(11);

				command[0] = 0x55;
				command[1] = 0x00;//dummy value
				command[2] = 0x00;
				command[3] = 0x28;
				command[4] = 0x07; // acc info type = 0x01

				QByteArray str = m_Info.m_StringToSend.toUtf8();
				if(str.length() > 63)
				{
					str.chop(64);
				}
				str.append('\0');
				command.append(str);
				//command.append('\0');

				command[1] = 3 + str.length();
				int nChecksumIndex = str.length()+5;
				command[nChecksumIndex] = CIpodControllerGlobal::checkSum(command,(nChecksumIndex-1) ) ;

				m_CommandBytes = command;
				break;
			}
		default :
			break;
	}
//        //LOG_TRACE << "in CRetAccessoryInfo marshall " << LOG_ENDL;
	return m_CommandBytes;

}

void  CRetAccessoryInfo::unmarshall(QByteArray )
{
        return;
}
