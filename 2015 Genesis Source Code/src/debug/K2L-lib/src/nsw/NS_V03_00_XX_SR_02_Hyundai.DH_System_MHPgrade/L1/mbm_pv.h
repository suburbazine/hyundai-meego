/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Private declarations and definitions of the Message Buffer 
                Management (MBM)
File:           mbm_pv.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R.Lundstrom, T.Jahnke
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
  * \brief      Private declarations and definitions of the Message Buffer 
  *             Management (MBM)
  */

#ifndef _MBM_PV_H
#define _MBM_PV_H


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mbm.h"

/*
==============================================================================
    Module internal definitions
==============================================================================
*/

#define MBM_MEM_MAX_SEG         (MBM_MSG_NUM + 1)       /*!< Max free memory segments */

/*! Minimum size of each allocated RX to make sure that they can be converted
  * into TX for special needs. */
#define MBM_RX_MIN_SIZE         ((word)(PMS_CTRL_HDR_MAX_SIZE + PMS_ERR_MSG_RSVD_PAYLOAD))

#define MBM_MEM_MAX_UNIT_SIZE   128                     /*!< The unit size is currently limited to 128 */
#define MBM_MEM_MAX_UNIT_NUM    65535                   /*!< The unit num is not allowed to exceed the range of word */

#define MBM_CFG_UNIT_SIZE   ((word)MBM_MEM_UNIT_SIZE)   /*!< Internal variant of MBM_MEM_UNIT_SIZE */
#define MBM_CFG_UNIT_NUM    ((word)MBM_MEM_UNIT_NUM)    /*!< Internal variant of MBM_MEM_UNIT_NUM */

#ifdef MDM_MBM_BUFFER_ALLOCATION_FAILED
  #ifndef MBM_MDM_TIMEOUT
    #define MBM_MDM_TIMEOUT     ((word)200)
  #endif
    #define MBM_MDM_TX          ((word)0)
    #define MBM_MDM_RX          ((word)1)
#endif

/*
==============================================================================
    Rules
==============================================================================
*/

#ifndef PACKET_COMMUNICATION_ONLY
  #ifdef ENABLE_HF_340              /* HF340 uses 3 buffers permanently */
    #if (MBM_MSG_RSVD_TX < 9)
        #error MBM_MSG_RSVD_TX too low, set to at least 9
    #endif
  #else
    #if (MBM_MSG_RSVD_TX < 6)
        #error MBM_MSG_RSVD_TX too low, set to at least 6
    #endif
  #endif   

    #if (MBM_MSG_RSVD_RX < (ADS_RX_PREALLOC + 5))
        #error MBM_MSG_RSVD_RX too low, set to at least (ADS_RX_PREALLOC + 5)
    #endif
#else
    #if (MBM_MSG_NUM < 6)
        #error MBM_MSG_NUM too low, set to at least 6
    #endif

    #if (MBM_MSG_RSVD_RX < 1)
        #error MBM_MSG_RSVD_RX too low, set to at least 1
    #endif

    #if (MBM_MSG_RSVD_TX < 1)
        #error MBM_MSG_RSVD_TX too low, set to at least 1
    #endif
#endif

#if (MBM_MSG_RSVD_RX + MBM_MSG_RSVD_TX) > MBM_MSG_NUM
    #error The sum of MBM_MSG_RSVD_RX and MBM_MSG_RSVD_TX must not be bigger than MBM_MSG_NUM
#endif

#if (MBM_MEM_UNIT_SIZE > MBM_MEM_MAX_UNIT_SIZE)
    #error MBM_MEM_UNIT_SIZE must not be larger than 128
#endif

#if (MBM_MEM_UNIT_NUM > MBM_MEM_MAX_UNIT_NUM)
    #error MBM_MEM_UNIT_NUM must not exceed 65535
#endif

#if (MBM_MEM_UNIT_SIZE * MBM_MEM_UNIT_NUM) < ((PMS_RX_PREALLOC + 2) * PMS_BUFSIZE_SMALL)
    #error (MBM_MEM_UNIT_SIZE * MBM_MEM_UNIT_NUM) is too small
#endif

/*
==============================================================================
    Module internal macros
==============================================================================
*/

/*! Calculates the number of memory units needed for a given size (in bytes) */
#define MBM_MEM_UNITS(bytes)    ((((bytes) + MBM_CFG_UNIT_SIZE) - (word)1) / MBM_CFG_UNIT_SIZE)
/*! Calculates the size in bytes for a given number of memory units */
#define MBM_MEM_BYTES(units)    ((units) * MBM_CFG_UNIT_SIZE)

/*! Updates the previous and next element in a list when a new element is inserted  */
#define MBM_MEM_LINK(prev, array, i)     (array)[i].next = (prev).next;     (prev).next     = (i)
/*! Updates affected references other elements when a list element is removed */
#define MBM_MEM_UNLINK(prev, array, i)   ((prev).next = (array)[i].next)




#ifdef MBM_USAGE_API
  /*!
    * \brief    Initializes the MBM usage information at startup
    */
  #define _MBM_USAGE_INIT()        MNS_MEM_SET((byte *)&mbmUsage, (byte)0, (word)sizeof(mbmUsage)); mbmUsage.current.fragments = 1

  /* The following macros should only be called with MX_MBM_CS locked (to avoid corrupting values) */

  /*! Allocated TX */
  #define _MBM_USAGE_UPDATE_TX()   _MbmUpdateUsageValue(msgPool.txAlloc, (word)PMS_OFFSETOF(MbmUsage,tx))
  /*! Allocated RX */
  #define _MBM_USAGE_UPDATE_RX()   _MbmUpdateUsageValue(msgPool.rxAlloc, (word)PMS_OFFSETOF(MbmUsage,rx))
  /*! Allocated handles (RX and TX) */
  #define _MBM_USAGE_UPDATE_NUM()  _MbmUpdateUsageValue((msgPool.rxAlloc + msgPool.txAlloc), (word)PMS_OFFSETOF(MbmUsage,num))
  /*! Allocated memory */
  #define _MBM_USAGE_MEM_ALLOC(sz) if(sz) _MbmUpdateUsageValue(mbmUsage.current.memory+(sz), (word)PMS_OFFSETOF(MbmUsage,memory))
  /*! Freed memory */
  #define _MBM_USAGE_MEM_FREE(sz)  if(sz) _MbmUpdateUsageValue(mbmUsage.current.memory-(sz), (word)PMS_OFFSETOF(MbmUsage,memory))
  /*! Fragments increased by 1 */
  #define _MBM_USAGE_FRAG_INC()    _MbmUpdateUsageValue(mbmUsage.current.fragments+(word)1, (word)PMS_OFFSETOF(MbmUsage,fragments))
  /*! Fragments decreased by 1 */
  #define _MBM_USAGE_FRAG_DEC()    _MbmUpdateUsageValue(mbmUsage.current.fragments-(word)1, (word)PMS_OFFSETOF(MbmUsage,fragments))

  #ifdef MBM_USAGE_CALLBACK
    /*! \brief      Declaration of an external callback function which is fired once a certain threshold is exceeded 
      */
    void MBM_USAGE_CALLBACK(MbmUsage *);
  #else
    #define MBM_USAGE_CALLBACK(x)
  #endif

  #ifdef MBM_USAGE_INIT_CALLBACK
    /*! \brief  Declaration of an external callback function which is fired to set usage a threshold during the MBM initialization 
      */
    void MBM_USAGE_INIT_CALLBACK(MbmUsage *);
  #else
    #define MBM_USAGE_INIT_CALLBACK(x)
  #endif
#else
  #define _MBM_USAGE_INIT()
  #define _MBM_USAGE_UPDATE_TX()
  #define _MBM_USAGE_UPDATE_RX()
  #define _MBM_USAGE_UPDATE_NUM()
  #define _MBM_USAGE_MEM_ALLOC(sz)
  #define _MBM_USAGE_MEM_FREE(sz)
  #define _MBM_USAGE_FRAG_INC()
  #define _MBM_USAGE_FRAG_DEC()
#endif

/*
==============================================================================
    Module internal structures
==============================================================================
*/

#ifdef MBM_MIN
    /*! Administrates the available buffer handles */
    typedef struct MbmPool
    {
        MbmQueue *q_ptr;            /*!< Queue of free buffers          */
        word      txRsvd;           /*!< Buffers reserved for TX        */
        word      rxRsvd;           /*!< Buffers reserved for RX        */
        word      txAlloc;          /*!< Currently allocated TX buffers */
        word      rxAlloc;          /*!< Currently allocated RX buffers */
        #ifdef MDM_MBM_BUFFER_ALLOCATION_FAILED
        TMostTimer mdmTxTimeout;    /*!< Timeout timer for MDM tx messages */
        TMostTimer mdmRxTimeout;    /*!< Timeout timer for MDM rx messages */
        bool       mdmTxLocked;     /*!< Flag to lock MDM tx messages */
        bool       mdmRxLocked;     /*!< Flag to lock MDM rx messages */
        #endif
    } MbmPool;

    /*! MBM Pool initialization structure */
    typedef struct MbmPoolInitStruct
    {
        MbmPool  *pool_ptr;         /*!< Pool of buffer handles */
        MbmQueue *q_ptr;            /*!< Queue of free buffers */
        MbmBuf   *firstMbmBuf_ptr;  /*!< The first MBM buffer in the queue */
        word      numDataBufs;      /*!< The total number of buffers */
        word      txRsvd;           /*!< Buffers reserved for TX */
        word      rxRsvd;           /*!< Buffers reserved for RX */
    } MbmPoolInitStruct;



    #if (MBM_MEM_MAX_SEG <= 255)
    /*!
      * MbmMemInfo is used to keep lists of free space and their sizes to
      * quickly find and allocate memory for a message.
      * To reduce memory usage a single-linked list is used with index instead
      * of pointers where index has the smallest possible type.
      *
      * For systems with a huge number of messages this memory management is
      * however quite wasteful, but such systems should use their own memory
      * manager!
      *
      * Minimal datatype needs to index all possible free segments + 1 since
      * index 0 is not a valid index for a node of this list.
      */
    typedef byte MbmMemInfoIndex;
    #else
    typedef word MbmMemInfoIndex;
    #endif

    typedef struct MbmMemInfo
    {
        MbmMemInfoIndex next;   /*!< Next MbmMemInfo */
        word start;             /*!< Start of memory array */
        word size;              /*!< Total size of array */
    } MbmMemInfo;


    #ifdef MBM_USAGE_API
    /*! \brief      All MBM usage information collected in a structure, makes 
      *             it easy to clear. 
      */
    typedef struct MbmUsageCollection
    {
        MbmUsage current;       /*!< Current memory usage */
        MbmUsage top;           /*!< Maximum memory usage */
        MbmUsage threshold;     /*!< Values for notification when violated */
    } MbmUsageCollection;
    #endif

#endif  /* #ifdef MBM_MIN */


/*
==============================================================================
    Module internal function prototypes
==============================================================================
*/

#ifdef MBM_20
    /*lint -sem(_MbmInitPool,1p)*/
    static void _MbmInitPool(const MbmPoolInitStruct *is_ptr);
#endif

#ifdef MBM_26
    /*lint -sem(_MbmInitPool,1p)*/
    static byte *_MbmMemAlloc(word *size);
#endif

#ifdef MBM_28
    static void _MbmMemInit(void);
#endif

#ifdef MBM_40
    static void _MbmUpdateUsageValue(word value, word offset_to_member);
#endif

#ifdef MBM_43
    static _INLINE void MbmSendMdmMbmBufferAllocationFailed(word type);
#endif

#ifdef MBM_46
    static void MbmMdmTimerCb(word event_flag);
#endif


#endif /* #ifndef _MBM_PV_H */

/*
==============================================================================
    End Of File
==============================================================================
*/
