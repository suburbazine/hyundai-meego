/*
 ******************************************************************
 * COPYRIGHT © 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE : DHAVN_QCANController								  *
 * PROGRAMMER : Sambit Patnaik									  *
 * DATE : 8 August 2011                                    		  *
 * VERSION : 1.7												  *
 *--------------------------------------------------------------- *
 * MODULE SUMMARY : This file defines all the structures          *
 *                  corresponding to CAN_RAW and CAN_BCM          *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS * 
 * VERSION   DATE            PROGRAMMER               COMMENT
 ******************************************************************
 * 1.7                                               cr 5128
 * 1.6                                               cr 4906
 * 1.5                                               cr 2226
 * 1.4						     cr 1781
 * 1.3						     cr 1291
 * 1.2                                               cr 673
 * 1.1                                               cr 302
 * 1.0       8 August 2011  Sambit Patnaik           Draft version
 ******************************************************************/


#ifndef DHAVN_QCANCONTROLLER_APPLNTXRXSTRUCT_H
#define DHAVN_QCANCONTROLLER_APPLNTXRXSTRUCT_H

#include <QtCore/QtCore>
#include <QMetaType>
#include <QObject>
#include <QFileInfo>
#include <QTextStream>
#include <QTime>
#include <QDebug>
#include <DHAVN_AppFramework_Logger_Def.h>

#define HS_LOG( logText )                                           \
{                                                                   \
    QString sLogString;                                             \
    QTextStream tsLogStream( &sLogString );                         \
    tsLogStream << __FUNCTION__ << " :: " << logText;                 \
    Logger::Instance()->Log( sLogString, Logger::PRIORITY_HI );     \
}

#define CR_LOG( logText )                                           \
{                                                                   \
    QString sLogString;                                             \
    QTextStream tsLogStream( &sLogString );                         \
    tsLogStream << __FUNCTION__ << " :: " << logText;                 \
    Logger::Instance()->Log( sLogString, Logger::PRIORITY_CRITICAL );     \
}

#define LW_LOG( logText )                                           \
{                                                                   \
    QString sLogString;                                             \
    QTextStream tsLogStream( &sLogString );                         \
    tsLogStream << __FUNCTION__ << " :: " << logText;                 \
    Logger::Instance()->Log( sLogString, Logger::PRIORITY_LOW );     \
}

// Constant declared for the CAN data payload
const int gc_can_data_size = 8;

/** This structure hold the data member analogous to can_frame */
struct SQCanFrame
{
    uint uqcanid;    /**< The CAN ID of the frame */
    uchar uqcandlc;  /**< The data length field of the CAN frame */
    uchar uqcandata[8];  /**< The CAN frame payload */
};

/** This structure hold the list of CAN frames and the count */
struct SQCanFrameList
{
    uint uqcount;    /**< The count of the SQCanFrame */
    QList<SQCanFrame> qcanframelist;    /**< The list of SQCanFrames */
};

/** This structure hold the data member analogous to timeval structure */
struct SQTime
{
    qlonglong lqtime_sec;    /**< seconds */
    qlonglong lqtime_usec;   /**< microseconds */
};

/** This structure hold the data member analogous to bcm_msg_head */
struct SQBCMHead
{
    uint uqopcode;   /**< opcode */
    uint uqflags;    /**< special flags */
    uint uqcount;    /**< number of frames to send before changing interval */
    struct SQTime lqval1, lqval2; /**< @lqval1: interval for the first @uqcount frames */
                                /**< @lqval2 :interval for the following frames */
    uint uqcanid;    /**<  CAN ID of frames to be sent or received */
    uint uqnframes;  /**< number of frames appended to the message head */
    struct SQCanFrame qframes[0];   /**< array of CAN frames */
};

/** This structure hold the list of can ID's and the count */
struct SQCanIdList
{
    uint uqcount;	/**< The count of the can ID's */
    QList<uint> qcanidlist;   /**< The list of can ID's */
};

/** This is required for the registration in the QT meta system */
Q_DECLARE_METATYPE(SQCanFrame)
Q_DECLARE_METATYPE(SQTime)
Q_DECLARE_METATYPE(SQBCMHead)
Q_DECLARE_METATYPE(SQCanFrameList)
Q_DECLARE_METATYPE(SQCanIdList)

#endif // DHAVN_QCANCONTROLLER_APPLNTXRXSTRUCT_H
