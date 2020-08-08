/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 The class which provides the data from the co- *
 *                 processor chip, it help to get the certificate *
 *                 , Signature to authorize the IPOD.             *
 *                 and also provide the details like device ID,   *
 *                 version numbers etc.                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODCONTROLLER_LOGOIMAGETRASMITTER_H
#define DHAVN_IPODCONTROLLER_LOGOIMAGETRASMITTER_H

#include <QObject>
#include <QDebug>
#include <DHAVN_IPodController_SetDisplayImage.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <QImage>
#include <DHAVN_LogUtil.h>

using namespace ExtendedLingo;

class CLogoImageTrasmitter : public QObject
{
private:
    USE_LOG_UTIL
public:
    CLogoImageTrasmitter(iPodControllerPrivateData *privateData, bool isTidEnabled, int maxWidth, int maxHeight, int format);
    ~CLogoImageTrasmitter();
    QByteArray TransmittLogo(int index,int m_TransID, int format); // modified by jungae 2012.12.04 ISV63632

private:
    void init();
    void deInit();
    QString getLogoFile();

public:
        int m_logoMaxWidth;
        int m_logoMaxHeight;
        int m_logoFormat;

private:
    /* Logo relate variables */
        int m_logoWidth;
        int m_logoHeight;

        int m_logoCompletIndex;
        int m_logoTotalCount;
        int m_logoChunkSize;
        int m_logoTotalImageSize;
        int m_logoCompletedImageSize;
        int m_logoImageFd;
        bool m_isTidEnabled;


};

#endif // DHAVN_IPODCONTROLLER_LOGOIMAGETRASMITTER_H
