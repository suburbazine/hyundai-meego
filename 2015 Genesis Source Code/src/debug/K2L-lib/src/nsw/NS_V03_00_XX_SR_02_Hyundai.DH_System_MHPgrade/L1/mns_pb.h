/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of the MOST NetServices Kernel (Basic Layer)
File:           mns_pb.h
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
  * \brief      Public API of the MOST NetServices Kernel (Basic Layer)
  */

#ifndef _MNS_PB_H
#define _MNS_PB_H

#include "mostdef1.h"
#include "wads_pb.h"

#define MNS_P_NONE          ((word) 0x0000)

#define MNS_P_SRV_PMS       ((word) 0x0001)
#define MNS_P_SRV_MIS       ((word) 0x0002)
#define MNS_P_SRV_MNS       ((word) 0x0004)

#ifdef AMS_MIN
    #define MNS_P_SRV_AMS   ((word) 0x0008)
#else
    #define MNS_P_SRV_AMS   MNS_P_NONE
#endif

#ifdef MCS_MIN
    #define MNS_P_SRV_WMCS  ((word) 0x0010)
#else
    #define MNS_P_SRV_WMCS  MNS_P_NONE
#endif

#ifdef ADS_MIN
    #define MNS_P_SRV_WADS  ((word) 0x0020)
#else
    #define MNS_P_SRV_WADS  MNS_P_NONE
#endif

#ifdef VMSV_MIN
    #define MNS_P_SRV_VMSV  ((word) 0x0040)
#else
    #define MNS_P_SRV_VMSV  MNS_P_NONE
#endif

#ifdef SCM_MIN
    #define MNS_P_SRV_WSCM  ((word) 0x0080)
#else
    #define MNS_P_SRV_WSCM  MNS_P_NONE
#endif

#ifdef AAM_MIN
    #define MNS_P_SRV_AAM     ((word) 0x0100)
#else
    #define MNS_P_SRV_AAM     MNS_P_NONE
#endif

#ifdef GAS_MIN
    #define MNS_P_SRV_GAS     ((word) 0x0200)
#else
    #define MNS_P_SRV_GAS     MNS_P_NONE
#endif

#define MNS_P_SRV_FREE11        ((word) 0x0400)
#define MNS_P_SRV_FREE12        ((word) 0x0800)


#ifdef FSM_MIN
    #define MNS_P_SRV_FSM       ((word) 0x1000)
#else
    #define MNS_P_SRV_FSM       MNS_P_NONE
#endif

#ifdef SCMPM_MIN
    #define MNS_P_SRV_SCMPM     ((word) 0x2000)
#else
    #define MNS_P_SRV_SCMPM     MNS_P_NONE
#endif

#ifdef MDM_MIN
    #define MNS_P_SRV_MDM       ((word) 0x4000)
#else
    #define MNS_P_SRV_MDM       MNS_P_NONE
#endif

#define MNS_P_SRV_MBM           ((word) 0x8000)

/*
==============================================================================
    Service Range and Check Aliases
==============================================================================
*/


#define MNS_P_SRV_FIRST         ((word) 0x0001)
#define MNS_P_SRV_LAST          ((word) 0x0200)

#define MNS_P_SRV_COMM          ((word) ( MNS_P_SRV_PMS  | MNS_P_SRV_MIS  | MNS_P_SRV_AMS  ))
#define MNS_P_SRV_BASE          ((word) ( MNS_P_SRV_COMM | MNS_P_SRV_MNS  | MNS_P_SRV_WADS | MNS_P_SRV_GAS ))
#define MNS_P_SRV_SHADOW        ((word) ( MNS_P_SRV_WMCS | MNS_P_SRV_VMSV | MNS_P_SRV_WSCM ))
#define MNS_P_SRV_ALL           ((word) ( MNS_P_SRV_BASE | MNS_P_SRV_SHADOW ))

#define MNS_P_SRV_ALL_RESET     ((word) (MNS_P_SRV_ALL & (word)~( MNS_P_SRV_PMS  | MNS_P_SRV_MIS  | MNS_P_SRV_WADS )))

/*
==============================================================================
    MNS Service flags (public only for trace purposes)
==============================================================================
*/

#define MNS_P_ALL_SERVICES_READY    ((word) 0x0001)
#define MNS_P_AS_GO_PROTECTED       ((word) 0x0002)
#define MNS_P_BUF_FREED             ((word) 0x0004)
#define MNS_P_AS_GO_SEMI_PROTECTED  ((word) 0x0008)
#define MNS_P_AS_GO_ATTACHED        ((word) 0x0010)
#define MNS_P_VI_RETRY              ((word) 0x0020)
#define MNS_P_INIT_TIMER            ((word) 0x0040)
#define MNS_P_L2                    ((word) 0x0080)
#define MNS_P_MHP                   ((word) 0x0100)
#define MNS_P_PMHS                  ((word) 0x0200)
#define MNS_P_WATCHDOG_TIMER        ((word) 0x0400)
#define MNS_P_NTF_COMPLETE          ((word) 0x0800)
#define MNS_P_SRV_CHECK             ((word) 0x1000)
#define MNS_P_AS_GO_NET_OFF         ((word) 0x2000)
#define MNS_P_START_GAS_MODE        ((word) 0x4000)
#define MNS_P_FIRST                     MNS_P_ALL_SERVICES_READY
#define MNS_P_LAST                      MNS_P_START_GAS_MODE

#define MNS_P_PACKET_LAYERS         (MNS_P_MHP | MNS_P_PMHS)

/*! INIC.Bandwidth.Status */
#define NTF_BANDWIDTH                          ((dword) 0x00000001)
/*! INIC.MuteMode.Status */
#define NTF_MUTEMODE                           ((dword) 0x00000002)
/*! INIC.NodeDelay.Status */
#define NTF_NODEDELAY                          ((dword) 0x00000004)
/*! NB.NodeAddress.Status */
#define NTF_NODE_ADDR                          ((dword) 0x00000008)
/*! NB.GroupAddress.Status */
#define NTF_GROUP_ADDR                         ((dword) 0x00000010)
/*! INIC.NIState.Status */
#define NTF_NISTATE                            ((dword) 0x00000020)
/*! INIC.DeviceMode.Status */
#define NTF_DEVMODE                            ((dword) 0x00000040)
/*! INIC.LockState.Status */
#define NTF_LOCKSTATE                          ((dword) 0x00000080)
/*! INIC.NCState.Status */
#define NTF_NCSTATE                            ((dword) 0x00000100)
/*! INIC.VersionInfo.Status */
#define NTF_VERSIONINFO                        ((dword) 0x00000200)
/*! INIC.Priority.Status */
#define NTF_PRIORITY                           ((dword) 0x00000400)
/*! INIC.RetryOptions.Status */
#define NTF_RETRY_OPTS                         ((dword) 0x00000800)
/*! NB.RetryParameters.Status */
#define NTF_RETRY_PARAMS                       ((dword) 0x00001000)
/*! INIC.AbilityToSegment.Status */
#define NTF_ABILITY_TO_SEGMENT                 ((dword) 0x00002000)
/*! NB.PermissionToWake.Status */
#define NTF_PERMISSION_TO_WAKE                 ((dword) 0x00004000)
/*! INIC.PMIState.Status */
#define NTF_PMISTATE                           ((dword) 0x00008000)
/*! INIC.WatchdogMode.Status */
#define NTF_WATCHDOGMODE                       ((dword) 0x00010000)
/*! INIC.RemoteAccess.Status */
#define NTF_REMOTE_ACCESS                      ((dword) 0x00020000)
/*! INIC.Attenuation.Status */
#define NTF_ATTENUATION                        ((dword) 0x00040000)
/*! INIC.MidLevelRetries.Status */
#define NTF_MIDLEVELRETRY                      ((dword) 0x00080000)

/*! wait for no result messages at all */
#define NTF_NONE                               ((dword) 0x00000000)
/*! wait for all result messages before going to "init complete" */

#define NTF_MASK                               ((dword) (0x000FFFFF & ~NTF_REMOTE_ACCESS & ~NTF_PRIORITY & ~NTF_RETRY_OPTS & ~NTF_NODEDELAY & ~NTF_PERMISSION_TO_WAKE) )

/*! notification mask of the wSCM service */
#define NTF_MASK_WSCM   ((dword)(NTF_BANDWIDTH | NTF_MUTEMODE))
/*! notification mask of the vMSV service */
#define NTF_MASK_VMSV   ((dword)(NTF_NISTATE | NTF_DEVMODE | NTF_LOCKSTATE | NTF_NCSTATE))
/*! notification mask of NBMIN related properties */
#define NTF_MASK_NBMIN  ((dword)(NTF_NODE_ADDR | NTF_GROUP_ADDR | NTF_RETRY_PARAMS))

/*! OS81050 Rev. D */
#define OS81050D                                ((byte)  1)
/*! OS81050 Rev. E */
#define OS81050E                                ((byte)  2)
/*! OS81082 Rev. A */
#define OS81082A                                ((byte)  3)
/*! OS81082 Rev. B */
#define OS81082B                                ((byte)  4)
/*! OS81050 unknown revision */
#define OS81050_ANY                             ((byte)  5)
/*! OS81082 unknown revision */
#define OS81082_ANY                             ((byte)  6)
/*! OS81110 unknown revision */
#define OS81110_ANY                             ((byte)  7)
/*! OS81110 Rev. A*/
#define OS81110A                                ((byte)  8)
/*! OS81110 Rev. B*/
#define OS81110B                                ((byte)  9)
/*! OS81110 Rev. C*/
#define OS81110C                                ((byte) 10)
/*! OS81060 Rev.A */
#define OS81060A                                ((byte) 11)
/*! OS81060 unknown revision */
#define OS81060_ANY                             ((byte) 12)
/*! OS81092 Rev. B */
#define OS81092B                                ((byte) 13)
/*! OS81092 unknown revision */
#define OS81092_ANY                             ((byte) 14)
/*! OS81082 Rev. C */
#define OS81082C                                ((byte) 15)
/*! OS81092 Rev. C */
#define OS81092C                                ((byte) 16)
/*! OS81110 Rev. D*/
#define OS81110D                                ((byte) 17)
/*! OS81060 Rev.B */
#define OS81060B                                ((byte) 18)


/*! no special feature codes */
#define NSF_NONE                                ((word) 0x0000)
/*! this bit is used for features that are supported by all current firmware
  * versions, but where optional in the past (may be used in customers code)
  */
#define NSF_LEGACY                              ((word) 0x0001)
/*! extended socket support */
#define NSF_EXT_SOCKETS                         NSF_LEGACY
/*! oPhy */
#define NSF_O_PHY                               ((word) 0x0002)
/*! ePhy */
#define NSF_E_PHY                               ((word) 0x0004)
/*! MOST25 */
#define NSF_MOST_25                             ((word) 0x0008)
/*! MOST50 */
#define NSF_MOST_50                             ((word) 0x0010)
/*! connection label support (if not present = Byte channel list) */
#define NSF_CONN_LABELS                         ((word) 0x0020)
/*! support for INIC.NetOnTimer */
#define NSF_NETONTIMER                          ((word) 0x0040)
/*! support for TxRetry */
#define NSF_TX_RETRY                            ((word) 0x0080)
/*! support for Slave Wakeup */
#define NSF_SLAVE_WAKEUP                        ((word) 0x0100)
/*! supports Ring Break Diagnosis */
#define NSF_RBD                                 ((word) 0x0200)
/*! supports signal attenuation */
#define NSF_ATTENUATION                         ((word) 0x0400)
/*! supports Alternative Packet Address */
#define NSF_ALT_PACKET_ADDR                     ((word) 0x0800)
/*! is a ROM variant */
#define NSF_ROM                                 ((word) 0x1000)
/*! MOST150 */
#define NSF_MOST_150                            ((word) 0x4000)
/*! Disable SCError in FW 1.10.07 */
#define NSF_OS81050_V01_10_07                   ((word) 0x8000)


#define MNS_GO_ATTACHED_MODE                    ((byte) 0x01)
#define MNS_GO_PROTECTED_MODE                   ((byte) 0x00)

#define MNS_DENY                                ((byte) 0x00)
#define MNS_ALLOW                               ((byte) 0x01)

#define MNS_TM_WD_DISABLE                       ((word)0xFFFF)
#define MNS_TM_WD_MIN_TIMEOUT                   ((word)0x0014)
#ifdef MNS_TM_WD_EXTENDED_TIMEOUT 
    #if MNS_TM_WD_EXTENDED_TIMEOUT < 0x01F4
        #define MNS_TM_WD_MAX_TIMEOUT           ((word)0x01F4)
    #elif MNS_TM_WD_EXTENDED_TIMEOUT > 5000
        #define MNS_TM_WD_MAX_TIMEOUT           ((word) 5000) 
    #else 
        #define MNS_TM_WD_MAX_TIMEOUT           MNS_TM_WD_EXTENDED_TIMEOUT
    #endif
#else
    #define MNS_TM_WD_MAX_TIMEOUT               ((word)0x01F4)
#endif

#define MNS_TM_WD_THRESHOLD_DISABLE             ((word)0xFFFF)


#define MNS_ATW_OFF                             ((byte) 0x00)
#define MNS_ATW_ON                              ((byte) 0x01)
#define MNS_ATW_CRITICAL                        ((byte) 0x02)

#define MNS_PHASE_RESET                         ((byte) 0x01)
#define MNS_PHASE_INIT                          ((byte) 0x02)

typedef word TTimerDiff;                        /* used only in L2 and MHP */

typedef void TMnsStdCB(TMnsResult result);

typedef struct TNetServicesConfig
{
    struct TMnsConfig
    {
        void (*most_service_request_fptr)(void);
        void (*on_init_complete_fptr)(void);
        void (*on_stop_netservices_fptr)(void);
        byte (*wakeup_query_fptr)(void);
        void (*on_error_fptr)(TMnsResult code);
        void (*on_buf_freed_fptr)(void);
        word (*get_tick_count_fptr)(void);
        void (*next_min_timeout_fptr)(word timeout);

        /*! This structure holds the watchdogmode configuration that shall be
          * transmitted to the INIC (overwrite = MNS_TRUE) or used during init till
          * the INIC communicates its configuration (overwrite=MNS_FALSE).
          */
        struct TMnsConfigWatchdogMode
        {
            /*! Whether to overwrite the configuration present inside the INIC
              * or not.
              */
            bool overwrite;

            /*! Whether the INIC shall reset the EHC if the watchdog times out
              * or not (only effective if "overwrite" is MNS_TRUE).
              */
            bool reset;

            /*! The timeout the watchdog of the INIC shall use. Depending on
              * "overwrite" this is only used during initalization (MNS_FALSE) or
              * commanded to the INIC (MNS_TRUE). That means that in case of
              * overwrite=MNS_FALSE this is only a hint till the INIC communicates
              * its timeout to the EHC.
              */
            word timeout;

            /*! Determines in devices with power master a delay when the
              * network is shut down, after the INIC is entering the EHCIState
              * Protected (only effective if "overwrite" is MNS_TRUE).
              */
            word auto_shutdown_delay;

            /*! Determines the threshold for MOST Debug Message (in milliseconds)
              * Valid range:    0..500 or 
              *                 0xFFFF (INIC's default value is used)
              * If the value is unequal to 0xFFFF, it has to be less than the value watchdogmode.timeout.
              * This paramter is relevant for MOST150 only. At MOST50 the value is don't care.
              */
            word threshold;

        } watchdogmode;

        /*! The amount of events the Kernel shall handle if it gets called. */
        byte burst;

    } general;

    struct TLldConfig
    {
        void (*start_interfaces_fptr)(void);
        void (*stop_interfaces_fptr)(void);
        void (*reset_fptr)(void);
        void (*on_buf_freed_fptr)(void);
        byte (*i2c_tx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);
        byte (*ctrl_tx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);
        byte (*data_tx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);
        byte ctrl_interface_id;
        byte data_interface_id;

    } lld;

    struct TPmsConfig
    {
        byte rx_burst;
    } pms;

    struct TMsgConfig
    {
        byte (*tx_status_fptr)(byte status, TMsgTx *msg_ptr);
        byte (*tx_filter_fptr)(TMsgTx *msg_ptr);
        byte (*rx_complete_fptr)(TMsgRx *rx_ptr);
        void (*rx_error_fptr)(byte error, TMsgRx *rx_ptr);
        #ifdef AMS_TX_BYPASS_FILTER
        bool (*tx_bypass_fptr)(TMsgTx *msg_ptr);
        #endif

        #ifdef MSG_RX_USER_PAYLOAD
        word (*get_payload_buf_fptr)(TMsgRx *rx_ptr, word required_size);
        void (*free_payload_buf_fptr)(TMsgRx *rx_ptr);
        word (*reallocate_payload_buf_fptr)(TMsgRx *rx_ptr, word required_size);
        #endif
        #ifdef NS_AMS_MSV2
        void (*tx_buffer_flushed_fptr)(word num);
        #endif
        #if (defined NS_AMS_MSV2) || (defined NS_MSV_ET)
        void (*tx_msg_flushed_fptr)(TMsgTx *msg_ptr);
        #endif

        #ifdef MSG_TX_USER_PAYLOAD
        void (*free_tx_payload_fptr)(byte *data_ptr);
        void (*free_tx_payload_ext_fptr)(TMsgTx *msg_ptr);
        #endif
        void (*retry_config_adjusted_fptr) (TMsgRetryConfig* config_ptr);

        byte rx_burst;
        #ifndef PMS_RX_OPT4
        bool rx_direct;
        #endif

    } msg;

    #ifdef ADS_MIN
    struct TDataConfig
    {
        void (*tx_status_fptr)(TDataTx *msg_ptr);
        byte (*rx_complete_fptr)(TDataRx *rx_ptr);
        byte (*rx_filter_fptr)(TDataRx *rx_ptr);

        #if (defined PACKET_COMMUNICATION_ONLY) && (defined _OS81110_NA_MDP)
        void (*mdp_state_fptr)(TDataNAOverMDP *state_ptr);
        #endif

        #ifdef DATA_RX_DBG_FILTER_ENABLED
        byte (*rx_dbg_filter_fptr)(TDataRx *rx_ptr);
        #endif

        #ifdef DATA_TX_DBG_STATUS_ENABLED
        bool (*tx_dbg_status_fptr)(TDataTx *tx_ptr);
        #endif

        #ifdef DATA_TX_USER_PAYLOAD
        void (*free_data_tx_payload_fptr)(byte *data_ptr);
        void (*free_data_tx_payload_ext_fptr)(TDataTx *msg_ptr);
        #endif

        byte rx_burst;
        bool rx_direct;

    } wads;
    #endif

    #ifdef VMSV_MIN
    struct TVmsvConfig
    {
        void (*msval_state_changed_fptr)(byte state);
        void (*msval_error_fptr)(byte error, byte *info_ptr);
        void (*msval_event_fptr)(byte event, byte *info_ptr);
        void (*msval_diag_result_fptr)(byte result, byte *info_ptr);
        void (*pmistate_changed_fptr)(byte state, byte events);
        #ifdef MSV_DIAG_RESULT_MSG
        void (*rbd_store_rbd_result_fptr)(byte rbd_status, byte length, byte *diag_id);
        #endif

        #if (defined MSV_DIAG_RESULT_MSG) || (defined _OS81110_SSO)
        struct TVmsvDiagID
        {
            byte *stream;
            byte length;
        } diag_id;
        #endif

        #ifdef _OS81110_SSO
        void (*sso_store_ssoresult_fptr)(byte result);
        byte (*sso_restore_ssoresult_fptr)(void);
        #endif
    } vmsv;
    #endif

    #ifdef SCM_MIN
    struct TScmConfig
    {
        void (*on_error_fptr)(TMnsResult code, byte handle);
        void (*bandwidth_changed_fptr)(byte preset,  byte current,
                                       byte total,   word free,
                                       byte packet,  byte used);
        void (*sc_demute_fptr)(byte* sc_status, word length);
        #ifdef _OS81110_SPDIF
        void (*spdif_status_fptr)(byte *info, byte len);
        #endif
        struct TScmConfigPacket
        {
            byte mode;
            byte port_id;

            /* MediaLB port parameter */
            byte clock_config;

            #ifdef _OS81110_SPI
            /* SPI port parameter */
            byte clock_mode;
            byte interrupt_threshold;
            #endif

            struct TScmConfigPacketChannel
            {
                word address;
                byte bandwidth;

            } in;

            struct TScmConfigPacketChannel out;

        } packet;

    } scm;

    #endif

    #ifdef MDM_MIN
    struct TMdmConfig
    {
        void (*app_config_fptr)(void);
    } mdm;
    #endif

    #ifdef GAS_MIN
    struct TGasConfig
    {
        void (*dbg_cb_fptr)(byte action_id, byte error_code);
    } gas;
    #endif


} TNetServicesConfig;

typedef void TMostEventCB(word event);

typedef struct TMostTimer TMostTimer;
struct TMostTimer
{
    TMostEventCB *cb_ptr;
    word          event;
    word          value;
    word          periodic;
    TMostTimer   *prev_ptr;
    TMostTimer   *next_ptr;

};

/*! MOST NetServices Version Information */
typedef struct TMostVersionInfo
{
    /*! Product Name consists of High Number, Middle Number and Low Number
     * (e.g., 0x08, 0x11, 0x10 represents OS81110)
     */
    byte prod_name[3];

    /* INIC Hardware Chip Id */
    byte chip_id;

    /* INIC Hardware Release Number */
    byte hw_release;

    /*! The id for the chip.
      *
      * @see constants declared in mns_pb.h (e.g. OS81050D, OS81082A etc.)
      */
    byte hw;

    /*! The firmware version (Major [0], Minor [1], Release [2]). */
    byte fw[3];

    /*! The firmware version (Year [0], Month [1], Day [2]).
      *
      * @see MostGetRevision
      */
    byte fw_date[3];

    /*! The NetServices version (Major [0], Minor [1], Release [2], Step [3]).
      * If Step is 0x00 it is an official release. If it is 0x01-0x99 it is an
      * development snapshot for SMSC internal use only. In case of 0xB1-0xB9
      * it is a beta-release and 0xC1-C9 are release-candidates.
      */
    byte ns[4];

    /*! The version of the config string (Major [0], Minor [1], Release [2]). */
    byte cs[3];

    /*! The version of the INIC API in use */
    byte api[3];

} TMostVersionInfo;




#ifdef MNS_0
    void InitNetServices(void);
#endif

#ifdef MNS_1
    void MostService(void);
#endif

#ifdef MNS_16
    void MostSetTimer(TMostTimer *timer_ptr, word milliseconds, bool periodic);
#endif

#ifdef MNS_15
    /*! Clear the value of a timer (set to zero without fireing the event).
      *
      * @author     SSemmler
      * @date       last update:  4/19/2005
      * @version    2.0.3
      * @see        MostSetTimer
      * @see        MostGetTimer
      * @see        MostRegisterTimer
      * @see        MostUnregisterTimer
      *
      * @param      timer_ptr is a pointer to the timer instance
      */
    #define MostClearTimer(timer_ptr) MostSetTimer(timer_ptr, (word)0, MNS_FALSE)
#endif

#ifdef MNS_17
    word MostGetTimer(TMostTimer *timer_ptr);
#endif

#ifdef NS_MNS_MNS2

    #define MNS2_E_TIMER_EXTERN 0x000A
    #define MNS2_E_PEN_EXTERN   0x8000
    #define MNS2_E_REQ_EXTERN   0x4000

    void InitNetServicesLayer2(void);
    word MostServiceLayer2(word opt, word events);
    byte MsgRxOutMsg(TMsgRx *msg_ptr);
#endif

#ifdef NS_MNS_MHP
    #define MNSH_E_TIMER_TX_EXTERN  0x0002
    #define MNSH_E_TIMER_RX_EXTERN  0x0008
    #define MNSH_E_PEN_EXTERN       0x8000
    #define MNSH_E_REQ_EXTERN       0x4000
    #define MNSH_E_TX_EVENT_EXTERN  0x0001

    void MhpInit(void);
    word MostServiceHigh(word events);
    word MhpGetMinTimeout(word min);
#endif

#ifdef NS_MNS_PMHS
    #define MNSP_E_TIMER_TX_EXTERN  0x0002
    #define MNSP_E_TIMER_RX_EXTERN  0x0020
    #define MNSP_E_PEN_EXTERN       0x8000
    #define MNSP_E_REQ_EXTERN       0x4000

    void PacketInit(void);
    word MostServicePacket(word events);
    void PacketTimerIntDiff(TTimerDiff diff);
    word PacketGetMinTimeout(word min);
#endif

#ifdef MNS_45
    void MostRegisterTimer(TMostTimer *timer_ptr, TMostEventCB *cb_ptr, word event);
#endif

#ifdef MNS_46
    void MostUnregisterTimer(TMostTimer *timer_ptr);
#endif

#ifdef MNS_54
    bool MostIsSupported(word feature);
#endif

#ifdef MNS_56
    _INLINE void MnsMemCpy(byte* tgt_ptr, const byte* src_ptr, word size);
#endif

#ifdef MNS_57
    void MostGetVersionInfo(TMostVersionInfo *info_ptr);
#endif

#ifdef MNS_58
    void MostGetRevision(byte *tgt_ptr);
#endif

#ifdef MNS_62
    void MostCheckTimers(void);
#endif

#ifdef MNS_63
    TNetServicesConfig * GetNetServicesConfig(void);
#endif

#ifdef MNS_64
    void StopNetServices(void);
#endif

#ifdef MNS_65
    byte MostAllowRemoteAccess(byte access);
#endif

#ifdef MNS_71
    void MhpTxDelayTimeout(void);
#endif

#ifdef MNS_102
void MnsSignalNetOn(void);
#endif

#ifdef MNS_112
    void MnsGetNetServicesVersion(byte *mns_ver);
#endif

#endif /* _MNS_PB_H */

