/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of the Asynchronous Data Service Wrapper (WADS)
File:           wads_pb.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      S.Kerber, S.Semmler
Date:           31.January.2012

FileGroup:      Layer I
Customer ID:    34C40C060300xx.HYUNDAI.DH
FeatureCode:    FCR1
------------------------------------------------------------------------------

                (c) Copyright 1998-2012
                SMSC
                All Rights Reserved

------------------------------------------------------------------------------



Modifications
~~~~~~~~~~~~~
Date            By      Description

==============================================================================
*/

/*! \file
  * \brief      Public API of the Asynchronous Data Service Wrapper (WADS)
  */

#ifndef _ADS_PB_H
#define _ADS_PB_H

#include "mostdef1.h"   /* Definition File for MOST NetServices Basic Layer */

/*
==============================================================================
    ADS Service flags (public only for trace purposes)
==============================================================================
*/

#define ADS_P_NONE              ((word) 0x0000)         /* general 'none' */

#define ADS_P_RX                ((word) 0x0001)
/*
#define ADS_P_FREE2             ((word) 0x0002)
#define ADS_P_FREE3             ((word) 0x0004)
#define ADS_P_FREE4             ((word) 0x0008)
#define ADS_P_FREE5             ((word) 0x0010)
#define ADS_P_FREE6             ((word) 0x0020)
#define ADS_P_FREE7             ((word) 0x0040)
#define ADS_P_FREE8             ((word) 0x0080)
#define ADS_P_FREE9             ((word) 0x0100)
#define ADS_P_FREE10            ((word) 0x0200)
#define ADS_P_FREE11            ((word) 0x0400)
#define ADS_P_FREE12            ((word) 0x0800)
#define ADS_P_FREE13            ((word) 0x1000)
#define ADS_P_FREE14            ((word) 0x2000)
#define ADS_P_FREE15            ((word) 0x4000)
#define ADS_P_FREE16            ((word) 0x8000)
*/
#define ADS_P_FIRST             (word) ADS_P_RX
#define ADS_P_LAST              (word) ADS_P_RX

#define DATA_RCV_TYPE_LOGICAL     ((byte) 0x00) /* aimed to logical node addr               */
#define DATA_RCV_TYPE_NODE_POS    ((byte) 0x01) /* aimed to node pos addr                   */
#define DATA_RCV_TYPE_BROADCAST   ((byte) 0x02) /* received via broadcast                   */
#define DATA_RCV_TYPE_GROUPCAST   ((byte) 0x03) /* received via groupcast                   */
#define DATA_RCV_TYPE_ALTERNATIVE ((byte) 0x04) /* received via alternative packet address  */

#define DATA_RX_FILTER_DEFAULT  ((byte) 0x00)   /* pass to rx_complete */
#define DATA_RX_FILTER_FREE     ((byte) 0x01)   /* done, free the rx ptr */
#define DATA_RX_FILTER_BUSY     ((byte) 0x02)   /* currently busy, re-call */

#define DATA_RX_COMPLETE_FREE   ((byte) 0x01)   /* done, free the rx ptr */
#define DATA_RX_COMPLETE_BUSY   ((byte) 0x02)   /* currently busy, re-call */

#ifdef ADS_4
    TDataTx * DataGetTxPtrExt(word size);
#endif

#ifdef ADS_5
    void DataSend(TDataTx *msg_ptr);
#endif

#ifdef ADS_11
    byte DataRequestStatus(void);
#endif


#ifdef NS_WADS_MHP
    byte MhpRxTriggerAds(pcTDataRx ptr_ads);
    void MhpTxFrameComplete(void);
    void MhpTxEndAllCon(void);
#endif

#if (defined PACKET_COMMUNICATION_ONLY) && (defined _OS81110_NA_MDP)
typedef struct TDataNAOverMDP
{
    /*! \brief Property INIC.NIState */
    byte ni_state;

    /*! \brief System state */
    byte sys_state;

    /*! \brief Property NetBlock.Nodeaddress */
    word node_addr;

    /*! \brief Property NetBlock.EUI48 */
    byte eui48[6];

} TDataNAOverMDP;

#endif

#endif /* _ADS_PB_H */

