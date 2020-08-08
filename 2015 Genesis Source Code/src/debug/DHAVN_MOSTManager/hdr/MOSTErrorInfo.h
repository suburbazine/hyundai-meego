/*=== K2L GmbH ===============================================================*/
/**
file        MOSTErrorInfo.h
            
            Example ACS code for extracting error information from ACS internal
            message object. Please note that this is just a workarroung and
            is not intended to be standard way of doing that.

            K2L will provide this feature in standard ACS product and when
            available please swith to the standard interface provided inside
            namespace k2l::acs::protocols::most.

            This is sample code and not intended for production code.
            You may copy and use it at your own risk. K2L does not grant express or
            implied warrenties for sample code.

            COPYRIGHT (c) 2008-2012 by K2L GmbH
            All rights reserved.

            File History  
      
Date        Author Scope
--------------------------------------------------------------------------------
2012-05-04  bko   Initial version.
*/

/*==============================================================================
            Alle Rechte an der Software verbleiben bei der K2L GmbH.

            Die K2L GmbH raeumt dem Lizenznehmer nach vollstaendiger
            Bezahlung der Lizenzgebuehr ein nicht ausschliessliches,
            unwiderrufliches, nicht uebertragbares, zeitlich und geografisch
            nicht beschraenktes Recht zur Nutzung der jeweils
            auftragsgegenstaendlichen Software innerhalb des Lizenznehmers ein.

            Die Weitergabe des Nutzungsrechts durch den Lizenznehmer ist auf
            dessen Zulieferer beschraenkt. Die Zulieferer sind zu verpflichten,
            die Software nur im Rahmen von Projekten fuer den Lizenznehmer
            zu verwenden; weitere Rechte der Zulieferer sind auszuschliessen.
===HEADER END=================================================================*/


#ifndef _MOSTERRORINFO_H_
#define _MOSTERRORINFO_H_

#include <stdint.h>
#include <K2LSystem.h>
#include <K2LACSClient.h>
#include <ACSMessage.h>
#include <ACSStreams.h>
#include <K2LACSMOST.h>
#include <K2LACSEvents.h>
#include <K2LDebug.h>
#include "DHAVN_MOSTManager_ACSApplication.h"
#include <MOSTProtocolDefinitions.h>

namespace k2l
{
    namespace acs
    {
        namespace samples
        {
            class CMOSTErrorInfo
            {
            public:
                // This method is just a workarround to be able to retrieve errorCode and errorInfo from MOST Error Operation.
                // You need to give container pointer to be able to retrieve errorCode. This is done indirectly through shadow/fblock
                // pointer: GetMOSTError(m_tuner.Container(), .....)
                static bool GetMOSTError(
                    k2l::acs::protocols::most::CMOSTElements* pContainer,   //[in]
                    uint8_t& errorCode,                                     //[out]
                    uint32_t& errorInfoLength,                              //[out]
                    const uint8_t*& errorInfo                               //[out]
                    )
                {
                    bool parsed = false;

                    //Retrieve container pointer so that we can get low level ACS Message object to retrieve error information.
                    if (NULL != pContainer)
                    {
                        //Retrieve ACS message from container object. Please note that you are NOT ALLOWED to release this object here.
                        //This will be done by Container() object.
                        k2l::acs::base::CACSMessage* pCurrentMessage = pContainer->GetCurrentAcsMessage();
                        if (NULL != pCurrentMessage)
                        {
                            const uint8_t* cpACSPayload = NULL;
                            uint32_t acsMessageLength = 0;
                            if (pCurrentMessage->GetSegment(
                                k2l::acs::base::CACSMessage::LastSegmentIndex(),
                                &cpACSPayload,
                                acsMessageLength
                                ))
                            { 
                                //Internal protocol knowledge of the ACS message. Please do not use this directly in your code because it can change.
                                const uint32_t OFFSET_MOST_PAYLOAD = 18;    
                                const uint32_t OFFSET_MESSAGE_TYPE = 0;
                                const uint32_t OFFSET_OPERATION_TYPE = 15;
                                const uint32_t MESSAGE_TYPE_DATA_MESSAGE = 12;

                                //Check if we received MOST data message and operation type error (it is not internal error)
                                if (acsMessageLength >= OFFSET_MOST_PAYLOAD)
                                {
                                    if ((cpACSPayload[OFFSET_MESSAGE_TYPE] == MESSAGE_TYPE_DATA_MESSAGE) &&
                                        (cpACSPayload[OFFSET_OPERATION_TYPE] == k2l::acs::base::most::OPTYPE_ERROR))
                                    {
                                        const uint8_t* pErrorPayload = cpACSPayload + OFFSET_MOST_PAYLOAD;
                                        const uint32_t errorPayloadLength = acsMessageLength - OFFSET_MOST_PAYLOAD;
                                        if (errorPayloadLength >= 3)
                                        {
                                            k2l::acs::protocols::most::CACSMOSTReader reader(pErrorPayload, errorPayloadLength);

                                            if (reader.Read8(&errorCode))
                                            {
                                                //Following parameter is optional and depends on error code.
                                                errorInfoLength = reader.Capacity() - reader.Position();
                                                errorInfo = reader.PointerAtCurrentPosition();
                                                parsed = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    return parsed;
                }
            };
        }
    }
}

#endif

