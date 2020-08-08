/*
    File:       Transport.cpp
    Package:    LG iAP2 Service
    Author:     Jihwan Cho (jihwan.cho@lge.com)

    Copyright ¨Ï [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (¡°Software¡±) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (¡°Authorized License¡±). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <stdlib.h>

#include "Transport.h"
#ifdef MIBGP
#include "USBEventMonitor.h"
#else
#include "UsbDeviceMonitor.h"
#endif
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "ByteArray.h"
#include "iAP2Packet.h"

//#include "LinkController.h"

namespace IAP2
{

static void* recieverThreadFunc(void* data)
{
	Transport* recvThread = (Transport*) data;
	recvThread->recieverThread(data);
	return NULL;
}
int cond_flag = 0;

Transport::Transport(string devPath, AccUsbMode accUSbMode, void* pUserData,  struct UsbConnectCallBack* pCb) : mTid(0), mIsTerminated(false), mbThreadCreated(false), \
				mpRcvPacket(NULL), mbRcbPacketIsComplete(true), nbeforeLength(0), mbSendIap2Sequene(false), mAccUSbMode(accUSbMode)
{
	//LOGD("Transport() devPath[%s] accUSbMode[%d] mThread[%p]", devPath.c_str(), accUSbMode, mThread);
	mpConnCbs = pCb;
	//pthread_mutex_init(&mMutexRecvThSync, NULL);
	//pthread_cond_init(&mCondRecvThSync, NULL);
	cond_flag = 0;

	mDevicePath.assign(devPath);
	if (mDevicePath.empty()) {
		LOGE("ERROR : mDevicePath is empty");
		return;
	}

	mReaderFd = open(mDevicePath.c_str(), O_RDONLY/*|O_NONBLOCK*/);
	if (mReaderFd < 0) {
		//LOGE("ERROR : mReaderFd is smaller than 0, error[%d][%s]", errno, strerror(errno));
        LOGE("ERROR : mReaderFd open Failed %d", mReaderFd);
		return;
	}
	
	mWriterFD = open(mDevicePath.c_str(), O_WRONLY/*|O_NONBLOCK*/);
	if (mWriterFD < 0) {
		//LOGE("ERROR : mWriterFdis smaller than 0, error[%d][%s]", errno, strerror(errno));
        LOGE("ERROR : mWriterFD open Failed %d", mWriterFD);
		return;
	}

	if (accUSbMode == LINKCTRL_AUB_DEVICE) {
		struct termios tty;

		if(tcgetattr(mReaderFd, &tty) == -1) {
			LOGE("	ERROR : call tcgetattr() error[%d][%s]", errno, strerror(errno));
		}
		
		// backup tty, make it raw and apply changes
		mOrigReadTty = tty;
		cfmakeraw(&tty);
		if(tcsetattr(mReaderFd, TCSAFLUSH, &tty) < 0) {
			LOGE("	ERROR : call tcsetattr() error[%d][%s]", errno, strerror(errno));
		}

		//end of program or error path :
		//tcsetattr(mReaderFd, TCSAFLUSH, &orig_tty)


		if(tcgetattr(mWriterFD, &tty) == -1){
			LOGE("	ERROR : call tcgetattr() error[%d][%s]", errno, strerror(errno));
		}
		// backup tty, make it raw and apply changes
		mOrigWriteTty = tty;
		cfmakeraw(&tty);
		if(tcsetattr(mWriterFD, TCSAFLUSH, &tty) < 0) {
			LOGE("	ERROR : call tcsetattr() error[%d][%s]", errno, strerror(errno));
		}

		//end of program or error path :
		//tcsetattr(mWriterFD, TCSAFLUSH, &orig_tty)
	}

	mpUserData = pUserData;

	LOGI("	mReaderFd[%d] mWriterFD[%d] mpUserData[%p]", mReaderFd, mWriterFD, mpUserData);
	if (pthread_create(&mThread, NULL, recieverThreadFunc, (void*)this) == 0) {
		mbThreadCreated = true;
	}
	else {
		LOGE("	ERROR : fail to create thread");
	}

	//pthread_mutex_lock(&mMutexRecvThSync);
	//pthread_cond_wait(&mCondRecvThSync, &mMutexRecvThSync);
	//pthread_mutex_unlock(&mMutexRecvThSync);
	
	while (cond_flag != 1) {
		LOGD("Transport() waits for Thread Creation");
		usleep(100*1000);
	}
	LOGD("Transport() end");
}

Transport::~Transport()
{
	int retVal = 0;
	void* pThreadResult = NULL;
	
	LOGD("~Transport() mReaderFd[%d] mWriterFD[%d] mThread[%p] mbThreadCreated[%d]", mReaderFd, mWriterFD, mThread, mbThreadCreated);

	mIsTerminated = true;

	if (mbThreadCreated == true) {
		retVal = pthread_join(mThread, &pThreadResult);
		if (retVal != 0)
			LOGW("	WARNING : Thread join failed	error[%d][%s]", errno, strerror(errno));

		mbThreadCreated = false;
		LOGV("	thread joined");
	}
#if 0
	if (mAccUSbMode == LINKCTRL_AUB_DEVICE) {
		if(tcsetattr(mReaderFd, TCSAFLUSH, &mOrigReadTty) < 0) {
			LOGE("	ERROR : call tcsetattr() error[%d][%s]", errno, strerror(errno));
		}

		if(tcsetattr(mWriterFD, TCSAFLUSH, &mOrigWriteTty) < 0) {
			LOGE("	ERROR : call tcsetattr() error[%d][%s]", errno, strerror(errno));
		}
	}
#endif

	if (mReaderFd != -1) {
		retVal = close(mReaderFd);
		if (retVal == -1)
			LOGE("	ERROR : fail to close mReaderFd[%d] error[%d][%s]", mReaderFd, errno, strerror(errno));
	}

// Sometimes below code takes a lot of time about 5Sec.
// Actually this routine is not needed, because iAP2 gagdet driver is clean up right after.	
#if 1
	if (mWriterFD != -1) {
		LOGV("	before close mWriterFD");
		retVal = close(mWriterFD);
		if (retVal == -1)
			LOGE("	ERROR : fail to close mWriterFD[%d] error[%d][%s]", mWriterFD, errno, strerror(errno));
	}
#endif

	//pthread_cond_destroy(&mCondRecvThSync);
	//pthread_mutex_destroy(&mMutexRecvThSync);
	cond_flag = 0;

	LOGD("~Transport() end");
}

void* Transport::recieverThread(void* data)
{
	fd_set fds;
	struct timeval tv;
	int ret;

	//ByteArray rcvPacket;
	ssize_t nReadByte = 0;
	unsigned char* pRcvBuf = new unsigned char[MAX_RCV_PACKET_SIZE];

	mTid = getTid();
	(void) data;
	
	LOGD("recieverThread() created!!! mTid[%d]", mTid);

	//pthread_mutex_lock(&mMutexRecvThSync);
	//pthread_cond_signal(&mCondRecvThSync);
	//pthread_mutex_unlock(&mMutexRecvThSync);
	cond_flag = 1;

	if (pRcvBuf == NULL) {
		LOGE("	ERROR : pRcvBuf is NULL");
		return NULL;
	}
	
	while (!mIsTerminated) {
		FD_ZERO(&fds);
		FD_SET(mReaderFd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 100*1000; // 100 msec

		ret = select(mReaderFd+1, &fds, NULL, NULL, &tv);

	 	if(ret < 0)
			LOGE("	Transport Select ret : %d", ret);

		
		if (ret > 0 && FD_ISSET(mReaderFd, &fds)) {

			nReadByte = read(mReaderFd, pRcvBuf, MAX_RCV_PACKET_SIZE);
			//LOGD("== Recieved packet nReadByte[%d]", nReadByte);

			if (nReadByte > 0) {
				//printDataByHexa((char*)"$$$ recv Data", pRcvBuf, nReadByte);
				if (mbSendIap2Sequene == true) {
					unsigned char iAp2Sequence[] = {0xFF, 0x55, 0x02, 0x00, 0xEE, 0x10};
			
					iAP2LinkRunLoop_t* pIap2RunLoop = (iAP2LinkRunLoop_t*)mpUserData;

					if (pIap2RunLoop != NULL) {
						struct iAP2Link_st* link = pIap2RunLoop->link;
						if (link && link->context) {
							//LinkController* pInstance = (LinkController*)((iAP2LinkRunLoop_t*)(link->context))->context;
							bool bIap2Support;
							if (mAccUSbMode == LINKCTRL_AUB_HOST)
								bIap2Support = !memcmp(iAp2Sequence, pRcvBuf+2, sizeof(iAp2Sequence));
							else
								bIap2Support =	!memcmp(iAp2Sequence, pRcvBuf, sizeof(iAp2Sequence));

							if (bIap2Support) {
								LOGD("	iAP2 is supported");
								//pInstance->iap2SupportCb(CIS_IAP2_SURPORTED);
								mpConnCbs->iap2SupportCb(mpConnCbs->pData, CIS_IAP2_SURPORTED);
							}
							else {
								LOGD("	iAP1 is supported");
								//pInstance->iap2SupportCb(CIS_IAP1_SURPORTED);
								mpConnCbs->iap2SupportCb(mpConnCbs->pData, CIS_IAP1_SURPORTED);
							}
						}
						else {
							LOGE("	ERROR: link[%p] or link->context is NULL", link);						
						}
					}
					else {
						LOGE("	ERROR: pIap2RunLoop is NULL");
					}

					mbSendIap2Sequene = false;
				}

				if (mAccUSbMode == LINKCTRL_AUB_HOST) {
					assembleRcvPacket(pRcvBuf, nReadByte);
				}
				else {
					processInBoundTraffic(&pRcvBuf[0], nReadByte);
				}
			}
			else if (nReadByte < 0)
				LOGE("	Transport nReadByte %d", nReadByte);
		}	
			
	}
	
	delete[] pRcvBuf;
	LOGD("recieverThread() terminated!!! mTid[%d]", mTid);

	return NULL;
}

int Transport::sendDataToDevice(ByteArray& rSendPacket, bool bIap2Sequene)
{
	if (bIap2Sequene == true)
		mbSendIap2Sequene = true;
		
	if (mAccUSbMode == LINKCTRL_AUB_HOST)
		return sendHidData(rSendPacket);
	else
		return sendBulkData(rSendPacket);
}

static const int reports[][2] = {
// jungyeon.kim
// report id, report size(Hex), device report id, report size(Decimal)
{ 0x0d, 0x05 }, // 0		5
{ 0x0e, 0x09 }, // 1		9
{ 0x0f, 0x0d }, // 2		13
{ 0x10, 0x11 }, // 3		17
{ 0x11, 0x19 }, // 4		25
{ 0x12, 0x31 }, // 5		49
{ 0x13, 0x5f }, // 6		95
{ 0x14, 0xc1 }, // 7		193
{ 0x15, 0xff }, // 8		255
{ 0x00, 0x00 }
};

int lookupReport(int size, int& size_out)
{
	int i;
	for (i=0; reports[i][0]!=0; i++) {
		if (reports[i][1] >= size || reports[i+1][1] == 0x00) {
			size_out = reports[i][1];
			return reports[i][0];
		}
	}
	size_out  = reports[i][1];
	return reports[i][0];
}

int Transport::sendHidData(ByteArray& rSendPacket)
{
	ssize_t nWriteByte = 0;
	std::vector<int> reportSizeList;
	std::vector<int> reportIdList;

	//LOGD("sendHidData() size[%d]", rSendPacket.length());

	int count = 0;
	ByteArray tempArray;

	if (mWriterFD == -1) {
		LOGE("	ERROR : mWriterFD is -1");
		return -1;
	}
		
	while(count < rSendPacket.count()) {
		int size=0;
		tempArray.resize(0);
		tempArray = rSendPacket.mid(count);
		
		int reportId = lookupReport(tempArray.count()+2, size);
		reportIdList.push_back(reportId);
		reportSizeList.push_back(size);
		count += size;
		//LOGD("	finish to calc reportId[%d] size[%d] count[%d]", reportId, size, count);
	}

	int elementCount=0;// = reportIdList.count();
	int position=0;
	int repsize = 0;
	int repId = 0;
	ByteArray sendPacket;
	char* pPacketData = NULL;
	int vectorSize = 0;
	int i =0;

	vectorSize = reportSizeList.size();

	for (i=0; i<vectorSize; i++) {
		repId = reportIdList.at(i);
		repsize = reportSizeList.at(i);

		sendPacket.resize(2);
		pPacketData = sendPacket.data();
		
		pPacketData[0] = (char)repId;
		if(reportIdList.size() ==1)
			pPacketData[1] = 0x00;
		else if(elementCount == (int)(reportIdList.size()-1))
			pPacketData[1] = 0x01;
		else if(elementCount==0 && reportIdList.size() >1)
			pPacketData[1] = 0x02;
		else
			pPacketData[1] = 0x03;

		tempArray.resize(0);

		tempArray = rSendPacket.mid(position, repsize-2);
		sendPacket.append(tempArray);

		//printDataByHexa((char*)"& &&&=== send", (unsigned char*)sendPacket.constData(), sendPacket.length());
		nWriteByte = write(mWriterFD, sendPacket.constData(), sendPacket.length());
		LOGD("Transport write!![%p], dataLen : [%d], nWriteByte : [%d]", sendPacket.constData(), sendPacket.length(), nWriteByte);
		if (nWriteByte == -1)
			LOGE("	ERROR : fail to write mWriterFD[%d] error[%d][%s]", mWriterFD, errno, strerror(errno));
#if 0 // for test	
		else
			LOGD("** send packet nWriteByte[%d]", nWriteByte);
#endif

		elementCount++;
		position +=repsize-2;
	}

	return 0;

#if 0
	printDataByHexa("$$$ send Data", pData, dataLen);
#endif
}

//int Transport::sendBulkData(unsigned char* pData, int dataLen)
int Transport::sendBulkData(ByteArray& rSendPacket)
{
	ssize_t nWriteByte = 0;

	const char* pData = rSendPacket.constData();
	int dataLen = rSendPacket.length();
	int err_count = 10;
	int write_count = 100;

	//LOGD("sendBulkData() dataLen[%d]", dataLen);

	if (mWriterFD == -1)
		LOGE("	ERROR : mWriterFD is -1");

	//printDataByHexa((char*)"$$$ send Data", (unsigned char*)pData, dataLen);

	while (nWriteByte != dataLen && err_count != 0 && write_count != 0) {
		nWriteByte = write(mWriterFD, pData, dataLen);
		LOGD("Transport write!![%p], dataLen : [%d], nWriteByte : [%d]", pData, dataLen, nWriteByte);
		if (nWriteByte < 0) {
			LOGE("	ERROR : fail to write mWriterFD[%d] error[%d][%s]", mWriterFD, errno, strerror(errno));
			err_count--;
		}
		
		else {
            LOGD("write data is not sent at once.");
			dataLen = dataLen - nWriteByte;
			pData = pData + dataLen;
			nWriteByte = 0;
			write_count--;
		}
	}
	return 0;
}

void Transport::assembleRcvPacket(uint8_t* pRcvHidPaket, uint32_t length)
{
	//LOGD("	pRcvHidPaket[1]:[%u] length[%u]", pRcvHidPaket[1], length);
    switch(pRcvHidPaket[1]) {
	    case 0:
	        processInBoundTraffic(&pRcvHidPaket[2], length-2);
	        break;

	    case 1: //Last
	        mRcvPacketRepo.append((char*)&pRcvHidPaket[2], length-2);
			processInBoundTraffic((uint8_t*)mRcvPacketRepo.data(), (uint32_t)mRcvPacketRepo.length());
	        break;

	    case 2: //First
	        mRcvPacketRepo.resize(0);
	        mRcvPacketRepo.append((char*)&pRcvHidPaket[2], length-2);
	        break;

	    case 3: //Middle
	        mRcvPacketRepo.append((char*)&pRcvHidPaket[2], length-2);
	        break;
    }
}


/*
Inbound iAP2 traffic
--------------------

The accessory should perform the following on incoming iAP2 traffic:

1) Create iAP2Packet by calling iAP2PacketCreateEmptyRecvPacket
2) Parse the incoming data by calling iAP2PacketParseBuffer
   (pass in data buffer and iAP2Packet structure)
3) Check if a full iAP2Packet has been parsed by calling iAP2PacketIsComplete
4) If a complete iAP2Packet has not been parsed, go to (2)
5) Pass the complete iAP2Packet to the link layer by calling
   iAP2LinkRunLoopHandleReadyPacket
*/
void Transport::processInBoundTraffic(uint8_t* pRcvPaket, uint32_t length)
{
	int testLoopCount = 0;
	BOOL bDetect;
	uint32_t failedCheckSum;

	uint32_t sopDetect = 0;
	iAP2LinkRunLoop_t* pIap2RunLoop = (iAP2LinkRunLoop_t*)mpUserData;
	if (pRcvPaket == NULL) {
		LOGE("	ERROR : pRcvPaket is NULL");
		return;
	}
	if (pIap2RunLoop) {
		uint32_t nTotParsedBuffer = 0;
		do {
			if (mbRcbPacketIsComplete == true) {
				mpRcvPacket = iAP2PacketCreateEmptyRecvPacket(pIap2RunLoop->link);
			}
			/* delete becasue system peformance issue.
			if (testLoopCount>1)
				LOGD("	>>>>> processInBoundTraffic(). mpRcvPacket[0x%X] testLoopCount[%d] length[%u] nTotParsedBuffer[%d]", mpRcvPacket, testLoopCount, length, nTotParsedBuffer);
			*/
			nTotParsedBuffer += iAP2PacketParseBuffer((const uint8_t*)(pRcvPaket+nTotParsedBuffer), length-nTotParsedBuffer, mpRcvPacket, MAX_RCV_PACKET_SIZE/*0x80*/, &bDetect, &failedCheckSum, &sopDetect);

			mbRcbPacketIsComplete = (bool)iAP2PacketIsComplete(mpRcvPacket);
			if(mbRcbPacketIsComplete == true) {
				iAP2LinkRunLoopHandleReadyPacket(pIap2RunLoop, mpRcvPacket);
				mpRcvPacket = NULL;
			}
            /* delete becasue system peformance issue.
			else {
				LOGW("	Not compete length[%d] mpRcvPacket[%p]", length, mpRcvPacket);
			}
            */
			testLoopCount++;
		} while (nTotParsedBuffer < length);

        if( nbeforeLength != length) {
            LOGD(" processInBoundTraffic(). mbRcbPacketIsComplete[%d] testLoopCount[%d] length[%u] nTotParsedBuffer[%d] nbeforeLength[%d]", mbRcbPacketIsComplete, testLoopCount, length, nTotParsedBuffer, nbeforeLength);
            nbeforeLength = length;
        }
	}

}

}// namespace IAP2
