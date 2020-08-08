/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  15 Sept 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  Utility.
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 15 Sep 2011  Neelima Khedad           Draft version            *
 * 08 Dec 2011  Neelima Khedad           Added new structure to Telephone SCollectPBook
 * 08 Dec 2011  Shiva Kumar              Added new structure SMediadata to jukebox
 * 05 May 2012  Shiva Kumar              Added new structure SDisplayOSD to HMI
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 07 Dec 2012  Shiva Kumar              Modified for FCAT 1.22.1
 * 08 Jan 2013  Hanhong Keum             Modified for FCAT 1.22.5
 * 19 Mar 2013  Hanhong Keum             Modified for FCAT 1.22.7 and 1.22.8
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_UTILITY_H
#define DHAVN_MOSTMANAGER_UTILITY_H

#include <QDebug>
#include <QtDBus>

/** Defines for the logical address of external devices */
#define HU_DEVICE_ADDRESS      0x500
#define IBOX_DEVICE_ADDRESS    0x501
#define AMP_DEVICE_ADDRESS     0x502

/** MOST FCat Version **/
#define	MOST_VERSION_YY				15
#define	MOST_VERSION_MM			03
#define	MOST_VERSION_DD			04

/** Defines for common operation type */
#define METHOD_START            0x00
#define METHOD_ABORT            0x01
#define METHOD_STARTRESULT      0x02
#define METHOD_STARTRESULTACK   0x06
#define METHOD_ABORTACK         0x07
#define METHOD_STARTACK         0x08
#define METHOD_ERRORACK         0x09
#define METHOD_PROCESSINGACK    0x0A
#define METHOD_PROCESSING       0x0B
#define METHOD_RESULT           0x0C
#define METHOD_RESULTACK        0x0D
#define METHOD_INTERFACE        0x0E
#define METHOD_ERROR            0x0F

#define PROPERTY_SET            0x00
#define PROPERTY_GET            0x01
#define PROPERTY_SETGET         0x02
#define PROPERTY_STATUS         0x0C
#define PROPERTY_ERROR          0x0F

/** Defines for different states after calling StartresultACk for HMI*/
#define HMI_PROCESSING_ACK  1
#define HMI_RESULT_ACK      2
#define HMI_STARTRESULT_ACK 3
#define HMI_ERROR_ACK       4
#define HMI_START_ACK       5

/** Defines for different states after calling StartresultACk for IBOX */
#define IBOX_PROCESSING_ACK  1
#define IBOX_RESULT_ACK      2
#define IBOX_ERROR_ACK       3

/** SWDL macros to identify the status processingack,resultack or startresultack*/
#define STATUS_STARTRESULTACK   1
#define STATUS_PROCESSINGACK    2
#define STATUS_RESULTACK        3
#define STATUS_ERROR            4
#define STATUS_ABORTACK         5
#define STATUS_ERRORACK         6
#define STATUS_UNDEFINED        7
#define STATUS_STARTACK         8

/** Defines for different states after calling StartresultACk fro Speech */
#define SPEECH_PROCESSING_ACK  1
#define SPEECH_RESULT_ACK      2
#define SPEECH_START_ACK       3
#define SPEECH_ABORT_ACK       4

/** Defines for Result state for StartResultAck for Telephone */
#define TELEPHONE_PROCESSING_ACK    0x01
#define TELEPHONE_RESULT_ACK        0x02
#define TELEPHONE_ERROR_ACK         0x03

#define MAX_WAV_FRAME       10
#define MAX_WAV_BUFFER_SIZE 32 * 1024 //32Kb max

/*
#define EVENT_CHANNEL_MOST_HIGH                 17
#define EVENT_ID_MOST_HIGH_RECEIVE_REFUSED      4
#define EVENT_ID_MOST_HIGH_RECEIVE_FAILED	5
#define EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE	6
#define EVENT_ID_MOST_HIGH_TRANSMIT_FAILED	7
#define EVENT_ID_MOST_HIGH_TX_CONNECTION_END    8
*/

/** Structures definition for storing array of CallState data */
struct SCallStateData
{
    uint callState;
    uint callType;
};

struct SCallStateDataList
{
    uint listCount;
    QList<SCallStateData> callStateDatalist;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SCallStateData)
Q_DECLARE_METATYPE(SCallStateDataList)

/** Demarshalling QDBusArgument to SCallStateData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCallStateData &callStateData)
{
    argument.beginStructure();
    argument >> callStateData.callType >> callStateData.callState;
    argument.endStructure();
    return argument;
}

/** Marshalling SCallStateData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SCallStateData &callStateData)
{
    argument.beginStructure();
    argument << callStateData.callState << callStateData.callType;
    argument.endStructure();
    return argument;
}

/** Marshalling SCallStateDataList to QDBusArgument */
inline const QDBusArgument &operator<<(QDBusArgument &argument, const SCallStateDataList &callStateDatalist)
{
    argument.beginStructure();
    argument << callStateDatalist.listCount;

    for(uint count =0; count < callStateDatalist.listCount; count++ )
    {
        argument << callStateDatalist.callStateDatalist[count].callState
                 << callStateDatalist.callStateDatalist[count].callType;
    }

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SCallStateDataList */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCallStateDataList &callStateDatalist)
{
    argument.beginStructure();
    callStateDatalist.callStateDatalist.clear();
    argument >> callStateDatalist.listCount;
    SCallStateData dummyCallStatedata[callStateDatalist.listCount];

    for( uint cnt = 0; cnt < callStateDatalist.listCount; cnt++ )
    {
        callStateDatalist.callStateDatalist.append(dummyCallStatedata[cnt]);
    }

    for( uint count =0; count < callStateDatalist.listCount; count++ )
    {
       argument >> callStateDatalist.callStateDatalist[count].callState
                >> callStateDatalist.callStateDatalist[count].callType;
    }

    argument.endStructure();
    return argument;
}

/** STRUCTURES FOR SWDL */

/** structure definition for storing reprogramming mode property parameters*/
struct SReporgrammingMode{

    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    uint SWDLmode;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SReporgrammingMode)

/** Operator overloaded to copy the data from structure SReporgrammingMode over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SReporgrammingMode &sreprogrammingmode)
{
    argument.beginStructure();
    argument << sreprogrammingmode.status << sreprogrammingmode.swdlid << sreprogrammingmode.DeviceID
             << sreprogrammingmode.senderhandle << sreprogrammingmode.SWDLmode;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SReporgrammingMode  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SReporgrammingMode &sreprogrammingmode)
{
    argument.beginStructure();

    argument >> sreprogrammingmode.status >> sreprogrammingmode.swdlid >> sreprogrammingmode.DeviceID
             >> sreprogrammingmode.senderhandle >> sreprogrammingmode.SWDLmode;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit update property parameters*/
struct SInitUpdate
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    uint day;
    uint month;
    uint year;
    QString diskID;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SInitUpdate)

/** Operator overloaded to copy the data from structure SInitUpdate over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SInitUpdate &sinitupdate)
{
    argument.beginStructure();
    argument << sinitupdate.status << sinitupdate.swdlid << sinitupdate.DeviceID
             << sinitupdate.senderhandle << sinitupdate.day << sinitupdate.month
             << sinitupdate.year << sinitupdate.diskID;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SRequestDownload  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SInitUpdate &sinitupdate)
{
    argument.beginStructure();
    argument >> sinitupdate.status >> sinitupdate.swdlid >> sinitupdate.DeviceID
             >> sinitupdate.senderhandle >> sinitupdate.day >> sinitupdate.month
             >> sinitupdate.year >> sinitupdate.diskID;
    argument.endStructure();
    return argument;
}
/** structure definition for storing unit request download property parameters*/
struct SRequestDownload
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    uint moduletype;
    uint moduleindex;
    uint blocksize;
    uint swdlchannel;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SRequestDownload)

/** Operator overloaded to copy the data from structure SRequestDownload over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SRequestDownload &srequestdownload)
{
    argument.beginStructure();
    argument << srequestdownload.status << srequestdownload.swdlid << srequestdownload.DeviceID
             << srequestdownload.senderhandle << srequestdownload.moduletype << srequestdownload.moduleindex
             << srequestdownload.blocksize << srequestdownload.swdlchannel;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SRequestDownload  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SRequestDownload &srequestdownload)
{
    argument.beginStructure();
    argument >> srequestdownload.status >> srequestdownload.swdlid >> srequestdownload.DeviceID
             >> srequestdownload.senderhandle >> srequestdownload.moduletype >> srequestdownload.moduleindex
             >> srequestdownload.blocksize >> srequestdownload.swdlchannel;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit transferdataAMS/CMS property parameters*/
struct STransferDataCMSAMS
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    qulonglong blockcounter;
    QByteArray swdldata;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(STransferDataCMSAMS)

/** Operator overloaded to copy the data from structure STransferDataCMSAMS over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const STransferDataCMSAMS &stransferdataCMS)
{
    argument.beginStructure();
    argument << stransferdataCMS.status << stransferdataCMS.swdlid << stransferdataCMS.DeviceID
             << stransferdataCMS.senderhandle << stransferdataCMS.blockcounter << stransferdataCMS.swdldata;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure STransferDataCMSAMS  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, STransferDataCMSAMS &stransferdataCMS)
{
    argument.beginStructure();
    argument >> stransferdataCMS.status >> stransferdataCMS.swdlid >> stransferdataCMS.DeviceID
             >> stransferdataCMS.senderhandle >> stransferdataCMS.blockcounter >> stransferdataCMS.swdldata;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit Prepare File Transfer property parameters*/
struct SPrepareFileTransfer
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    qulonglong filesize;
    QString swdlFilename;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SPrepareFileTransfer)
/** Operator overloaded to copy the data from structure SPrepareFileTransfer over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SPrepareFileTransfer &spreparefiletransfer)
{
    argument.beginStructure();
    argument << spreparefiletransfer.status << spreparefiletransfer.swdlid << spreparefiletransfer.DeviceID
             << spreparefiletransfer.senderhandle << spreparefiletransfer.filesize << spreparefiletransfer.swdlFilename;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SPrepareFileTransfer  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPrepareFileTransfer &spreparefiletransfer)
{
    argument.beginStructure();
    argument >> spreparefiletransfer.status >> spreparefiletransfer.swdlid >> spreparefiletransfer.DeviceID
             >> spreparefiletransfer.senderhandle >> spreparefiletransfer.filesize >> spreparefiletransfer.swdlFilename;
    argument.endStructure();
    return argument;
}

struct SFileInfoforSWDL
{
    uint fileSize;
    QString fileName;
    uint blocksize;
    QString filePath;
    uint transfertype;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SFileInfoforSWDL)

/** Operator overloaded to copy the data from structure SFileInfoforSWDL over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SFileInfoforSWDL &filetransferinfo)
{
    argument.beginStructure();
    argument << filetransferinfo.fileSize << filetransferinfo.fileName << filetransferinfo.blocksize
             << filetransferinfo.filePath << filetransferinfo.transfertype;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SFileInfoforSWDL  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SFileInfoforSWDL &filetransferinfo)
{
    argument.beginStructure();
    argument >> filetransferinfo.fileSize >> filetransferinfo.fileName >> filetransferinfo.blocksize
             >> filetransferinfo.filePath >> filetransferinfo.transfertype;
    argument.endStructure();
    return argument;
}


/** structure definition for storing unit Erase memory property parameters*/
struct SEraseMemory
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint Senderhandle;
    uint moduletype;
    uint moduleindex;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SEraseMemory)

/** Operator overloaded to copy the data from structure SEraseMemory over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SEraseMemory &serasememory)
{
    argument.beginStructure();

    argument << serasememory.status << serasememory.swdlid << serasememory.DeviceID << serasememory.Senderhandle <<
                serasememory.moduletype << serasememory.moduleindex;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SEraseMemory  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SEraseMemory &serasememory)
{
    argument.beginStructure();
    argument >> serasememory.status >> serasememory.swdlid >> serasememory.DeviceID
             >> serasememory.Senderhandle >> serasememory.moduletype >> serasememory.moduleindex;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit check activate module property parameters*/
struct SCheckActivateModule
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
    QByteArray checksum;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SCheckActivateModule)

/** Operator overloaded to copy the data from structure SCheckActivateModule over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SCheckActivateModule &scheckactivatemodule)
{
    argument.beginStructure();
    argument << scheckactivatemodule.status << scheckactivatemodule.swdlid << scheckactivatemodule.DeviceID
             << scheckactivatemodule.senderhandle << scheckactivatemodule.checksum;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SCheckActivateModule  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCheckActivateModule &scheckactivatemodule)
{
    argument.beginStructure();
    argument >> scheckactivatemodule.status >> scheckactivatemodule.swdlid >> scheckactivatemodule.DeviceID
             >> scheckactivatemodule.senderhandle >> scheckactivatemodule.checksum;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit request transfer exit property parameters*/
struct SRequestTransferExit
{
    uint status;
    uint swdlid;
    ushort DeviceID;
    uint senderhandle;
};
/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SRequestTransferExit)

/** Operator overloaded to copy the data from structure SRequestTransferExit over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SRequestTransferExit &srequesttransferexit)
{
    argument.beginStructure();
    argument << srequesttransferexit.status << srequesttransferexit.swdlid << srequesttransferexit.DeviceID << srequesttransferexit.senderhandle;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SRequestTransferExit */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SRequestTransferExit &srequesttransferexit)
{
    argument.beginStructure();
    argument >> srequesttransferexit.status >> srequesttransferexit.swdlid >> srequesttransferexit.DeviceID >> srequesttransferexit.senderhandle;
    argument.endStructure();
    return argument;
}

/** structure definition for storing unit module version property parameters*/
struct SModuleVerisonList
{
    uint moduletype;
    QString moduleversion;
};

/** structure definition for storing unit module version property parameters*/
struct SModuleVersionListInfo
{
    uint infosize;
    QList<SModuleVerisonList> moduleversionlistinfo;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SModuleVerisonList)
Q_DECLARE_METATYPE(SModuleVersionListInfo)

/** Operator overloaded to copy the data from structure SModuleVerisonList over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SModuleVerisonList &smoduleversionlist)
{
    argument.beginStructure();
    argument << smoduleversionlist.moduletype << smoduleversionlist.moduleversion;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SModuleVerisonList  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SModuleVerisonList &smoduleversionlist)
{
    argument.beginStructure();

    argument >> smoduleversionlist.moduletype >> smoduleversionlist.moduleversion;
    argument.endStructure();
    return argument;
}
/** Operator overloaded to copy the data from structure SModuleVersionListInfo over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SModuleVersionListInfo &smoduleversionlistinfo)
{
    argument.beginStructure();
    argument << smoduleversionlistinfo.infosize;
    for(uint count = 0; count < smoduleversionlistinfo.infosize ; count++)
    {
        argument << smoduleversionlistinfo.moduleversionlistinfo[count].moduletype
                 << smoduleversionlistinfo.moduleversionlistinfo[count].moduleversion;
    }
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SModuleVersionListInfo */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SModuleVersionListInfo &smoduleversionlistinfo)
{
    argument.beginStructure();
    argument >> smoduleversionlistinfo.infosize;
    SModuleVerisonList dummymoduleversion[smoduleversionlistinfo.infosize];

    for(uint count = 0; count < smoduleversionlistinfo.infosize; count++)
    {
        smoduleversionlistinfo.moduleversionlistinfo.append(dummymoduleversion[count]);
    }

    for(uint cnt = 0; cnt < smoduleversionlistinfo.infosize; cnt++)
    {
        argument >> smoduleversionlistinfo.moduleversionlistinfo[cnt].moduletype
                 >> smoduleversionlistinfo.moduleversionlistinfo[cnt].moduleversion;
    }

    argument.endStructure();
    return argument;
}

/** STRUCTURES FOR JUKEBOX */
struct SMediadata
{
    uint mode;
    uint tag;
    QString filename;
    QString artist;
    QString album;
    QString genre;
};

struct SCollectMediaData
{
    uint     state;
    ushort   senderHandle;
    uint     errorcode;
    quint32  arraycount;
    QString  errorinfo;
    QList<SMediadata> mediadatalist;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SCollectMediaData)
Q_DECLARE_METATYPE(SMediadata)

/** Operator overloaded to copy the data from structure PopUpResultData over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SMediadata &mediaData)
{
    argument.beginStructure();
    argument << mediaData.mode << mediaData.tag << mediaData.filename << mediaData.artist
             << mediaData.album << mediaData.genre;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure PopUpResultData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SMediadata &mediaData)
{
    argument.beginStructure();
    argument >> mediaData.mode >> mediaData.tag >> mediaData.filename >> mediaData.artist
             >> mediaData.album >> mediaData.genre;
    argument.endStructure();
    return argument;
}
/** Operator overloaded to copy the data from structure PopUpResultData over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SCollectMediaData &CollectMediaData)
{
    argument.beginStructure();

    argument << CollectMediaData.state << CollectMediaData.senderHandle << CollectMediaData.errorcode
             << CollectMediaData.arraycount << CollectMediaData.errorinfo;

    for(uint count = 0;count < CollectMediaData.arraycount; count ++)
    {
        argument << CollectMediaData.mediadatalist[count].mode
                 << CollectMediaData.mediadatalist[count].tag
                 << CollectMediaData.mediadatalist[count].filename
                 << CollectMediaData.mediadatalist[count].artist
                 << CollectMediaData.mediadatalist[count].album
                 << CollectMediaData.mediadatalist[count].genre;
    }

    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure PopUpResultData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCollectMediaData &CollectMediaData)
{
    argument.beginStructure();

    argument >> CollectMediaData.state >> CollectMediaData.senderHandle >> CollectMediaData.errorcode >> CollectMediaData.arraycount
             >> CollectMediaData.errorinfo;

    SMediadata dummymediadatalist[CollectMediaData.arraycount];

    for(uint count = 0; count < CollectMediaData.arraycount; count++)
    {
        CollectMediaData.mediadatalist.append(dummymediadatalist[count]);
    }

    for(uint cnt = 0; cnt < CollectMediaData.arraycount; cnt++)
    {
        argument >> CollectMediaData.mediadatalist[cnt].mode
                 >> CollectMediaData.mediadatalist[cnt].tag
                 >> CollectMediaData.mediadatalist[cnt].filename
                 >> CollectMediaData.mediadatalist[cnt].artist
                 >> CollectMediaData.mediadatalist[cnt].album
                 >> CollectMediaData.mediadatalist[cnt].genre;
    }

    argument.endStructure();
    return argument;
}


struct STunerStatusData
{
    ushort  tag;
    ushort ensID;
    uchar ensECC;
    uchar sidType;
    quint32 sid;
    ushort scidi;
    bool sync;
    bool serviceComponentValid;
    bool mute;
    QString frequencyLabel;
    quint32 frequency;
    uchar signalQuality;
};

struct STunerStatusDataList
{
    uint listCount;
    QList<STunerStatusData> dataList;
};

Q_DECLARE_METATYPE(STunerStatusData)
Q_DECLARE_METATYPE(STunerStatusDataList)

/** Marshalling STunerStatusData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const STunerStatusData &tunerStatusData)
{
    argument.beginStructure();
    argument << tunerStatusData.tag << tunerStatusData.ensID
             << tunerStatusData.ensECC << tunerStatusData.sidType
             << tunerStatusData.sid << tunerStatusData.scidi
             << tunerStatusData.sync << tunerStatusData.serviceComponentValid
             << tunerStatusData.mute << tunerStatusData.frequencyLabel
             << tunerStatusData.frequency << tunerStatusData.signalQuality;
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to STunerStatusData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, STunerStatusData &tunerStatusData)
{
    argument.beginStructure();
    argument >> tunerStatusData.tag >> tunerStatusData.ensID
               >> tunerStatusData.ensECC >> tunerStatusData.sidType
               >> tunerStatusData.sid >> tunerStatusData.scidi
               >> tunerStatusData.sync >> tunerStatusData.serviceComponentValid
               >> tunerStatusData.mute >> tunerStatusData.frequencyLabel
               >> tunerStatusData.frequency >> tunerStatusData.signalQuality;
    argument.endStructure();
    return argument;
}

/** Marshalling STunerStatusDataList to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const STunerStatusDataList &tunerStatusDataList)
{
    argument.beginStructure();

    argument << tunerStatusDataList.listCount;
    for(uint count =0; count < tunerStatusDataList.listCount; count++ ){
        argument << tunerStatusDataList.dataList[count].tag
                 << tunerStatusDataList.dataList[count].ensID
                 << tunerStatusDataList.dataList[count].ensECC
                 << tunerStatusDataList.dataList[count].sidType
                 << tunerStatusDataList.dataList[count].sid
                 << tunerStatusDataList.dataList[count].scidi
                 << tunerStatusDataList.dataList[count].sync
                 << tunerStatusDataList.dataList[count].serviceComponentValid
                 << tunerStatusDataList.dataList[count].mute
                 << tunerStatusDataList.dataList[count].frequencyLabel
                 << tunerStatusDataList.dataList[count].frequency
                 << tunerStatusDataList.dataList[count].signalQuality;
    }
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to STunerStatusDataList */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, STunerStatusDataList &tunerStatusDataList)
{
    argument.beginStructure();
    tunerStatusDataList.dataList.clear();
    argument >> tunerStatusDataList.listCount;

    STunerStatusData dummyTunerStatusData[tunerStatusDataList.listCount];
    for( uint cnt = 0; cnt < tunerStatusDataList.listCount; cnt++ )
    {
        tunerStatusDataList.dataList.append(dummyTunerStatusData[cnt]);
    }
    for( uint count =0; count < tunerStatusDataList.listCount; count++ )
    {
        argument >> tunerStatusDataList.dataList[count].tag
                >> tunerStatusDataList.dataList[count].ensID
                >> tunerStatusDataList.dataList[count].ensECC
                >> tunerStatusDataList.dataList[count].sidType
                >> tunerStatusDataList.dataList[count].sid
                >> tunerStatusDataList.dataList[count].scidi
                >> tunerStatusDataList.dataList[count].sync
                >> tunerStatusDataList.dataList[count].serviceComponentValid
                >> tunerStatusDataList.dataList[count].mute
                >> tunerStatusDataList.dataList[count].frequencyLabel
                >> tunerStatusDataList.dataList[count].frequency
                >> tunerStatusDataList.dataList[count].signalQuality;
    }

    argument.endStructure();
    return argument;
}

/** Structure Definitions for DVD control Fblock */
struct SPlayPositionInfo{
    ushort titleCurNumber;
    uint titleCurTime;
    uint titleTotalTime;
    uint chapterCurNumber;
    uint chapterCurTime;
    uint ChapterTotalTime;
    QString titleTag;
    QString subDirectoryTag;
};

Q_DECLARE_METATYPE(SPlayPositionInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const SPlayPositionInfo &playPositionStatus)
{
    argument.beginStructure();
    argument << playPositionStatus.titleCurNumber << playPositionStatus.titleCurTime
             << playPositionStatus.titleTotalTime << playPositionStatus.chapterCurNumber
             << playPositionStatus.chapterCurTime << playPositionStatus.ChapterTotalTime
             << playPositionStatus.titleTag << playPositionStatus.subDirectoryTag;
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPlayPositionInfo &playPositionStatus)
{
    argument.beginStructure();
    argument >> playPositionStatus.titleCurNumber >> playPositionStatus.titleCurTime
             >> playPositionStatus.titleTotalTime >> playPositionStatus.chapterCurNumber
             >> playPositionStatus.chapterCurTime >> playPositionStatus.ChapterTotalTime
             >> playPositionStatus.titleTag >> playPositionStatus.subDirectoryTag;
    argument.endStructure();
    return argument;
}

/** Structure Definitions for HMI Control */
/** Structure definition for Storing parameters required for
 *  pop up request
 */
struct SPopUpRequestData{
    ushort   senderHandle;
    uint     target;
    uint     displayHandle;
    uchar    popupSize;
    uchar    popupType;
    uchar    titleNo;
    uchar    listNo;
    uchar    textLineNo;
    uchar    iconID;
    uchar    iconPos;
    uchar    button1;
    uchar    button2;
    uchar    button3;
    uchar    button4;
    QString popupData;
    QString OSDText;
    ushort  deviceID;
};

/** Structure definition for returning parameters
 *  as apart of popup result
 */
struct SPopUpResultData
{
    uint     state;
    ushort   senderHandle;
    uint     target;
    uint 	 displayHandle;
    uchar    reactionButton;
    uchar    reactionList;
    QString  reactionText;
    ushort   deviceId;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SPopUpRequestData)
Q_DECLARE_METATYPE(SPopUpResultData)

/** Operator overloaded to copy the data from structure SPopUpRequestData over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SPopUpRequestData &popUpRequestData)
{
    argument.beginStructure();
    argument << popUpRequestData.senderHandle << popUpRequestData.target
             << popUpRequestData.displayHandle << popUpRequestData.popupSize
             << popUpRequestData.popupType <<  popUpRequestData.titleNo
             << popUpRequestData.listNo << popUpRequestData.textLineNo
             << popUpRequestData.iconID << popUpRequestData.iconPos
             << popUpRequestData.button1 << popUpRequestData.button2
             << popUpRequestData.button3 << popUpRequestData.button4
             << popUpRequestData.popupData << popUpRequestData.OSDText
             << popUpRequestData.deviceID ;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SPopUpRequestData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPopUpRequestData &popUpRequestData)
{
    argument.beginStructure();
    argument >> popUpRequestData.senderHandle >> popUpRequestData.target
             >> popUpRequestData.displayHandle >> popUpRequestData.popupSize
             >> popUpRequestData.popupType >> popUpRequestData.titleNo
             >> popUpRequestData.listNo >> popUpRequestData.textLineNo
             >> popUpRequestData.iconID >> popUpRequestData.iconPos
             >> popUpRequestData.button1 >> popUpRequestData.button2
             >> popUpRequestData.button3 >> popUpRequestData.button4
             >> popUpRequestData.popupData >> popUpRequestData.OSDText
             >> popUpRequestData.deviceID;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from structure SPopUpResultData over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SPopUpResultData &popUpResultData)
{
    argument.beginStructure();
    argument << popUpResultData.senderHandle << popUpResultData.target
             << popUpResultData.displayHandle << popUpResultData.reactionButton
             << popUpResultData.reactionList << popUpResultData.reactionText
             << popUpResultData.state << popUpResultData.deviceId;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SPopUpResultData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPopUpResultData &popUpResultData)
{
    argument.beginStructure();
    argument >> popUpResultData.senderHandle >> popUpResultData.target
             >> popUpResultData.displayHandle >> popUpResultData.reactionButton
             >> popUpResultData.reactionList >> popUpResultData.reactionText
             >> popUpResultData.state >> popUpResultData.deviceId;
    argument.endStructure();
    return argument;
}

struct SDate
{
    ushort year;
    uchar month;
    uchar day;
    uint datetype;
};
Q_DECLARE_METATYPE(SDate)

/** Demarshalling QDBusArgument to SCallStateData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SDate &systemDate)
{
    argument.beginStructure();
    argument >> systemDate.year >> systemDate.month >> systemDate.day >> systemDate.datetype;
    argument.endStructure();
    return argument;
}

/** Marshalling SCallStateData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SDate &systemDate)
{
    argument.beginStructure();
    argument << systemDate.year << systemDate.month << systemDate.day << systemDate.datetype;
    argument.endStructure();
    return argument;
}

/** Structure Definitions  for IBOX control */
/** Structure definition for parameters of SGISRequestData */
struct SGISRequestData{
    uint    state;
    ushort  senderhandle;
    QString uri;
    QString transactionID;
    bool    success;
    QByteArray gisdata;
};

/** Structure definition for parameters of SPOIData */
struct SPOIData{
    uchar       src;
    uchar       lang;
    uchar       cate;
    QString     rating;
    QString     phone;
    QString     addr;
    QString     zip;
    QString     name;
    QString     note;
    QString     url;
    ushort      angle;
    QString     distance;
    QString     direction;
    int         lat;
    int         lon;
    uchar       type;
    uint        alt;    
};

struct SPOIDataRequest{
    uint    state;
    ushort  senderhandle;
    QString uri;
    uint    arrayCount;
    QString attributionsInfo;
    QList<SPOIData> poiDataList;
};


/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SGISRequestData)
Q_DECLARE_METATYPE(SPOIData)
Q_DECLARE_METATYPE(SPOIDataRequest)

/** Marshalling SGISRequestData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SGISRequestData &gisRequestData)
{
    argument.beginStructure();
    argument << gisRequestData.senderhandle << gisRequestData.uri << gisRequestData.transactionID
             << gisRequestData.state << gisRequestData.success
             << gisRequestData.gisdata;
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SGISRequestData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SGISRequestData &gisRequestData)
{
    argument.beginStructure();
    argument >> gisRequestData.senderhandle >> gisRequestData.uri >> gisRequestData.transactionID
             >> gisRequestData.state >> gisRequestData.success
             >> gisRequestData.gisdata;
    argument.endStructure();
    return argument;
}

/** Marshalling SPOIData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SPOIData &poiData)
{
    argument.beginStructure();
    argument << poiData.src << poiData.lang << poiData.cate << poiData.rating
             << poiData.phone << poiData.addr << poiData.zip << poiData.name
             << poiData.note << poiData.url << poiData.angle << poiData.distance
             << poiData.direction << poiData.lat << poiData.lon << poiData.type
             << poiData.alt;
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SPOIData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPOIData &poiData)
{
    argument.beginStructure();
    argument >> poiData.src >> poiData.lang >> poiData.cate >> poiData.rating
             >> poiData.phone >> poiData.addr >> poiData.zip >> poiData.name
             >> poiData.note >> poiData.url >> poiData.angle >> poiData.distance
             >> poiData.direction >> poiData.lat >> poiData.lon >> poiData.type
             >> poiData.alt;
    argument.endStructure();
    return argument;
}

/** Marshalling SPOIDataRequest to QDBusArgument */
inline const QDBusArgument &operator<<(QDBusArgument &argument, const SPOIDataRequest &poiDataRequest)
{
    argument.beginStructure();

    argument << poiDataRequest.state
             << poiDataRequest.senderhandle
             << poiDataRequest.uri
             << poiDataRequest.arrayCount
             << poiDataRequest.attributionsInfo;

    for(uint count =0; count < poiDataRequest.arrayCount; count++ )
    {
        argument << poiDataRequest.poiDataList[count].src
                 << poiDataRequest.poiDataList[count].lang
                 << poiDataRequest.poiDataList[count].cate
                 << poiDataRequest.poiDataList[count].rating
                 << poiDataRequest.poiDataList[count].phone
                 << poiDataRequest.poiDataList[count].addr
                 << poiDataRequest.poiDataList[count].zip
                 << poiDataRequest.poiDataList[count].name
                 << poiDataRequest.poiDataList[count].note
                 << poiDataRequest.poiDataList[count].url
                 << poiDataRequest.poiDataList[count].angle
                 << poiDataRequest.poiDataList[count].distance
                 << poiDataRequest.poiDataList[count].direction
                 << poiDataRequest.poiDataList[count].lat
                 << poiDataRequest.poiDataList[count].lon
                 << poiDataRequest.poiDataList[count].type
                 << poiDataRequest.poiDataList[count].alt;
    }

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SPOIDataRequest */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPOIDataRequest &poiDataRequest)
{
    argument.beginStructure();
    poiDataRequest.poiDataList.clear();

    argument >> poiDataRequest.state
             >> poiDataRequest.senderhandle
             >> poiDataRequest.uri
             >> poiDataRequest.arrayCount
             >> poiDataRequest.attributionsInfo;

    SPOIData dummyPOIdata[poiDataRequest.arrayCount];

    for( uint cnt = 0; cnt < poiDataRequest.arrayCount; cnt++ )
    {
        poiDataRequest.poiDataList.append(dummyPOIdata[cnt]);
    }

    for( uint count =0; count < poiDataRequest.arrayCount; count++ )
    {
        argument >> poiDataRequest.poiDataList[count].src
                 >> poiDataRequest.poiDataList[count].lang
                 >> poiDataRequest.poiDataList[count].cate
                 >> poiDataRequest.poiDataList[count].rating
                 >> poiDataRequest.poiDataList[count].phone
                 >> poiDataRequest.poiDataList[count].addr
                 >> poiDataRequest.poiDataList[count].zip
                 >> poiDataRequest.poiDataList[count].name
                 >> poiDataRequest.poiDataList[count].note
                 >> poiDataRequest.poiDataList[count].url
                 >> poiDataRequest.poiDataList[count].angle
                 >> poiDataRequest.poiDataList[count].distance
                 >> poiDataRequest.poiDataList[count].direction
                 >> poiDataRequest.poiDataList[count].lat
                 >> poiDataRequest.poiDataList[count].lon
                 >> poiDataRequest.poiDataList[count].type
                 >> poiDataRequest.poiDataList[count].alt;
    }

    argument.endStructure();
    return argument;
}



/** Structure Definitions for Navigation COntrol class */
/** structure for GPSRawData and GPSRawDataBessel*/
struct SGPSData{
    qlonglong longitude;
    qlonglong latitude;
    uint FixGPS;
    uint HDOP;
    uint VDOP;
    int speedGPS;
    uint headingGPS;
    int height;
    uint year;
    uint month;
    uint day;
    uint hour;
    uint min;
    uint sec;
};

Q_DECLARE_METATYPE(SGPSData)

/** Operator overloaded to copy the data from structure SGPSData over QDbusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SGPSData &gpsdata)
{
    argument.beginStructure();
    argument << gpsdata.longitude << gpsdata.latitude << gpsdata.FixGPS << gpsdata.HDOP << gpsdata.VDOP
             << gpsdata.speedGPS << gpsdata.headingGPS << gpsdata.height << gpsdata.year << gpsdata.month
             << gpsdata.day << gpsdata.hour << gpsdata.min << gpsdata.sec;
    argument.endStructure();
    return argument;
}

/** Operator overloaded to copy the data from QDbusArgument over structure SGPSData  */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SGPSData &gpsdata)
{
    argument.beginStructure();

    argument >> gpsdata.longitude >> gpsdata.latitude >> gpsdata.FixGPS >> gpsdata.HDOP >> gpsdata.VDOP
             >> gpsdata.speedGPS >> gpsdata.headingGPS >> gpsdata.height >> gpsdata.year >> gpsdata.month
             >> gpsdata.day >> gpsdata.hour >> gpsdata.min >> gpsdata.sec;
    argument.endStructure();
    return argument;
}


/** StructureDefinitions for Telephone Control */

/** Structure definition for Storing parameters required for
 *  SPBookData
 */
struct SPBookData
{       
    QString name;    
    uchar   contact1;
    QString teleNum1;        
    uchar   contact2;
    QString teleNum2;
    uchar   contact3;
    QString teleNum3;
    uchar   contact4;
    QString teleNum4;
    uchar   contact5;
    QString teleNum5;    
};

struct SCollectPBook
{
    uint    state;
    ushort  senderHandle;
    uchar   pBookType;
    uint    arrayCount;
    QList<SPBookData> pBookDataList;
};


/** Declaring the structure over QtDbus */
Q_DECLARE_METATYPE(SPBookData)
Q_DECLARE_METATYPE(SCollectPBook)

/** Marshalling SPBookData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SPBookData &pBookData)
{
    argument.beginStructure();
    argument << pBookData.name
             << pBookData.contact1 << pBookData.teleNum1
             << pBookData.contact2 << pBookData.teleNum2
             << pBookData.contact3 << pBookData.teleNum3
             << pBookData.contact4 << pBookData.teleNum4
             << pBookData.contact5 << pBookData.teleNum5;
    argument.endStructure();
    return argument;
}

/** DeMarshalling QDBusArgument to SPBookData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SPBookData &pBookData)
{
    argument.beginStructure();
    argument >> pBookData.name
             >> pBookData.contact1 >> pBookData.teleNum1
             >> pBookData.contact2 >> pBookData.teleNum2
             >> pBookData.contact3 >> pBookData.teleNum3
             >> pBookData.contact4 >> pBookData.teleNum4
             >> pBookData.contact5 >> pBookData.teleNum5;
    argument.endStructure();
    return argument;
}


/** Marshalling SCollectPBook to QDBusArgument */
inline const QDBusArgument &operator<<(QDBusArgument &argument, const SCollectPBook &collectPBook)
{
    argument.beginStructure();
    argument << collectPBook.state << collectPBook.senderHandle << collectPBook.pBookType << collectPBook.arrayCount;    

    /*
    argument.beginArray( qMetaTypeId<SPBookData>() );
    for( uint count=0; count<collectPBook.arrayCount; count++ )
    {
        argument << collectPBook.pBookDataList[count].name
                 << collectPBook.pBookDataList[count].contact1
                 << collectPBook.pBookDataList[count].teleNum1
                 << collectPBook.pBookDataList[count].contact2
                 << collectPBook.pBookDataList[count].teleNum2
                 << collectPBook.pBookDataList[count].contact3
                 << collectPBook.pBookDataList[count].teleNum3
                 << collectPBook.pBookDataList[count].contact4
                 << collectPBook.pBookDataList[count].teleNum4
                 << collectPBook.pBookDataList[count].contact5
                 << collectPBook.pBookDataList[count].teleNum5;
    }
    argument.endArray();
    */

    argument.beginArray( qMetaTypeId<SPBookData>() );
    for( uint count=0; count<collectPBook.arrayCount; count++ )
    {
        argument << collectPBook.pBookDataList[count];
    }
    argument.endArray();

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SCollectPBook */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCollectPBook &collectPBook)
{
    argument.beginStructure();
    collectPBook.pBookDataList.clear();
    argument >> collectPBook.state >> collectPBook.senderHandle >> collectPBook.pBookType >> collectPBook.arrayCount ;

    /*
    SPBookData dummyPBookdata[collectPBook.arrayCount];

    for( uint cnt=0; cnt<collectPBook.arrayCount; cnt++ )
    {
        collectPBook.pBookDataList.append(dummyPBookdata[cnt]);
    }    

    for( uint count=0; count<collectPBook.arrayCount; count++ )
    {
        argument >> collectPBook.pBookDataList[count].name
                 >> collectPBook.pBookDataList[count].contact1
                 >> collectPBook.pBookDataList[count].teleNum1
                 >> collectPBook.pBookDataList[count].contact2
                 >> collectPBook.pBookDataList[count].teleNum2
                 >> collectPBook.pBookDataList[count].contact3
                 >> collectPBook.pBookDataList[count].teleNum3
                 >> collectPBook.pBookDataList[count].contact4
                 >> collectPBook.pBookDataList[count].teleNum4
                 >> collectPBook.pBookDataList[count].contact5
                 >> collectPBook.pBookDataList[count].teleNum5;
    }
    */

    argument.beginArray();
    for( uint count=0; count<collectPBook.arrayCount; count++ )
    {
        SPBookData pBookData;
        argument >> pBookData;
        collectPBook.pBookDataList.append(pBookData);
    }
    argument.endArray();    

    argument.endStructure();
    return argument;
}

/** Structures definition for storing array of CallHistory Info */
struct SCallHistoryInfo
{
    uint    mode;
    uint    callDirection;
    QString name;
    QString telNumber;
    QString dateNTime;
};

struct SCallHistoryInfoList
{
    uint listCount;
    QList<SCallHistoryInfo> infoList;
};

/** Declaring the structures over QtDbus */
Q_DECLARE_METATYPE(SCallHistoryInfo)
Q_DECLARE_METATYPE(SCallHistoryInfoList)

/** Demarshalling QDBusArgument to SCallHistoryInfo */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCallHistoryInfo &callHistoryInfo)
{
    argument.beginStructure();
    argument >> callHistoryInfo.mode >> callHistoryInfo.callDirection
             >> callHistoryInfo.name >> callHistoryInfo.telNumber
             >> callHistoryInfo.dateNTime;
    argument.endStructure();
    return argument;
}

/** Marshalling SCallHistoryInfo to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SCallHistoryInfo &callHistoryInfo)
{
    argument.beginStructure();
    argument << callHistoryInfo.mode << callHistoryInfo.callDirection
             << callHistoryInfo.name << callHistoryInfo.telNumber
             << callHistoryInfo.dateNTime;
    argument.endStructure();
    return argument;
}

/** Marshalling SCallHistoryInfoList to QDBusArgument */
inline const QDBusArgument &operator<<(QDBusArgument &argument, const SCallHistoryInfoList &callHistoryInfoList)
{
    argument.beginStructure();
    argument << callHistoryInfoList.listCount;

    /*
    for(uint count =0; count < callHistoryInfoList.listCount; count++ ){
        argument << callHistoryInfoList.infoList[count].mode
                 << callHistoryInfoList.infoList[count].callDirection
                 << callHistoryInfoList.infoList[count].name
                 << callHistoryInfoList.infoList[count].telNumber
                 << callHistoryInfoList.infoList[count].dateNTime;
    }
    */

    argument.beginArray( qMetaTypeId<SCallHistoryInfo>() );
    for( uint count=0; count<callHistoryInfoList.listCount; count++ )
    {
        argument << callHistoryInfoList.infoList[count];
    }
    argument.endArray();

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SCallHistoryInfoList */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCallHistoryInfoList &callHistoryInfoList)
{
    argument.beginStructure();
    callHistoryInfoList.infoList.clear();
    argument >> callHistoryInfoList.listCount;

    /*
    SCallHistoryInfo dummyCallHistorydata[callHistoryInfoList.listCount];
    for( uint cnt = 0; cnt < callHistoryInfoList.listCount; cnt++ ){
        callHistoryInfoList.infoList.append(dummyCallHistorydata[cnt]);
    }
    for( uint count =0; count < callHistoryInfoList.listCount; count++ ){
       argument >> callHistoryInfoList.infoList[count].mode
                >> callHistoryInfoList.infoList[count].callDirection
                >> callHistoryInfoList.infoList[count].name
                >> callHistoryInfoList.infoList[count].telNumber
                >> callHistoryInfoList.infoList[count].dateNTime;
    }
    */

    argument.beginArray();
    for( uint count=0; count<callHistoryInfoList.listCount; count++ )
    {
        SCallHistoryInfo callHistoryInfo;
        argument >> callHistoryInfo;
        callHistoryInfoList.infoList.append(callHistoryInfo);
    }
    argument.endArray();

    argument.endStructure();
    return argument;
}

/** Structures definition for storing array of DialNumber Info */
struct SDialNumberData
{
    uint    state;
    ushort  senderHandle;
    QString telNumber;
    uint    callId;
    QString errorInfo;
    uint errorCode;
};

/** Declaring the structure over QtDbus */
Q_DECLARE_METATYPE(SDialNumberData)

/** Marshalling SDialNumberData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SDialNumberData &dialnumData)
{
    argument.beginStructure();
    argument << dialnumData.senderHandle << dialnumData.state
             << dialnumData.callId << dialnumData.telNumber
             << dialnumData.errorInfo << dialnumData.errorCode;
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SDialNumberData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SDialNumberData &dialnumData)
{
    argument.beginStructure();
    argument >> dialnumData.senderHandle >> dialnumData.state
             >> dialnumData.callId >> dialnumData.telNumber
             >> dialnumData.errorInfo >> dialnumData.errorCode;
    argument.endStructure();
    return argument;
}

struct STunedProgram
{
    uint tvType;
    QString serviceName;
    uint system;
    uint channel_99;
    uint servicetablerow;
    QString countryISO3166;
    uint status;
    uint posInBrowserList;

};

/** Declaring the structure over QtDbus */
Q_DECLARE_METATYPE(STunedProgram)

/** Marshalling SDialNumberData to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const STunedProgram &tunedProgram)
{
    argument.beginStructure();
    argument << tunedProgram.tvType << tunedProgram.serviceName << tunedProgram.system
             << tunedProgram.channel_99 << tunedProgram.servicetablerow << tunedProgram.countryISO3166
             << tunedProgram.status << tunedProgram.posInBrowserList;

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SDialNumberData */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, STunedProgram &tunedProgram)
{
    argument.beginStructure();
    argument >> tunedProgram.tvType >> tunedProgram.serviceName >> tunedProgram.system
            >> tunedProgram.channel_99 >> tunedProgram.servicetablerow >> tunedProgram.countryISO3166
            >> tunedProgram.status >> tunedProgram.posInBrowserList;

    argument.endStructure();
    return argument;
}


struct SDisplayOSD
{
    uchar target;
    uchar osdId;
    uchar osdControl;
    uchar osdMode;
    uchar textColor;
    QString osdText;
};

/** Declaring the structure over QtDbus */
Q_DECLARE_METATYPE(SDisplayOSD)

/** Marshalling SDisplayOSD to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SDisplayOSD &displayOSD)
{
    argument.beginStructure();
    argument << displayOSD.target
             << displayOSD.osdId
             << displayOSD.osdControl
             << displayOSD.osdMode
             << displayOSD.textColor
             << displayOSD.osdText;
    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SDisplayOSD */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SDisplayOSD &displayOSD)
{
    argument.beginStructure();
    argument >> displayOSD.target
             >> displayOSD.osdId
             >> displayOSD.osdControl
             >> displayOSD.osdMode
             >> displayOSD.textColor
             >> displayOSD.osdText;
    argument.endStructure();
    return argument;
}

struct SSearchRoute
{
    uchar optype;
    ushort senderHandle;
    uchar target;
    int lat;
    int lon;
    QString name;
};

Q_DECLARE_METATYPE(SSearchRoute)

inline QDBusArgument &operator<<(QDBusArgument &argument, const SSearchRoute &searchRoute)
{
    argument.beginStructure();
    argument << searchRoute.optype << searchRoute.senderHandle << searchRoute.target << searchRoute.lat << searchRoute.lon << searchRoute.name;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SSearchRoute &searchRoute)
{
    argument.beginStructure();
    argument >> searchRoute.optype >> searchRoute.senderHandle >> searchRoute.target >> searchRoute.lat >> searchRoute.lon >> searchRoute.name;
    argument.endStructure();
    return argument;
}

struct SShowMap
{
    uchar optype;
    ushort senderHandle;
    uchar target;
    int lat;
    int lon;
    QString name;
};

Q_DECLARE_METATYPE(SShowMap)

inline QDBusArgument &operator<<(QDBusArgument &argument, const SShowMap &showMap)
{
    argument.beginStructure();
    argument << showMap.optype << showMap.senderHandle << showMap.target << showMap.lat << showMap.lon << showMap.name;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SShowMap &showMap)
{
    argument.beginStructure();
    argument >> showMap.optype >> showMap.senderHandle >> showMap.target >> showMap.lat >> showMap.lon >> showMap.name;
    argument.endStructure();
    return argument;
}

struct SRegisterMyLocation
{
    uchar optype;
    ushort senderHandle;
    uchar target;
    int lat;
    int lon;
    QString name;
};

Q_DECLARE_METATYPE(SRegisterMyLocation)

inline QDBusArgument &operator<<(QDBusArgument &argument, const SRegisterMyLocation &registerMyLoacation)
{
    argument.beginStructure();
    argument << registerMyLoacation.optype << registerMyLoacation.senderHandle << registerMyLoacation.target
             << registerMyLoacation.lat << registerMyLoacation.lon << registerMyLoacation.name;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SRegisterMyLocation &registerMyLoacation)
{
    argument.beginStructure();
    argument >> registerMyLoacation.optype >> registerMyLoacation.senderHandle >> registerMyLoacation.target
             >> registerMyLoacation.lat >> registerMyLoacation.lon >> registerMyLoacation.name;
    argument.endStructure();
    return argument;
}

struct STPEGInfo
{
    QString date;
    QString time;
    QString text;
};

Q_DECLARE_METATYPE(STPEGInfo)

inline QDBusArgument &operator<<(QDBusArgument &argument, const STPEGInfo &tpegInfo)
{
    argument.beginStructure();
    argument << tpegInfo.date << tpegInfo.time << tpegInfo.text;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, STPEGInfo &tpegInfo)
{
    argument.beginStructure();
    argument >> tpegInfo.date >> tpegInfo.time >> tpegInfo.text;
    argument.endStructure();
    return argument;
}

// FCat15.3.4
struct SCheckGPSBoundary
{
    uchar optype;
    ushort senderHandle;
    int lat;
    int lon;
};

Q_DECLARE_METATYPE(SCheckGPSBoundary)

inline QDBusArgument &operator<<(QDBusArgument &argument, const SCheckGPSBoundary &checkGPSBoundary)
{
    argument.beginStructure();
    argument << checkGPSBoundary.optype << checkGPSBoundary.senderHandle << checkGPSBoundary.lat  << checkGPSBoundary.lon;
    argument.endStructure();
    return argument;
}

inline const QDBusArgument &operator>>(const QDBusArgument &argument, SCheckGPSBoundary &checkGPSBoundary)
{
    argument.beginStructure();
    argument >> checkGPSBoundary.optype >> checkGPSBoundary.senderHandle >> checkGPSBoundary.lat >> checkGPSBoundary.lon;
    argument.endStructure();
    return argument;
}


#endif // DHAVN_MOSTMANAGER_UTILITY_H
