/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 Global function defined in this class to be    *
 *                 ised in multiple files                         *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IPODCONTROLLERGLOBAL_H
#define IPODCONTROLLERGLOBAL_H
#include <qbytearray.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QCryptographicHash>                        // Added by pawan 2013.05.16.





class CIpodControllerGlobal
{
public:
static void printArray(QByteArray array)
{
    USE_LOG_UTIL
    LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "Printing Array of command: " << LOG_ENDL;
    int count = array.count()/8;
    count++;
    for(int i=0;i<count;i++)
    {
        //LOG_INFO<<"[" << i << "] = ["<< (array.mid(i*8, 8)).toHex().data() << "]" << LOG_ENDL;
    }
}

static char checkSum(QByteArray data, int len)
{
    //QByteArray temp(data.mid(1,len));
    char sum=0;
    for(int i=1; i<=len; i++)
    {
        sum +=data[i];
    }
    return -sum;
}

// Added by pawan 2013.05.16.
static quint64 getUIDForStrings(QString albumname, QString artistName, QString titleName, int duration)
{
    QString combinedString;
    combinedString.append(albumname).append(artistName).append(titleName).setNum(duration);

    QByteArray cryp = QCryptographicHash::hash(combinedString.toUtf8(), QCryptographicHash::Md5);
    QString uidstr = cryp.toHex();

    //get 8 byte and convert it to number
    QString myuidstr = (uidstr.mid(0,16)).prepend("0x");

    bool ok;
    quint64 uid = myuidstr.toULongLong(&ok, 16);
    ////LOG_TRACE << "basicp uid is generated: " << uidstr << LOG_ENDL;
    return uid;
}


/*
// Added by pawan 2013.05.16.
static QString getUIDForStrings(QString albumname, QString artistName, QString titleName, int duration)
{
    QString combinedString;
    combinedString.append(albumname).append(artistName).append(titleName).setNum(duration);
    QByteArray cryp = QCryptographicHash::hash(combinedString.toUtf8(), QCryptographicHash::Md5);
    QString uidstr = cryp.toHex();
    ////LOG_TRACE << "basicp uid is generated: " << uidstr << LOG_ENDL;
    return uidstr;
}
*/
// Added by pawan 2013.05.16.
};
#endif // IPODCONTROLLERGLOBAL_H
