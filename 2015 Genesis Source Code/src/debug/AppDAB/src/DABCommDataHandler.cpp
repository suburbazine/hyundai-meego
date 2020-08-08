#include "DABCommDataHandler.h"
#include "DABProtocol.h"
#include <fcntl.h>
#include <QDebug>

// for External Interrupt
#define DAB_INT_DEV_NAME	"/dev/dhavn-dmb"
#define DAB_INT_STR		"notify"
#define DAB_INT_STR_SIZE	sizeof(DAB_INT_STR)

quint8 m_spiDataBuf[DABCommDataHandler::SPI_PACKET_SIZE*DABCommDataHandler::ACC_COUNT];


DABCommDataHandler::DABCommDataHandler(DABCommSPI *comSpi, QObject *parent) :
    QThread(parent), pCommSPIPacketInfo(NULL)
{
    qWarning() << Q_FUNC_INFO << " : DABCommDataHander Creating";

//    PrepareLogFile(parent);
    m_spiDev = comSpi;
    stopped = false; 
    device = 0;         //# 20130327 prevent
    frameposition = 0;  //# 20130327 prevent
    continuity = 0;     //# 20130327 prevent

    // Interrupt Device Init...
    m_dabIntDev = open(DAB_INT_DEV_NAME, O_RDONLY);
    qWarning("dabIntDev = %d", m_dabIntDev);
    if(m_dabIntDev < 0) {
        qWarning("Can't open DMB interrupt device\n");
    }
}

DABCommDataHandler::~DABCommDataHandler()
{
    mutex.lock();
    qWarning() << Q_FUNC_INFO << "DABCommDataHandler  destruct!!"<< endl;
    m_spiDev->Close();
    close(m_dabIntDev);
    mutex.unlock();
}

void DABCommDataHandler::stop()
{
    mutex.lock();
    qWarning() << Q_FUNC_INFO << QThread::currentThreadId() << "Stopped !!"<< endl;
    stopped = true;
    mutex.unlock();
}


void
DABCommDataHandler::run()
{
    qint32 size;
    quint8 buf[DAB_INT_STR_SIZE] = {0,};
    quint8 rx_data[SPI_PACKET_SIZE];

    mutex.lock();
    //qWarning() << Q_FUNC_INFO << "==== SPI Thread Started.. !! ========";
    mutex.unlock();

    if(m_dabIntDev < 0)
        stop();

    while(!stopped)
    {
        size = read(m_dabIntDev, buf, DAB_INT_STR_SIZE);
        //qWarning("Read dab_int_dev = %d, read size = %d", m_dabIntDev, size);

        if(size >= 0)
        {
            size = m_spiDev->Read(rx_data, SPI_PACKET_SIZE);
            checkDataPacket(rx_data, SPI_PACKET_SIZE);
        }
        else
        {
            qWarning("COMSpi:: Interrupt Device read failed !! ");
        }

        usleep(10000);
    }

    stopped = false;
    //qWarning() << Q_FUNC_INFO << "==== SPI Thread Ended.. !! ========";
}

bool DABCommDataHandler::checkDataPacket(const quint8 *buffer, const int bufLen)
{
    Q_UNUSED(bufLen);
    //qWarning() << Q_FUNC_INFO << "=======================================SPI Packet Parsing START==================";
    //qWarning() << "Buffer Length: " << bufLen;

    quint16 *checksum;
    quint8 index = 0;
    quint8 symbol = 0;
    quint8 continuityIndexFirstLastDevice = 0;
    unsigned char continuityIndex = 0;
    unsigned char first = 0;
    unsigned char last = 0;
    unsigned char device = 0;
    unsigned char frameposition =0;
    quint8 dataLengthLow = 0;
    quint8 dataLengthHigh = 0;
    quint16 dataTotalLength = 0;
    quint8 commandValue = 0;
    int paddingCount = 0;

    //Start Symbol
    symbol = buffer[index++];
    //qWarning("Start Symbol     : 0x%02X", symbol);

    if(symbol != SPI_START_SYMBOL)
    {
        qWarning("Symbol value = %d . Start Symbol is incorrect. ", symbol);
        return false;
    }

    // Continuity
    continuityIndexFirstLastDevice = buffer[index++];
    continuityIndex = (continuityIndexFirstLastDevice >> 4 ) & 0x0F;
    //qWarning("Continuity index : 0x%02X", continuityIndex);
    debugPrintOutContinuity(continuity, continuityIndex);

    // First
    first = (continuityIndexFirstLastDevice & 0x0F) >> 3;
    //qWarning("First            : 0x%02X", first);

    // Last
    last = (continuityIndexFirstLastDevice >> 2) & 0x01;
    //qWarning("Last             : 0x%02X", last);

    //Frame Position
    frameposition = (continuityIndexFirstLastDevice & 0x0F) >> 2;
    //qWarning(" frameposition   : 0x%02X", frameposition);
    this->pCommSPIPacketInfo->setFrameposition(frameposition);
    debugPrintOutFramePosition(frameposition);

    // Device
    device = continuityIndexFirstLastDevice & 0x03;
    //qWarning("Device           : 0x%02X", device);
    this->pCommSPIPacketInfo->setDevice(device);
    debugPrintOutDevice(device);

    // DataLength High
    dataLengthHigh = buffer[index++];
    dataTotalLength = dataLengthHigh;

    // DataLength Low
    dataLengthLow = buffer[index++];
    dataTotalLength |= (dataLengthLow<<8);

    //Total Length
    //qWarning("Packet Length : 0x%04X(%d)", dataTotalLength, dataTotalLength);

    // CRC Check
    unsigned int cCheck = calculateCRC16(buffer, dataTotalLength+4);
    checksum = (quint16*)(buffer + dataTotalLength+4);
//    qWarning(" CheckSum: 0x%02X, cCheck:0x%02X", *checksum, cCheck);
    if(*checksum != cCheck)
    {
        qWarning("Checksum error !! checksum: 0x%02X,    Calculate checksum: 0x%02X", *checksum, cCheck);
//        return false;
    }

    //qWarning("Buffer Offset value = %d", index);
    quint8 m_spiDataBuf[dataTotalLength];
    memset(m_spiDataBuf, 0x00, dataTotalLength);
    memcpy(m_spiDataBuf, buffer+index, dataTotalLength);

    if(frameposition == SPI_INTERMEDIATE_PACKET)
    {
        commandValue = this->pCommSPIPacketInfo->getCommandValue();

        if(commandValue == CMD_EVENT_SPI_EPG)
        {
            quint8* packetBuffer = this->pCommSPIPacketInfo->getPacketBuffer();
            if(packetBuffer != NULL)
            {
                int lastIndex = this->pCommSPIPacketInfo->getPacketLastIndex();
                int totalPacketlength = this->pCommSPIPacketInfo->getTotalLength();
                //qWarning("lastIndex = %d, index = %d, dataLength = %d", lastIndex, index, dataTotalLength);
                int insertDataLength = lastIndex + dataTotalLength;
                //qWarning() << "=============CMD_EVENT_SPI_EPG============= totalPacketLength = " << totalPacketlength << ", insertDataLength = " << insertDataLength;
                if(totalPacketlength > insertDataLength)
                {
                    memcpy((packetBuffer+lastIndex), (const char*)(buffer+index), dataTotalLength);
                    this->pCommSPIPacketInfo->setPacketBuffer(packetBuffer);
                    this->pCommSPIPacketInfo->setPacketLastIndex(lastIndex+dataTotalLength);
                }
            }
            else
            {
                qWarning() << "======================== SPI_INTERMEDIATE_PACKET : CMD_EVENT_SPI_EPG : packetBuffer is NULL, so packet ignored !! ================";
            }
        }
#ifdef __DAB_TPEG__
        else if(commandValue == CMD_EVENT_SPI_TPEG)
        {
            quint8* packetBuffer = this->pCommSPIPacketInfo->getPacketBuffer();
            if(packetBuffer != NULL)
            {
                int lastIndex = this->pCommSPIPacketInfo->getPacketLastIndex();
                //qWarning("lastIndex = %d, index = %d, dataLength = %d", lastIndex, index, dataTotalLength);
                int totalPacketlength = this->pCommSPIPacketInfo->getTotalLength();
                int insertDataLength = lastIndex + dataTotalLength;
                //qWarning() << "=============CMD_EVENT_SPI_EPG============= totalPacketLength = " << totalPacketlength << ", insertDataLength = " << insertDataLength;
                if(totalPacketlength > insertDataLength)
                {
                    memcpy((packetBuffer+lastIndex), (const char*)(buffer+index), dataTotalLength);
                    this->pCommSPIPacketInfo->setPacketBuffer(packetBuffer);
                    this->pCommSPIPacketInfo->setPacketLastIndex(lastIndex+dataTotalLength);
                }
            }
            else
            {
                qWarning() << "======================== SPI_INTERMEDIATE_PACKET : CMD_EVENT_SPI_TPEG : packetBuffer is NULL, so packet ignored !! ================";
            }
        }
#endif
        else if(commandValue == CMD_EVENT_SPI_JPEG || commandValue == CMD_EVENT_SPI_PNG || commandValue == CMD_EVENT_SPI_STATION_LOGO)
        {
            //qWarning("index = %d, dataLength = %d", index, dataTotalLength);
            QByteArray bArray ((const char*)(buffer+index), dataTotalLength);
            //qWarning("intermediate packet of a series parsing success : emit receivedCommand(SPI EVENT, Command[%d], framePosition[%d], length[%d]", commandValue, frameposition, dataTotalLength);
            emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, commandValue, frameposition, bArray, dataTotalLength, this->pCommSPIPacketInfo->getTotalLength());
        }
    }
    else if(frameposition == SPI_LAST_PACKET)
    {
        commandValue = this->pCommSPIPacketInfo->getCommandValue();

        if(commandValue == CMD_EVENT_SPI_EPG)
        {            
            quint8* packetBuffer = this->pCommSPIPacketInfo->getPacketBuffer();
            if(packetBuffer != NULL)
            {
                int totalPacketlength = this->pCommSPIPacketInfo->getTotalLength();
                int lastIndex = this->pCommSPIPacketInfo->getPacketLastIndex();
                paddingCount = (4 - totalPacketlength%4)%4;
                //qWarning("lastIndex = %d, index = %d, dataLength = %d, paddingCount = %d", lastIndex, index, dataTotalLength, paddingCount);
                int insertDataLength = lastIndex + dataTotalLength - paddingCount;
                //qWarning() << "=============CMD_EVENT_SPI_EPG============= totalPacketLength = " << totalPacketlength << ", insertDataLength = " << insertDataLength;
                if(totalPacketlength == insertDataLength)
                {
                    memcpy((packetBuffer+lastIndex), (const char*)(buffer+index), dataTotalLength-paddingCount);
                    QByteArray bArray ((const char*)packetBuffer, totalPacketlength);
                    //qWarning("last packet of a series parsing success : emit receivedCommand(SPI EVENT, Command[%d], framePotion[%d], paddingCount[%d], totalLength[%d]", commandValue, frameposition, paddingCount, totalPacketlength);
                    emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, commandValue, frameposition, bArray, totalPacketlength/*totalPacketlength-paddingCount*//*dataTotalLength-paddingCount*/, totalPacketlength);
                }
                else
                {
                    qWarning() << " ==============CMD_EVENT_SPI_EPG========EPG Data Incorrect=========================";
                }
                free(packetBuffer);
                this->pCommSPIPacketInfo->washOffData();
            }
            else
            {
                qWarning() << "======================== SPI_LAST_PACKET : CMD_EVENT_SPI_EPG : packetBuffer is NULL, so packet ignored !! ================";
                this->pCommSPIPacketInfo->washOffData();
            }
        }
#ifdef __DAB_TPEG__
        else if(commandValue == CMD_EVENT_SPI_TPEG)
        {           
            quint8* packetBuffer = this->pCommSPIPacketInfo->getPacketBuffer();
            if(packetBuffer != NULL)
            {
                int totalPacketlength = this->pCommSPIPacketInfo->getTotalLength();
                int lastIndex = this->pCommSPIPacketInfo->getPacketLastIndex();
                paddingCount = (4 - totalPacketlength%4)%4;
                //qWarning("lastIndex = %d, index = %d, dataLength = %d, paddingCount = %d", lastIndex, index, dataTotalLength, paddingCount);
                int insertDataLength = lastIndex + dataTotalLength - paddingCount;
                //qWarning() << "=============CMD_EVENT_SPI_TPEG============= totalPacketLength = " << totalPacketlength << ", insertDataLength = " << insertDataLength;
                if(totalPacketlength == insertDataLength)
                {
                    memcpy((packetBuffer+lastIndex), (const char*)(buffer+index), dataTotalLength-paddingCount);
                    QByteArray bArray ((const char*)packetBuffer, totalPacketlength);
                    //qWarning("last packet of a series parsing success : emit receivedCommand(SPI EVENT, Command[%d], framePotion[%d], paddingCount[%d], totalLength[%d]", commandValue, frameposition, paddingCount, totalPacketlength);
                    emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, commandValue, frameposition, bArray, totalPacketlength/*totalPacketlength-paddingCount*//*dataTotalLength-paddingCount*/, totalPacketlength);
                }
                else
                {
                    qWarning() << " ==============CMD_EVENT_SPI_TPEG=======EPG Data Incorrect=========================";
                }
                free(packetBuffer);
                this->pCommSPIPacketInfo->washOffData();
            }
            else
            {
                qWarning() << "======================== SPI_LAST_PACKET : CMD_EVENT_SPI_TPEG : packetBuffer is NULL, so packet ignored !! ================";
                this->pCommSPIPacketInfo->washOffData();
            }
        }
#endif		
        else if(commandValue == CMD_EVENT_SPI_JPEG || commandValue == CMD_EVENT_SPI_PNG || commandValue == CMD_EVENT_SPI_STATION_LOGO)
        {
            //Padding Count
            paddingCount = (4 - this->pCommSPIPacketInfo->getTotalLength()%4)%4;
            //qWarning(" paddingCount : %d, index : %d, Buffer Length : %d", paddingCount, index, this->pCommSPIPacketInfo->getTotalLength());

            QByteArray bArray ((const char*)(buffer+index), dataTotalLength-paddingCount);
            //qWarning("last packet of a series parsing success : emit receivedCommand(SPI EVENT, Command[%d], framePostion[%d], length[%d]", commandValue, frameposition, dataTotalLength-paddingCount);
            emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, commandValue, frameposition, bArray, dataTotalLength-paddingCount, this->pCommSPIPacketInfo->getTotalLength());
            this->pCommSPIPacketInfo->washOffData();
        }
    }
    else // first packet or one packet
    {
        //qWarning("This Packet type is first packet or one packet.");
        parsingDataPacket(m_spiDataBuf, dataTotalLength, frameposition);
    }

    //qWarning() << Q_FUNC_INFO << "End... ";
    return true;
}

void
DABCommDataHandler::parsingDataPacket(quint8 *buffer, int bufLen, unsigned char frameposition)
{
    //qWarning() << Q_FUNC_INFO << "Started... ";

    int index = 0;
    quint8 byte;
    quint8 type;
    quint16 command;
    quint16 packetLength;
    quint16 paddingCount;

    //TYPE
    byte = buffer[index++];
    type = byte;
    //qWarning(" Packet Type: 0x%02X", type);

    //Command
    byte = buffer[index++];
    command = byte;
    byte = buffer[index++];
    command |= byte << 8;
    this->pCommSPIPacketInfo->setCommandValue(command);
    //qWarning(" Packet Command: 0x%04X", command);

    //Reserved
    byte = buffer[index++];
    //qWarning(" Packet Reserved: 0x%02X", byte);

    //Packet Length
    byte = buffer[index++];
    //qWarning(" Packet Length high : 0x%02X", byte);
    packetLength = byte;

    byte = buffer[index++];
    //qWarning(" Packet Length Low : 0x%02X", byte);
    packetLength |= (byte<<8);

    //qWarning(" Packet Length : 0x%04X(%d)", packetLength, packetLength);
    this->pCommSPIPacketInfo->setTotalLength(packetLength);

    //Padding Count
    paddingCount = (4 - packetLength%4)%4;
    //qWarning(" paddingCount : %d, index : %d, Buffer Length : %d", paddingCount, index, bufLen);

    if(frameposition == SPI_ONE_AND_ONLY_PACKET)
    {
        //qWarning("This Packet is only one packet !!");
        int packetLen = bufLen - index - paddingCount;
        if(packetLen != packetLength)
        {
            this->pCommSPIPacketInfo->washOffData();
            qWarning() << " =========SPI_ONE_AND_ONLY_PACKET=======Data Incorrect ============================";
            return;
        }

        QByteArray bArray ((const char*)(buffer+index), packetLen);
        //qWarning("One Packet parsing success : emit receivedCommand(SPI EVENT, Command[%d], framePostion[%d], length[%d]", command, frameposition, packetLen);
        emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, command, frameposition, bArray, packetLen, this->pCommSPIPacketInfo->getTotalLength());
        this->pCommSPIPacketInfo->washOffData();
    }
    else
    {
        if(command == CMD_EVENT_SPI_EPG || command == CMD_EVENT_SPI_TPEG)
        {
            if(packetLength < bufLen-index)
            {
                qWarning() << "======================== SPI_FIRST_PACKET : CMD_EVENT_SPI_TPEG or CMD_EVENT_SPI_EPG : packet ignored !! ================";
                this->pCommSPIPacketInfo->setTotalLength(0);
            }
            else
            {
                quint8* packetBuffer = NULL;
                packetBuffer = (quint8*)malloc(packetLength);
                memset(packetBuffer, 0x00, packetLength);
                //qWarning() << "==============>  EPG or TPEG malloc() packetLength = " << packetLength;
                memcpy(packetBuffer, (const char*)(buffer+index), bufLen-index);
                //qWarning() << "<==============  EPG or TPEG malloc() index = " << buffer+index << " length = " << bufLen-index;
                this->pCommSPIPacketInfo->setPacketBuffer(packetBuffer);
                this->pCommSPIPacketInfo->setPacketLastIndex(bufLen-index);
            }
        }
        else if(command == CMD_EVENT_SPI_JPEG || command == CMD_EVENT_SPI_PNG || command == CMD_EVENT_SPI_STATION_LOGO)
        {
            QByteArray bArray ((const char*)(buffer+index), bufLen-index);
            //qWarning("First Packet success : emit receivedCommand(SPI EVENT, Command[%d], framePostion[%d], length[%d]", command, frameposition, bufLen-index);
            emit receivedCommand(DAB_SPI_EVENT_COMMAND_TYPE, command, frameposition, bArray, bufLen-index, this->pCommSPIPacketInfo->getTotalLength());
        }
    }
}

void
DABCommDataHandler::debugPrintOutContinuity(unsigned char oldConti, unsigned char newConti)
{
    Q_UNUSED(oldConti);
    Q_UNUSED(newConti);
//    if((oldConti == (newConti - 1)) || (oldConti == 0x0F && newConti == 0)){
//        qWarning("     == continuous frame");
//    }else if( oldConti == 0 && newConti == 0)
//    {
//        qWarning("     == NO continuous frame");
//    }else{
//        qWarning("     == Unknown continuous value!!!!");
//    }
}

void
DABCommDataHandler::debugPrintOutFramePosition(unsigned char pos)
{
    Q_UNUSED(pos);
//    switch(pos)
//    {
//    case 0: qWarning("     == intermediate Packet of a series.");
//        break;
//    case 1: qWarning("     == last packet of a series.");
//        break;
//    case 2: qWarning("     == first packet of a series.");
//        break;
//    case 3: qWarning("     == the one and only packet.");
//        break;
//    default: qWarning("    === Continuity Unknown Error..!!!!");
//        break;
//    }
}

void
DABCommDataHandler::debugPrintOutDevice(unsigned char device)
{
    Q_UNUSED(device);
//    switch(device)
//    {
//    case 0: qWarning("     == device UART 0 "); break;
//    case 1: qWarning("     == device UART 1 "); break;
//    case 2: qWarning("     == device SPI 0 "); break;
//    case 3: qWarning("     == device SPI 1 "); break;
//    default: qWarning("     == device Unknown "); break;
//    }
}

void
DABCommDataHandler::setCommDataHandler(DABCommSPIPacketInfo* comPacketInfo)
{
     pCommSPIPacketInfo = comPacketInfo;
}
