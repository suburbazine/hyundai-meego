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
#include <DHAVN_IPodController_taskHandler.h>

CTaskHandler::CTaskHandler(QObject* parent) : QObject(parent)
{
}

void CTaskHandler::packetAvailable(int /*lingoID*/,int /*commandID*/,const QByteArray& /*buf*/ )
{

}

/* Stops the current task */
void CTaskHandler::stopCurrentTask()
{

}

/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/
void CTaskHandler::stopTaskHandler()
{

}

