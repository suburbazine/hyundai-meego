/*
 ******************************************************************
 * COPYRIGHT � 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE : DHAVN_QCANController				  *
 * PROGRAMMER : Shiva Kumar					  *
 * DATE : 14 March 2012                                    	  *
 * VERSION : 1.9						  *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : This file defines APIs which interact with    *
 *                  the Socket CAN_RAW and  CAN_BCM.              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS * 
 * VERSION   DATE            PROGRAMMER               COMMENT
 ******************************************************************
 * 1.9     14 March 2012    Shiva Kumar              cr 6313
 * 1.8                                               cr 5636
 * 1.7                                               cr 5128
 * 1.6                      Divya Ravindran          cr 4906
 * 1.5                                               cr 2226
 * 1.4						     cr 1781
 * 1.3						     cr 1291
 * 1.2                                               cr 673
 * 1.1                                               cr 302
 * 1.0       8 August 2011  Sambit Patnaik           Draft version
 ******************************************************************/

#include "DHAVN_QCANController_CANTxRx.h"

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

// PF_CAN assigned this number as a standard
#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

/*
 *******************************************************************
 * Function:CQCANController_CANTxRx()
 *
 * Description :Default constructor
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */
CQCANController_CANTxRx::CQCANController_CANTxRx():m_iCanSocket(0), m_iBcmSocket(0), m_CanIdList(NULL),
                         m_pCanSocketNotifier(NULL), m_pCanBCMSocketNotifier(NULL)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx constructor entry");
}

/*
 *******************************************************************
 * Function:Init()
 *
 * Description :Initialize the member variables
 *
 * Parameters :none
 *
 * Returns :bool
 *
 *******************************************************************
 */
bool CQCANController_CANTxRx::Init()
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Init entry");
    bool initStatus = false;
    //Create RAW socket and set the socket option
    bool initRawStatus = Initialize_raw_socket();
    if(initRawStatus)
    {
        //Create BCM socket and set the socket option
		//This Initialization is done if RAW socket is properly initialized
        //so that, if Init() is success, it implies all sockts are created
        //properly
        bool initBcmStatus = Initialize_bcm_socket();
        if(initBcmStatus)
        {
           initStatus = true;
        }
    }

    //Allocate the structure that hold the CAN ID list
    m_CanIdList = new SQCanIdList();
    if(m_CanIdList)
    {
        m_CanIdList->uqcount = 0;
    }
    else
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Init, Memory allocation failed!!!");
        initStatus = false;
    }
    LW_LOG("[QCAN] CQCANController_CANTxRx::Init exit");
    return initStatus;
}

/*
 *******************************************************************
 * Function:~CQCANController_CANTxRx()
 *
 * Description :Default destructor
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */
CQCANController_CANTxRx::~CQCANController_CANTxRx()
{
    //cleanup code
    if(m_pCanSocketNotifier)
    {
        delete m_pCanSocketNotifier;
        m_pCanSocketNotifier = NULL;
    }
    if(m_pCanBCMSocketNotifier)
    {
        delete m_pCanBCMSocketNotifier;
        m_pCanBCMSocketNotifier = NULL;
    }
    if(m_CanIdList)
    {
        delete m_CanIdList;
    }
}

/*
 *******************************************************************
 * Function:Initialize_raw_socket()
 *
 * Description :Initializes the CAN RAW socket
 *
 * Parameters :none
 *
 * Returns :bool
 *
 *******************************************************************
 */
bool CQCANController_CANTxRx::Initialize_raw_socket()
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket entry");
    //Create RAW socket
    m_iCanSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_iCanSocket < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket, Error in RAW socket creating!!!");
        return false;
    }

    int loopback = 1; /* 0 = disabled, 1 = enabled (default) */

    //Set the socket option to prevent local loopback notification
    int ret = setsockopt(m_iCanSocket, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    if (ret < 0 ) {
        CR_LOG("[QCAN] Failed to set the socket option to prevent local loopback notification!!!");
        return false;
    }

    struct sockaddr_can addr;
    struct ifreq ifr;

    addr.can_family = AF_CAN;
    strncpy(ifr.ifr_name, "can0", IFNAMSIZ);
    int ioctl_status = ioctl(m_iCanSocket, SIOCGIFINDEX, &ifr);
    if(ioctl_status < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket, Error in RAW socket ioctl() call!!!");
        return false;
    }
    addr.can_ifindex = ifr.ifr_ifindex;
    int status = bind(m_iCanSocket, (const struct sockaddr *)&addr, sizeof(addr));
    if(status < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket, Error in RAW socket binding!!!");
        return false;
    }
    //Instantiate the socket notifier for the RAW socket and connect it to
    //the socket identifier
    m_pCanSocketNotifier = new(std::nothrow) QSocketNotifier(m_iCanSocket,QSocketNotifier::Read,this);
    if(!m_pCanSocketNotifier)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket, Memory allocation failed!!!");
        return false;
    }
    Connect_read_socket_notifier();
    LW_LOG("[QCAN] CQCANController_CANTxRx::Initialize_raw_socket exit");
    return true;
}

/*
 *******************************************************************
 * Function:Initialize_bcm_socket()
 *
 * Description :Initializes the CAN BCM socket
 *
 * Parameters :none
 *
 * Returns :bool
 *
 *******************************************************************
 */
bool CQCANController_CANTxRx::Initialize_bcm_socket()
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket entry");
    m_iBcmSocket = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
    if (m_iBcmSocket < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket, Error in BCM socket creating!!!");
        return false;
    }

    int loopback = 1; /* 0 = disabled, 1 = enabled (default) */
    //Set the socket option to prevent local loopback notification
    int ret = setsockopt(m_iBcmSocket, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback, sizeof(loopback));
    if (ret < 0 ) LW_LOG("[QCAN] QCANController_CANTxRx::Initialize_bcm_socket : setsockopt ERROR!!!!");

    struct sockaddr_can addr;
    struct ifreq ifr;
    addr.can_family = AF_CAN;
    strncpy(ifr.ifr_name, "can0", IFNAMSIZ);
    int ioctl_status = ioctl(m_iBcmSocket, SIOCGIFINDEX, &ifr);
    if(ioctl_status < 0)
    {
       CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket, Error in BCM socket ioctl() call!!!");
        return false;
    }
    addr.can_ifindex = ifr.ifr_ifindex;
    //Connect the BCM socket to the address
    int connectStatus = ::connect(m_iBcmSocket, (const struct sockaddr *)&addr, sizeof(addr));
    if(connectStatus < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket, Error in BCM socket connect call!!!");
        return false;
    }
    m_pCanBCMSocketNotifier = new(std::nothrow) QSocketNotifier(m_iBcmSocket,QSocketNotifier::Read,this);
    if(!m_pCanBCMSocketNotifier)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket, Memory allocation failed!!!");
        return false;
    }
    Connect_read_bcm_socket_notifier();
    LW_LOG("[QCAN] CQCANController_CANTxRx::Initialize_bcm_socket exit");
    return true;
}

/*
 *******************************************************************
 * Function:Can_send_data(const SQCanFrame &c_data)
 *
 * Description :This API will write the CAN frame to the CAN RAW Socket
 *
 * Parameters :
 *    c_data     r   This will hold the CAN Frame that should be written
 *                 to the CAN RAW Socket
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_send_data(const SQCanFrame &c_data)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_send_data entry");
    //Initialise the can_frame structure
    struct can_frame frame;
    ssize_t retval = 0;
    frame.can_id = c_data.uqcanid;
    for(int index = 0; index < gc_can_data_size; index++)
    {
        frame.data[index] = c_data.uqcandata[index];
    }
    frame.can_dlc  = c_data.uqcandlc;
    //Write the CAN frame to the RAW socket
    if((retval = write(m_iCanSocket, &frame, sizeof(frame))) < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Can_send_data, Error in socket write!!!");
    }
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_send_data exit");
}

/*
 *******************************************************************
 * Function:Can_send_bcm_data(const SQBCMHead &c_bcm_data,
 *                             const SQCanFrameList &c_data)
 *
 * Description :This API will write the BCM Message to the CAN BCM Socket
 *
 * Parameters :
 *    c_bcm_data     r   This will hold the BCM Message that should be written
 *                    to the CAN BCM Socket
 *    c_data        r   This will have the list of CAN frames that has to be
 *                    written to the CAN BCM socket
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_send_bcm_data(const SQBCMHead &c_bcm_data, const SQCanFrameList &c_data)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_send_bcm_data entry");
    ssize_t retval = 0;
    struct SQCanBCMStruct* pbcmCanMsg = (struct SQCanBCMStruct*)malloc(sizeof(bcm_msg_head) +
                                        (c_data.uqcount)*(sizeof(can_frame)));


    //Initialization of pbcmCanMsg
    pbcmCanMsg->qbcmhead.opcode = 0;
    pbcmCanMsg->qbcmhead.flags = 0;
    pbcmCanMsg->qbcmhead.count = 0;
    pbcmCanMsg->qbcmhead.ival1.tv_sec = 0;
    pbcmCanMsg->qbcmhead.ival1.tv_usec = 0;
    pbcmCanMsg->qbcmhead.ival2.tv_sec = 0;
    pbcmCanMsg->qbcmhead.ival2.tv_usec = 0;
    pbcmCanMsg->qbcmhead.can_id = 0;
    pbcmCanMsg->qbcmhead.nframes = 0;
    pbcmCanMsg->qcanframes[0].can_dlc = 0;
    pbcmCanMsg->qcanframes[0].can_id = 0;
    pbcmCanMsg->qcanframes[0].data[0] = 0;
    pbcmCanMsg->qcanframes[0].data[1] = 0;
    pbcmCanMsg->qcanframes[0].data[2] = 0;
    pbcmCanMsg->qcanframes[0].data[3] = 0;
    pbcmCanMsg->qcanframes[0].data[4] = 0;
    pbcmCanMsg->qcanframes[0].data[5] = 0;
    pbcmCanMsg->qcanframes[0].data[6] = 0;
    pbcmCanMsg->qcanframes[0].data[7] = 0;


    struct SQCanBCMStruct& bcmCanMsg = *pbcmCanMsg;
    struct can_frame *frame = bcmCanMsg.qcanframes;
    //Initialize the bcm_msg_head structure from SQBCMHead structure
    bcmCanMsg.qbcmhead.opcode = c_bcm_data.uqopcode;
    bcmCanMsg.qbcmhead.flags = c_bcm_data.uqflags;
    bcmCanMsg.qbcmhead.count = c_bcm_data.uqcount;
    bcmCanMsg.qbcmhead.ival1.tv_sec = c_bcm_data.lqval1.lqtime_sec;
    bcmCanMsg.qbcmhead.ival1.tv_usec = c_bcm_data.lqval1.lqtime_usec;
    bcmCanMsg.qbcmhead.ival2.tv_sec = c_bcm_data.lqval2.lqtime_sec;
    bcmCanMsg.qbcmhead.ival2.tv_usec = c_bcm_data.lqval2.lqtime_usec;
    bcmCanMsg.qbcmhead.can_id = c_bcm_data.uqcanid;
    bcmCanMsg.qbcmhead.nframes = c_bcm_data.uqnframes;

    //Initialize the can_frame structure array
    for(uint index = 0; index < c_data.uqcount; index++)
    {
        frame->can_id = c_data.qcanframelist[index].uqcanid;
        //Copying the can frame payload data
        for(int dataCnt = 0; dataCnt < gc_can_data_size; dataCnt++)
        {
            frame->data[dataCnt] = c_data.qcanframelist[index].uqcandata[dataCnt];
        }

        frame->can_dlc = c_data.qcanframelist[index].uqcandlc;
        frame++;
    }
    //Write the BCM message structure to the socket
    if((retval = write(m_iBcmSocket, &bcmCanMsg, sizeof(bcm_msg_head)+(c_data.uqcount)*(sizeof(can_frame)))) < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Can_send_bcm_data, Error in socket write!!!");
    }

    //clean the local pointer
    free(pbcmCanMsg);
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_send_bcm_data exit");
}

/*
 *******************************************************************
 * Function:Can_receive_bcm_data(const SQBCMHead &c_bcm_data)
 *
 * Description :This API will be called when the application triggers to
 *              receive a BCM message. This API will set the BCM socket
 *              option with the CAN ID for which the application has
 *              requested a BCM message
 *
 * Parameters :
 *    c_bcm_data     r   This will hold the BCM Message that is used to set the
 *                    BCM socket option
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_receive_bcm_data(const SQBCMHead &c_bcm_data)
 {
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_receive_bcm_data entry");
    struct can_filter canfilter;
    //Set the CAN ID from the SQBCMHead structure
    canfilter.can_id   = c_bcm_data.uqcanid; /* SFF frame */
    canfilter.can_mask = CAN_SFF_MASK;

    int ret = setsockopt(m_iBcmSocket, SOL_CAN_RAW, CAN_RAW_FILTER, &canfilter, sizeof(canfilter));
    if(ret < 0) CR_LOG("[QCAN] CQCANController_CANTxRx::Can_receive_bcm_data error!!!");
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_receive_bcm_data exit");
 }

/*
 *******************************************************************
 * Function:Can_receive_data(const SQCanFrameList &c_data)
 *
 * Description :This API will be called when CAN Data is available in
 *              the CAN RAW/BCM socket. This will be called when socket
 *              notifier gets a call indicating some data is available
 *
 * Parameters :
 *    c_data     r   This will hold the list of CAN Frames
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_receive_data(const SQCanFrameList &c_data)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_receive_data entry");
    //This SIGNAL will invoke the Received_data_from_can_raw API in
    //CQCANController_ApplnTxRx
    emit Can_data_received(c_data);

    //Enable the notifier to start listening for the data
    m_pCanSocketNotifier->setEnabled(true);
    m_pCanBCMSocketNotifier->setEnabled(true);
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_receive_data exit");
}

/*
 *******************************************************************
 * Function:Get_can_id()
 *
 * Description :This SLOT will be called when there is some data available
 *              for the read from the CAN RAW socket. This is connected
 *              to the SIGNAL activated() which socket notifier emits
 *              when data is available. This API basically will trigger a
 *              call to application requesting for the CAN ID's
 *
 * Parameters :none
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Get_can_id()
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Get_can_id entry");
    //Disable the socket notifier till the received data
    //is sent to the application
    m_pCanSocketNotifier->setEnabled(false);

    if(!m_CanIdList->uqcount)
    {
        LW_LOG("[QCAN] CQCANController_CANTxRx::Filter_can_id");
        //SIGNAL to get the list of CAN frame ID's
        emit Filter_can_id();
    }
    else
    {
        //Read the data from the socket for the ID's already stored
        //in m_CanIdList struct
        Can_read_data_raw(*m_CanIdList);
    }
    LW_LOG("[QCAN] CQCANController_CANTxRx::Get_can_id exit");
}

/*
 *******************************************************************
 * Function:Get_can_bcm_msg()
 *
 * Description :This SLOT will be called when there is some data available
 *              for the read from the CAN BCM socket. This is connected
 *              to the SIGNAL activated() which socket notifier emits
 *              when data is available. This API basically will trigger a
 *              call to application requesting for the BCM message
 *
 * Parameters :none
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Get_can_bcm_msg()
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Get_can_bcm_msg entry");
    //Disable the socket notifier till the received data
    //is sent to the application
    m_pCanBCMSocketNotifier->setEnabled(false);

    LW_LOG("[QCAN] CQCANController_CANTxRx::Filter_can_bcm_msg");
    //SIGNAL to get the BCM message
    emit Filter_can_bcm_msg();
    LW_LOG("[QCAN] CQCANController_CANTxRx::Get_can_bcm_msg exit");
}

/*
 *******************************************************************
 * Function:Connect_read_socket_notifier()
 *
 * Description :Connects the CAN RAW socket with QSocketNotifier
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Connect_read_socket_notifier()
{
    //Connect the CAN RAW socket notifier
    connect(m_pCanSocketNotifier,SIGNAL(activated(int)),this,SLOT(Get_can_id()));
}

/*
 *******************************************************************
 * Function:Connect_read_bcm_socket_notifier()
 *
 * Description :Connects the CAN BCM socket with QSocketNotifier
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Connect_read_bcm_socket_notifier()
{
    //Connect the CAN BCM socket notifier
    connect(m_pCanBCMSocketNotifier,SIGNAL(activated(int)),this,SLOT(Get_can_bcm_msg()));
}

/*
 *******************************************************************
 * Function:Can_read_bcm_data_raw(const SQBCMHead &c_bcm_data)
 *
 * Description :This SLOT will be called when there is some data
 *              available for the read from the CAN BCM socket.
 *              This is connected to the SIGNAL activated() which socket
 *              notifier emits, when data is available.This API will
 *              read the CAN BCM socket for the list of CAN ID's and
 *              send the list of can_frames to  Can_receive_data API
 *
 * Parameters :
 *      c_bcm_data     r   This will hold the BCM message passed from the
 *                      client
 *
 * Returns :none
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_read_bcm_data_raw(const SQBCMHead &c_bcm_data)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_read_bcm_data_raw entry");
    SQCanFrameList qcanFrameList;

    struct SQCanBCMStruct *pbcmCanMsg = (struct SQCanBCMStruct *)malloc(sizeof(bcm_msg_head)
                                        + (c_bcm_data.uqnframes)*(sizeof(can_frame)));


    //Initialization of pbcmCanMsg
    pbcmCanMsg->qbcmhead.opcode = 0;
    pbcmCanMsg->qbcmhead.flags = 0;
    pbcmCanMsg->qbcmhead.count = 0;
    pbcmCanMsg->qbcmhead.ival1.tv_sec = 0;
    pbcmCanMsg->qbcmhead.ival1.tv_usec = 0;
    pbcmCanMsg->qbcmhead.ival2.tv_sec = 0;
    pbcmCanMsg->qbcmhead.ival2.tv_usec = 0;
    pbcmCanMsg->qbcmhead.can_id = 0;
    pbcmCanMsg->qbcmhead.nframes = 0;
    pbcmCanMsg->qcanframes[0].can_dlc = 0;
    pbcmCanMsg->qcanframes[0].can_id = 0;
    pbcmCanMsg->qcanframes[0].data[0] = 0;
    pbcmCanMsg->qcanframes[0].data[1] = 0;
    pbcmCanMsg->qcanframes[0].data[2] = 0;
    pbcmCanMsg->qcanframes[0].data[3] = 0;
    pbcmCanMsg->qcanframes[0].data[4] = 0;
    pbcmCanMsg->qcanframes[0].data[5] = 0;
    pbcmCanMsg->qcanframes[0].data[6] = 0;
    pbcmCanMsg->qcanframes[0].data[7] = 0;


    struct SQCanBCMStruct &bcmCanMsg = *pbcmCanMsg;

    //Initialize the SQCanBCMStruct
    bcmCanMsg.qbcmhead.opcode = c_bcm_data.uqopcode;
    bcmCanMsg.qbcmhead.flags = c_bcm_data.uqflags;
    bcmCanMsg.qbcmhead.count = c_bcm_data.uqcount;
    bcmCanMsg.qbcmhead.ival1.tv_sec = c_bcm_data.lqval1.lqtime_sec;
    bcmCanMsg.qbcmhead.ival1.tv_usec = c_bcm_data.lqval1.lqtime_usec;
    bcmCanMsg.qbcmhead.ival2.tv_sec = c_bcm_data.lqval2.lqtime_sec;
    bcmCanMsg.qbcmhead.ival2.tv_usec = c_bcm_data.lqval2.lqtime_usec;
    bcmCanMsg.qbcmhead.can_id = c_bcm_data.uqcanid;
    bcmCanMsg.qbcmhead.nframes = c_bcm_data.uqnframes;

    //Assign the empty frame array to the bcmCanMsg, it will be filled
    //when read() returns
    struct can_frame *frame = bcmCanMsg.qcanframes;

    //Initializing the list of can frame count
    qcanFrameList.uqcount = 0;

    //Read BCM data from the BCM socket
    int retStatus = read(m_iBcmSocket, &bcmCanMsg, sizeof(bcm_msg_head) + (c_bcm_data.uqnframes)*(sizeof(can_frame)));
    if(retStatus < 0)
    {
        CR_LOG("[QCAN] CQCANController_CANTxRx::Can_read_bcm_data_raw, Error in receiving data from BCM socket!!!");
    }
    else
    {
         //Iterate for the number of frames that is set in the BCM head message structure
         for(uint frameCount = 0; frameCount < (c_bcm_data.uqnframes); frameCount++)
        {
            SQCanFrame qcanFrame;
            qcanFrame.uqcanid = frame->can_id & CAN_SFF_MASK;   /* standard frame format (SFF) */

            //Copying the can frame payload data
            for(int index = 0; index < gc_can_data_size; index++)
            {
                qcanFrame.uqcandata[index] = frame->data[index];
            }

             qcanFrame.uqcandlc = frame->can_dlc;

             //Append only the valid can_frame structure to the list and update the count
             if(qcanFrame.uqcandlc > 0 && c_bcm_data.uqcanid == bcmCanMsg.qbcmhead.can_id)
             {
                qcanFrameList.qcanframelist.append(qcanFrame);
                qcanFrameList.uqcount++;
             }
             frame++;
        }
        Can_receive_data(qcanFrameList);
    }
    //clean the local pointer
    free(pbcmCanMsg);
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_read_bcm_data_raw exit");
}

/*
 *******************************************************************
 * Function:Can_read_data_raw(const QList<uint> &c_can_id_list)
 *
 * Description :This function will be called from CQCANController_ApplnTxRx
 *              with the list of CAN ID's using filter. This API will
 *              read the CAN RAW socket for the list of CAN ID's and
 *              send the list of can_frames to  Can_receive_data API
 *
 * Parameters :
 *    c_can_id_list     r   This will hold the list of CAN ID's
 *
 * Returns :void
 *
 *******************************************************************
 */
void CQCANController_CANTxRx::Can_read_data_raw(const SQCanIdList &c_can_id_list)
{
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_read_data_raw entry");
    //store the list of CAN ID that application has sent
    m_CanIdList->uqcount = c_can_id_list.uqcount;
    m_CanIdList->qcanidlist = c_can_id_list.qcanidlist;

    SQCanFrameList qcanFrameList;
    qcanFrameList.uqcount = 0;
    qcanFrameList.qcanframelist.clear();

    int canIdCount = c_can_id_list.uqcount;
    //Set the can filter with the list of CAN ID's received from the
    //application
    struct can_filter canfilter[canIdCount];
    memset(&canfilter, 0x0, sizeof(can_filter));
    for(int index = 0; index < canIdCount; index++ )
    {
        canfilter[index].can_id   = c_can_id_list.qcanidlist[index]; /* SFF frame */
        canfilter[index].can_mask = CAN_SFF_MASK;
    }
    //Set the socket option with the list if CAN ID's
    int ret = setsockopt(m_iCanSocket, SOL_CAN_RAW, CAN_RAW_FILTER, &canfilter, sizeof(canfilter));
    if( ret < 0 ) return;
    struct can_frame canRawFrame;
    SQCanFrame qcanFrame;

    int readStatus = read(m_iCanSocket, &canRawFrame, sizeof(canRawFrame));
    if(readStatus > 0 && m_CanIdList->qcanidlist.contains(canRawFrame.can_id & CAN_SFF_MASK))
    {
        qcanFrame.uqcanid = canRawFrame.can_id & CAN_SFF_MASK;   /* standard frame format (SFF) */
        //Copy the can frame payload data
        for(int index = 0; index < gc_can_data_size; index++)
        {
            qcanFrame.uqcandata[index] = canRawFrame.data[index];
        }

         qcanFrame.uqcandlc = canRawFrame.can_dlc;
         //Append the SQCanFrame to the list and update the count
         qcanFrameList.qcanframelist.append(qcanFrame);
         qcanFrameList.uqcount++;

         //send data to application
         Can_receive_data(qcanFrameList);

    }
    else
    {
        //Enable the socket notifier to again listen to data
        m_pCanSocketNotifier->setEnabled(true);
    }
    LW_LOG("[QCAN] CQCANController_CANTxRx::Can_read_data_raw exit");
}
