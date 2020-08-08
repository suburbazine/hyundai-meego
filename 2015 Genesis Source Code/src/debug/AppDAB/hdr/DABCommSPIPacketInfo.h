#ifndef DABCOMMSPIPACKETINFO_H
#define DABCOMMSPIPACKETINFO_H

#include <QObject>

class DABCommSPIPacketInfo : public QObject
{
    Q_OBJECT
public:
    explicit DABCommSPIPacketInfo(QObject *parent = 0);
    void setDevice(unsigned char conti){this->device = conti;}
    void setFrameposition(unsigned char framepos){this->frameposition = framepos;}
    void setTotalLength(quint16 totalLen){this->totalLength = totalLen;}
    void setCommandValue(quint8 value){this->commandValue = value;}
    void setSLSFileName(QString str){this->slsFilename = str;}
    void setLogoFilename(QString str){this->m_sLogoFilename = str;}
    void setTempLogoFilename(QString str){this->m_sTempLogoFilename = str;}
    void setPacketBuffer(quint8* pPacketBuffer){this->packetBuffer = pPacketBuffer;}
    void setPacketLastIndex(int index){this->packetLastIndex = index;}
    void washOffData(void);
    unsigned char getDevice(){return this->device;}
    unsigned char getFrameposition(){return this->frameposition;}
    quint16 getTotalLength(){return this->totalLength;}

    quint8 getCommandValue(){return this->commandValue;}
    QString getSLSFileName(){return this->slsFilename;}
    QString getLogoFilename(){return this->m_sLogoFilename;}
    QString getTempLogoFilename(){return this->m_sTempLogoFilename;}
    quint8* getPacketBuffer(){return this->packetBuffer;}
    int getPacketLastIndex(){return this->packetLastIndex;}
    void setExistLogo(bool exist) { this->m_bExistLogo = exist; }
    bool getExistLogo() { return this->m_bExistLogo; }

    void setCommDataHandler(QObject* pCommManger);

    DABCommSPIPacketInfo* getCommSPIPacketInfo(){return this;}

private:
    unsigned char device;
    unsigned char frameposition;
    quint16 totalLength;
    quint8 commandValue;
    QObject* pDABCommManager;
    QString slsFilename;
    QString m_sLogoFilename;
    QString m_sTempLogoFilename;
    quint8* packetBuffer;
    int packetLastIndex;
    bool m_bExistLogo;
signals:

public slots:

};

#endif // DABCOMMSPIPACKETINFO_H
