
#include "ahaCommonUtility.h"

void printBinaryByteArray(QByteArray array)
{
    int nArraySize = array.size();
    int i;
    for(i=0; i<nArraySize ; i++)
    {
        //extract one byte and convert it to hexa
        DEBUG_COMMON_UTIL("%x ", (unsigned char) (array.data())[i]);
    }
    DEBUG_COMMON_UTIL("\n nArraySize: %d\n", nArraySize);
}

uint16_t generateUniqueQueryID(bool bGenerateNextID)
{
    static uint16_t nQueryID = 0;
    if(bGenerateNextID == true)
        nQueryID++;
    if(nQueryID == AHA_TRANSACTION_ID_LIMIT)
        nQueryID = 0;
    return nQueryID;
}

//Intels are little endian. I should be able to just copy it.
uint16_t byteToUint16_t(QByteArray &byteArray, int nIndex)
{
    uint16_t i;
    uint16_t num;
    uint8_t* byte = (uint8_t*)&num;
    for(i = 0; i<2;i++)
        byte[i] = byteArray[nIndex+i];
    return num;
}

uint32_t byteToUint32_t(QByteArray &byteArray, int nIndex)
{
    uint16_t i;
    uint32_t num;
    uint8_t* byte = (uint8_t*)&num;
    for(i = 0; i<4;i++)
        byte[i] = byteArray[nIndex+i];
    return num;
}

double byteToDouble(QByteArray &byteArray, int nIndex)
{
    uint16_t i;
    double db_num;
    uint8_t* byte = (uint8_t*)&db_num;
    for(i = 0; i<8;i++)
        byte[i] = byteArray[nIndex+i];
    return db_num;
}

int64_t byteToInt64_t(QByteArray &byteArray, int nIndex)
{
    uint16_t i;
    int64_t n64_num;
    uint8_t* byte = (uint8_t*)&n64_num;
    for(i = 0; i<8;i++)
        byte[i] = byteArray[nIndex+i];
    return n64_num;
}

uint64_t byteToUint64_t(QByteArray &byteArray, int nIndex)
{
    uint16_t i;
    uint64_t n64_num;
    uint8_t* byte = (uint8_t*)&n64_num;
    for(i = 0; i<8;i++)
        byte[i] = byteArray[nIndex+i];
    return n64_num;
}

uint16_t calculateCheckSum(QByteArray &pInData)
{
    int i = 0;
    int nArraySize = pInData.size();

    uint16_t num = 0;
    uint16_t checksum = 0;

    uint8_t LSB_Byte;

    DEBUG_COMMON_UTIL("Calculating checksum\n");
    for (i=0; i < nArraySize -1 ; i=i+2)
    {
        //convert byte to two byte number
        if(i != 8) //8 and 9 is checksum field, should not be calculated.
        {
            num = byteToUint16_t(pInData,i);
            DEBUG_COMMON_UTIL("%d ", num);
            checksum += num;
        }
    }
    //0 padding for the last byte if it pInData is odd octet
    if(nArraySize%2 !=0) //if odd number
    {
        LSB_Byte = (uint8_t) pInData[nArraySize-1];
        num = LSB_Byte;
        DEBUG_COMMON_UTIL("\nlast byte %d\n", num);
        checksum += num;
    }

    checksum = -checksum;

    DEBUG_COMMON_UTIL("\nChecksum: %d\n", checksum);
    return checksum;
}

QString byteToQString(QByteArray &byteArray, int nIndex, int nByteLength)
{
    //this function converts the uint8_t[*] to QString. it assume that the incoming bytes are in UTF8.
    QString str;
    QByteArray tempByteArray;
    tempByteArray = byteArray.mid(nIndex, nByteLength);
    str = QString::fromUtf8(tempByteArray.data(), tempByteArray.size());
    return str;
}

bool doubleIsZero(double dbValue)
{
    if(dbValue < 0.0001 && dbValue > -0.0001)
    {
        return true;
    }
    else
    {
        return false;
    }
}
