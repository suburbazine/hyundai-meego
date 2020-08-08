#include <QDebug>
#include "DABCommReceiverThread.h"
#include "DABProtocol.h"

/****************************/
/* CRC SIZE */
/****************************/
#define CRC_TABLE_SIZE 256

//************************************************************************
// Lookup table to get the 16 bit sums for the CRC calculation
// X^16 + X^12 + X^5 + 1
// (compile as external const table)
const unsigned short crc16_table[CRC_TABLE_SIZE] =
{
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
        0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
        0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
        0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
        0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
        0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
        0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
        0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
        0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
        0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
        0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
        0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
        0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
        0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
        0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
        0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
        0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
        0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
        0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
        0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
        0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
        0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
        0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};

unsigned short	calculateCRC16(const unsigned char* pIData, unsigned int  IDataLen)
{
        unsigned int  bTemp, ICounter;
        unsigned short ICrc_Accum;

        ICrc_Accum = 0;
        for (ICounter=0; ICounter<IDataLen; ICounter++)
        {
                bTemp = ((ICrc_Accum>>8)^pIData[ICounter])&0xFF;
                ICrc_Accum = crc16_table[bTemp]^(ICrc_Accum<<8);
        }

        return ICrc_Accum;
}

DABCommReceiverThread::DABCommReceiverThread(DABCommUART *comUart, QObject *parent) :
    QThread(parent)
{
    qWarning() << Q_FUNC_INFO << " DABCommReceiverThread start!! ";
    //start();

    DABModule = comUart;
    stopped = false;
    state = ProcessStartByte;

    packetBuffer = NULL;
    packetLastIndex = 0;
    commandValue = 0;
    totalLength = 0;
    memset(dataBuffer, 0x00, DataBufferSize);
    memset(readBuffer, 0x00, ReadBufferSize);
    packetLength = 0;
    checksum = 0;
    command = 0;
    byte = 0;
    type = 0;
    device = 0;
    frameposition = 0;
    continuity = 0;
    packetState = ProcessPacketType;
    dataLen = 0;
    dataBufOffset = 0;
}

DABCommReceiverThread::~DABCommReceiverThread()
{
    //wait();
    this->DABModule->Close();
}

void DABCommReceiverThread::run(void)
{
    qWarning() << Q_FUNC_INFO << QThread::currentThreadId();
    int res = 0;
    int debugcount = 0;

    while(!stopped)
    {
        if((debugcount++ % 10000) == 0) qWarning("  ===========>run...");///printf(".");

        res = this->DABModule->Read(readBuffer, ReadBufferSize /*ReadBufferSize*/);
        if(res > 0)
        {
            parsingCommand(readBuffer, res);
            //qWarning() << "parsingCommand end..";
        }

        usleep(SleepTime);

    }
    stopped = false;
    qWarning() << Q_FUNC_INFO << QThread::currentThreadId() << " == END ==";
}

void DABCommReceiverThread::stop()
{
    qWarning() << Q_FUNC_INFO << QThread::currentThreadId();

    mutex.lock();
    stopped = true;
    mutex.unlock();
}
void DABCommReceiverThread::debugPrintOutContinuity(unsigned char oldConti, unsigned char newConti)
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

void DABCommReceiverThread::debugPrintOutFramePosition(unsigned char pos)
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
void DABCommReceiverThread::debugPrintOutDevice(unsigned char device)
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


bool
DABCommReceiverThread::parsingPacketSeries(unsigned char *buffer, int bufLen)
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
    qDebug(" Packet Type: 0x%02X", type);

    //Command
    byte = buffer[index++];
    command = byte;
    byte = buffer[index++];
    command |= byte << 8;
    this->setCommandValue(command);
    qDebug(" Packet Command: 0x%04X", command);

    //Reserved
    byte = buffer[index++];
    qDebug(" Packet Reserved: 0x%02X", byte);

    //Packet Length
    byte = buffer[index++];
    qDebug(" Packet Length high : 0x%02X", byte);
    packetLength = byte;

    byte = buffer[index++];
    qDebug(" Packet Length Low : 0x%02X", byte);
    packetLength |= (byte<<8);

    qCritical(" Packet Length : 0x%04X(%d)", packetLength, packetLength);
    this->setTotalLength(packetLength);

    //Padding Count
    paddingCount = (4 - packetLength%4)%4;
    qDebug(" paddingCount : %d, index : %d, Buffer Length : %d", paddingCount, index, bufLen);

    quint8* packetBuffer = NULL;
    packetBuffer = (quint8*)malloc(packetLength);
    memset(packetBuffer, 0x00, packetLength);
    memcpy(packetBuffer, (const char*)(buffer+index), bufLen-index);
    this->setPacketBuffer(packetBuffer);
    this->setPacketLastIndex(bufLen-index);    
    return true;
}

bool DABCommReceiverThread::parsingPacket(unsigned char *buffer, int bufLen)
{
    //qWarning() << Q_FUNC_INFO;
    //qWarning() << "== ParsingPacket start!! Buffer Length : " << bufLen;

    int i = 0;

    //return true;
    //int frameStartPoint = 0;
    quint8 byte;
    while(i < bufLen)
    {
        switch(packetState)
        {
            case ProcessPacketType:
            {
                byte = buffer[i++];
                type = byte;
                //qWarning(" Packet Type: 0x%02X", type);
                packetState = ProcessPacketCommand;
                break;
            }
            case ProcessPacketCommand:
            {
                byte = buffer[i++];
                command = byte;
                byte = buffer[i++];
                command |= byte << 8;
                //Reserve Byte Check
                byte = buffer[i++];
                // ignore reserved byte
                //qWarning(" Packet Command: 0x%04X", command);
                //qWarning(" Packet Reserved: 0x%02X", byte);
                packetState = ProcessPacketLength;
                break;
            }
            case ProcessPacketLength:
            {
                byte = buffer[i++];
                //qWarning(" Packet Length high : 0x%02X", byte);
                packetLength = byte;

                byte = buffer[i++];
                //qWarning(" Packet Length Low : 0x%02X", byte);
                packetLength |= (byte<<8);

                //qWarning(" Packet Length : 0x%04X", packetLength);

                if(packetLength > 0)
                    packetState = ProcessPacketData;
                else{
                    //qWarning(" Frame Parsing success ==> Type: 0x%02X, Command: 0x%02X, Data Length: %d", type, command, packetLength);
                    emit receivedCommand(type, command, NULL, packetLength);
                    packetState = ProcessPacketSuccess;
                }
                break;
            }
            case ProcessPacketData:
            {
                // Copy data from readBuffer until Data length or readBuffer length...

                //qWarning(" bufLen: 0x%02X(%d), i:%d, Packet Length:%d, dataLen:%d ", bufLen, bufLen, i, packetLength, dataLen);
                unsigned int paddingCount = (4 - packetLength%4)%4;
                //qWarning(" Padding Count : 0x%02X", paddingCount);
                if((i+packetLength + paddingCount) == bufLen)
                {
                    //qWarning() << " BufferLen: " << bufLen << ", i+packetLength + paddingCount : "<< (i+packetLength + paddingCount) << " : " << ((i+packetLength + paddingCount)==(bufLen));
                    //qWarning(" Packet Parsing success ==> Type: 0x%02X, Command: 0x%02X, Data Length: %d", type, command, packetLength);
                    QByteArray bArray ((const char*)(buffer+i), packetLength);
                    emit receivedCommand(type, command, bArray, packetLength);
                    usleep(10000);
                    packetState = ProcessPacketSuccess;
                    //i += (packetLength + paddingCount);
                }else{
                    qWarning() << " Error BufferLen: " << bufLen << ", i+packetLength + paddingCount: "<< (i+packetLength + paddingCount) << " : " << ((i+packetLength + paddingCount)==(bufLen));
                    // parsing failure
//                    for(int j = 0; j < i; j++)
//                    {
//                        //qWarning("0x%02X, ", buffer[j]);
//                    }
                }
                i += (packetLength + paddingCount); //exit while loop
                break;
            }
            default:
                qWarning() << " ERROR! Unknown parsingState: " << packetState;
                break;
        }
    }
    if(packetState == ProcessPacketSuccess)
    {
        return true;
    }
    return false;
}

bool DABCommReceiverThread::parsingCommand(unsigned char *buffer, int bufLen)
{
    //qWarning() << Q_FUNC_INFO;
    //qWarning() << "== ParsingCommand start!! Buffer Length : " << bufLen;
    int i = 0;
    quint8 byte;
    while(i < bufLen)
    {
        switch(state)
        {
            case ProcessStartByte:
            {
                //frameStartPoint = i;
                byte = buffer[i++];
                if(byte == 0xA5 /*UART_CHANNEL_START_SYMBOL*/)
                {
                    //qWarning(" Start Symbol: 0x%02X", byte);
                    // initializing dataBuffer, checksum...
                    dataLen = 0;
                    continuity = 0;

                    state = ProcessContinuityIndexFirstLastDevice;
                    dataBufOffset = 0;
                    dataBuffer[dataBufOffset++] = byte;
                }
                else
                {
                    qDebug() << "Frame Parsing Error, Unknown Start Symbol ==>" << hex << byte;
                }

                break;
            }
            case ProcessContinuityIndexFirstLastDevice:
            {
                // Length: Command + Data, Length = (UINT16)(Len(H) << 8 | Len(L))
                // Continuity Index: Upper 4bit
                byte = buffer[i++];
                //qWarning(" byte: 0x%02X", byte);
                unsigned char cont = (byte >> 4 ) & 0x0F;
                //qWarning(" Continuity: 0x%02X", cont);
                debugPrintOutContinuity(continuity, cont);
                continuity = cont;

//                unsigned char first = (byte & 0x0F) >> 3;
                //qWarning(" First : 0x%02X", first);
//                unsigned char last = (byte >> 2) & 0x01;
                //qWarning(" Last : 0x%02X", last);
                frameposition = (byte & 0x0F)>>2;
                //qWarning(" frameposition : 0x%02X", frameposition);
                debugPrintOutFramePosition(frameposition);

                device = (byte & 0x03);
                //qWarning(" Device: 0x%02X", device);
                debugPrintOutDevice(device);

                state = ProcessDataLengthHigh;

                dataBuffer[dataBufOffset++] = byte;
                break;
            }
            case ProcessDataLengthHigh:
            {
                byte = buffer[i++];
                dataLen = byte;
                state = ProcessDataLengthLow;

                dataBuffer[dataBufOffset++] = byte;
                break;
            }
            case ProcessDataLengthLow:
            {
                byte = buffer[i++];
                dataLen |= byte<<8;

                state = ProcessData;

                dataBuffer[dataBufOffset++] = byte;
                //qWarning(" Data Length: 0x%02X", dataLen);
                break;
            }
            case ProcessData:
            {
                while((dataBufOffset-4) < dataLen && i < bufLen)
                {
                    byte = buffer[i++];
                    dataBuffer[dataBufOffset++] = byte;
                    //checksum += byte;

                    //qWarning("0x%02X, ", byte);
                }
//                if( i == bufLen){
//                    qWarning("  == Input Buffer is seperated. so stop parsing and wait next Frame bufOffset(%d), dataLen(%d,0x%04X)", dataBufOffset, dataLen, dataLen);
//                }
                if((dataBufOffset-4) == dataLen ) // Frame Parsing Completed. Check the CRC. // 4 is Frame Header size..
                {
                    state = ProcessCheckCRCHigh;
                }
                //else.. there is more data received.. so stay in this state.... In next parsingCommand, It will continue parsing..
                break;
            }

            case ProcessCheckCRCHigh:
            {
                byte = buffer[i++];
                checksum = byte;

                state = ProcessCheckCRCLow;
                break;
            }
            case ProcessCheckCRCLow:
            {
                byte = buffer[i++];
                checksum |= byte << 8;
                unsigned int cCheck = calculateCRC16(dataBuffer, dataBufOffset);
                //qWarning(" CheckSum: 0x%04X, cCheck:0x%04X", checksum, cCheck);

                if(checksum == cCheck)
                {
                    //qWarning(" Frame Parsing success =");

                    //emit receivedCommand(type, command, dataBuffer, packetLength);
                    if(this->frameposition == 0x03) // the one and only packet
                    {
                        packetState = ProcessPacketType;
                        if( this->parsingPacket(dataBuffer+4, dataLen) == false){ // 4 is Frame header size;
                            qDebug("  Parsing Packet Error!..");
                            state = ProcessStartByte; //Parsing is done. but Not Success.
                        }
                        else{
                            state = ProcessFrameSuccess; //Parsing is done. Success.
                        }
                    }
                    else if(this->frameposition == 0x00)    // intermdiate Packet of a series
                    {                        
                        qDebug() << " This packet is intermdiate !!";
                        quint8* packetBuffer = this->getPacketBuffer();
                        int lastIndex = this->getPacketLastIndex();
                        //qWarning("lastIndex = %d, index = %d, dataLength = %d", lastIndex, i, dataLen);
                        memcpy((packetBuffer+lastIndex), (const char*)(dataBuffer+4), dataLen);
                        this->setPacketBuffer(packetBuffer);
                        this->setPacketLastIndex(lastIndex + dataLen);
                        state = ProcessFrameSuccess; //Parsing is done. Success.
                    }
                    else if(this->frameposition == 0x01)    // last packet of a series
                    {
                        qDebug() << " This packet is Last!!";
                        quint8* packetBuffer = this->getPacketBuffer();
                        int totalPacketlength = this->getTotalLength();
                        int lastIndex = this->getPacketLastIndex();

//                        qDebug("lastIndex = %d, index = %d, dataLength = %d", lastIndex, i, dataLen);

                        int paddingCount = (4 - totalPacketlength%4)%4;

                        qDebug("lastIndex = %d, index = %d, dataLength = %d, paddingCount = %d", lastIndex, i, dataLen, paddingCount);

                        memcpy((packetBuffer+lastIndex), (const char*)(dataBuffer+4), dataLen-paddingCount);

                        QByteArray bArray ((const char*)packetBuffer, totalPacketlength);

                        qDebug("last packet of a series parsing success : emit receivedCommand(DAB_RESPONE, Command[%d], totalLength[%d]", commandValue, totalPacketlength);
                        emit receivedCommand(DAB_RESPONSE_COMMAND_TYPE, commandValue, bArray, totalPacketlength/*totalPacketlength-paddingCount*//*dataTotalLength-paddingCount*/);
                        qDebug() << " ==============before===========Free(packetBuffer) call !!";
                        free(packetBuffer);
                        qDebug() << " ==============after===========Free(packetBuffer) call !!";
                        this->setCommandValue(0x00);
                        this->setPacketBuffer(NULL);
                        this->setTotalLength(0x00);
                        this->setPacketLastIndex(0);                        
                        state = ProcessFrameSuccess; //Parsing is done. Success.
                    }
                    else if(this->frameposition == 0x02)    // first packet of a series
                    {
                        qDebug() << " This packet is Frist!!";
                        this->parsingPacketSeries(dataBuffer+4, dataLen);
                        state = ProcessFrameSuccess; //Parsing is done. Success.
                    }
                }
                else
                {
                    // transmission error...
                    // To do
                    qDebug() << Q_FUNC_INFO << "  Frame Checksum Error!!!!!!!!!! Checksum: " << hex << checksum << "<->" << cCheck;
                    qDebug() << Q_FUNC_INFO << "  command = " << commandValue;
                    for(int j = 0; j < i; j++)
                    {
                        qDebug("0x%02X, ", buffer[j]);
                    }
                   state = ProcessStartByte; //Parsing is done. but Not Success.
                }

                break;
            }
            case ProcessFrameSuccess:
                // A Frame Parsing is success. but there are more data.
                state = ProcessStartByte;
                break;
            default:
                qDebug() << Q_FUNC_INFO << " == Parsing State Error!! Unknown State: " << state ;
                qDebug() << " i:"<<i<<", bufLen:"<<bufLen;
            break;
        }
    } // while( i < bufLen)

    if( state == ProcessFrameSuccess){
        state = ProcessStartByte; //Success. reset state to Starting point.
        return true;
    }
    return false;
}



