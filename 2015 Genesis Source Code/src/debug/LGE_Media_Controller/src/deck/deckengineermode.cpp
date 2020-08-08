#include "deckengineermode.h"
#include "deckmanager.h"
#include "packet.h"
#include "util.h"

DeckEngineerMode::DeckEngineerMode(QObject *parent) : QObject(parent),
m_nResetCount(0),
m_bStartDiagnostic(false)
{   
    m_pTempTable[0] = 125;
    m_pTempTable[1] = 125;
    m_pTempTable[2] = 125;
    m_pTempTable[3] = 125;
    m_pTempTable[4] = 125;
    m_pTempTable[5] = 125;
    m_pTempTable[6] = 125;
    m_pTempTable[7] = 125;
    m_pTempTable[8] = 125;
    m_pTempTable[9] = 125;
    m_pTempTable[10] = 125;
    m_pTempTable[11] = 125;
    m_pTempTable[12] = 125;
    m_pTempTable[13] = 125;
    m_pTempTable[14] = 125;
    m_pTempTable[15] = 125;
    m_pTempTable[16] = 124;
    m_pTempTable[17] = 120;
    m_pTempTable[18] = 118;
    m_pTempTable[19] = 115;
    m_pTempTable[20] = 113;
    m_pTempTable[21] = 111;
    m_pTempTable[22] = 110;
    m_pTempTable[23] = 108;
    m_pTempTable[24] = 106;
    m_pTempTable[25] = 105;
    m_pTempTable[26] = 103;
    m_pTempTable[27] = 102;
    m_pTempTable[28] = 100;
    m_pTempTable[29] = 99;
    m_pTempTable[30] = 98;
    m_pTempTable[31] = 97;
    m_pTempTable[32] = 95;
    m_pTempTable[33] = 94;
    m_pTempTable[34] = 94;
    m_pTempTable[35] = 93;
    m_pTempTable[36] = 92;
    m_pTempTable[37] = 91;
    m_pTempTable[38] = 90;
    m_pTempTable[39] = 89;
    m_pTempTable[40] = 88;
    m_pTempTable[41] = 88;
    m_pTempTable[42] = 87;
    m_pTempTable[43] = 86;
    m_pTempTable[44] = 85;
    m_pTempTable[45] = 84;
    m_pTempTable[46] = 83;
    m_pTempTable[47] = 82;
    m_pTempTable[48] = 81;
    m_pTempTable[49] = 81;
    m_pTempTable[50] = 80;
    m_pTempTable[51] = 79;
    m_pTempTable[52] = 78;
    m_pTempTable[53] = 77;
    m_pTempTable[54] = 76;
    m_pTempTable[55] = 75;
    m_pTempTable[56] = 75;
    m_pTempTable[57] = 74;
    m_pTempTable[58] = 74;
    m_pTempTable[59] = 73;
    m_pTempTable[60] = 73;
    m_pTempTable[61] = 72;
    m_pTempTable[62] = 72;
    m_pTempTable[63] = 71;
    m_pTempTable[64] = 71;
    m_pTempTable[65] = 70;
    m_pTempTable[66] = 69;
    m_pTempTable[67] = 69;
    m_pTempTable[68] = 88;
    m_pTempTable[69] = 88;
    m_pTempTable[70] = 67;
    m_pTempTable[71] = 67;
    m_pTempTable[72] = 66;
    m_pTempTable[73] = 66;
    m_pTempTable[74] = 65;
    m_pTempTable[75] = 65;
    m_pTempTable[76] = 64;
    m_pTempTable[77] = 64;
    m_pTempTable[78] = 63;
    m_pTempTable[79] = 63;
    m_pTempTable[80] = 62;
    m_pTempTable[81] = 62;
    m_pTempTable[82] = 61;
    m_pTempTable[83] = 61;
    m_pTempTable[84] = 60;
    m_pTempTable[85] = 60;
    m_pTempTable[86] = 59;
    m_pTempTable[87] = 59;
    m_pTempTable[88] = 58;
    m_pTempTable[89] = 58;
    m_pTempTable[90] = 57;
    m_pTempTable[91] = 57;
    m_pTempTable[92] = 56;
    m_pTempTable[93] = 56;
    m_pTempTable[94] = 55;
    m_pTempTable[95] = 55;
    m_pTempTable[96] = 54;
    m_pTempTable[97] = 54;
    m_pTempTable[98] = 53;
    m_pTempTable[99] = 53;
    m_pTempTable[100] = 52;
    m_pTempTable[101] = 52;
    m_pTempTable[102] = 51;
    m_pTempTable[103] = 51;
    m_pTempTable[104] = 50;
    m_pTempTable[105] = 50;
    m_pTempTable[106] = 49;
    m_pTempTable[107] = 49;
    m_pTempTable[108] = 49;
    m_pTempTable[109] = 48;
    m_pTempTable[110] = 48;
    m_pTempTable[111] = 48;
    m_pTempTable[112] = 47;
    m_pTempTable[113] = 47;
    m_pTempTable[114] = 47;
    m_pTempTable[115] = 46;
    m_pTempTable[116] = 46;
    m_pTempTable[117] = 46;
    m_pTempTable[118] = 45;
    m_pTempTable[119] = 45;
    m_pTempTable[120] = 45;
    m_pTempTable[121] = 44;
    m_pTempTable[122] = 44;
    m_pTempTable[123] = 43;
    m_pTempTable[124] = 43;
    m_pTempTable[125] = 42;
    m_pTempTable[126] = 42;
    m_pTempTable[127] = 41;
    m_pTempTable[128] = 41;
    m_pTempTable[129] = 40;
    m_pTempTable[130] = 40;
    m_pTempTable[131] = 40;
    m_pTempTable[132] = 39;
    m_pTempTable[133] = 39;
    m_pTempTable[134] = 38;
    m_pTempTable[135] = 38;
    m_pTempTable[136] = 38;
    m_pTempTable[137] = 37;
    m_pTempTable[138] = 37;
    m_pTempTable[139] = 37;
    m_pTempTable[140] = 36;
    m_pTempTable[141] = 36;
    m_pTempTable[142] = 35;
    m_pTempTable[143] = 35;
    m_pTempTable[144] = 35;
    m_pTempTable[145] = 34;
    m_pTempTable[146] = 34;
    m_pTempTable[147] = 34;
    m_pTempTable[148] = 33;
    m_pTempTable[149] = 33;
    m_pTempTable[150] = 33;
    m_pTempTable[151] = 32;
    m_pTempTable[152] = 32;
    m_pTempTable[153] = 32;
    m_pTempTable[154] = 31;
    m_pTempTable[155] = 31;
    m_pTempTable[156] = 30;
    m_pTempTable[157] = 30;
    m_pTempTable[158] = 29;
    m_pTempTable[159] = 29;
    m_pTempTable[160] = 29;
    m_pTempTable[161] = 28;
    m_pTempTable[162] = 28;
    m_pTempTable[163] = 28;
    m_pTempTable[164] = 27;
    m_pTempTable[165] = 27;
    m_pTempTable[166] = 27;
    m_pTempTable[167] = 26;
    m_pTempTable[168] = 26;
    m_pTempTable[169] = 26;
    m_pTempTable[170] = 25;
    m_pTempTable[171] = 25;
    m_pTempTable[172] = 24;
    m_pTempTable[173] = 24;
    m_pTempTable[174] = 24;
    m_pTempTable[175] = 23;
    m_pTempTable[176] = 23;
    m_pTempTable[177] = 22;
    m_pTempTable[178] = 22;
    m_pTempTable[179] = 21;
    m_pTempTable[180] = 21;
    m_pTempTable[181] = 20;
    m_pTempTable[182] = 20;
    m_pTempTable[183] = 19;
    m_pTempTable[184] = 19;
    m_pTempTable[185] = 18;
    m_pTempTable[186] = 18;
    m_pTempTable[187] = 17;
    m_pTempTable[188] = 17;
    m_pTempTable[189] = 16;
    m_pTempTable[190] = 16;
    m_pTempTable[191] = 15;
    m_pTempTable[192] = 15;
    m_pTempTable[193] = 15;
    m_pTempTable[194] = 14;
    m_pTempTable[195] = 14;
    m_pTempTable[196] = 13;
    m_pTempTable[197] = 13;
    m_pTempTable[198] = 12;
    m_pTempTable[199] = 12;
    m_pTempTable[200] = 12;
    m_pTempTable[201] = 11;
    m_pTempTable[202] = 11;
    m_pTempTable[203] = 10;
    m_pTempTable[204] = 10;
    m_pTempTable[205] = 9;
    m_pTempTable[206] = 9;
    m_pTempTable[207] = 8;
    m_pTempTable[208] = 8;
    m_pTempTable[209] = 7;
    m_pTempTable[210] = 6;
    m_pTempTable[211] = 6;
    m_pTempTable[212] = 5;
    m_pTempTable[213] = 4;
    m_pTempTable[214] = 4;
    m_pTempTable[215] = 3;
    m_pTempTable[216] = 2;
    m_pTempTable[217] = 2;
    m_pTempTable[218] = 1;
    m_pTempTable[219] = 0;
    m_pTempTable[220] = 0;
    m_pTempTable[221] = -1;
    m_pTempTable[222] = -1;
    m_pTempTable[223] = -2;
    m_pTempTable[224] = -3;
    m_pTempTable[225] = -4;
    m_pTempTable[226] = -5;
    m_pTempTable[227] = -6;
    m_pTempTable[228] = -7;
    m_pTempTable[229] = -8;
    m_pTempTable[230] = -9;
    m_pTempTable[231] = -10;
    m_pTempTable[232] = -11;
    m_pTempTable[233] = -12;
    m_pTempTable[234] = -13;
    m_pTempTable[235] = -14;
    m_pTempTable[236] = -15;
    m_pTempTable[237] = -16;
    m_pTempTable[238] = -17;
    m_pTempTable[239] = -18;
    m_pTempTable[240] = -19;
    m_pTempTable[241] = -20;
    m_pTempTable[242] = -22;
    m_pTempTable[243] = -24;
    m_pTempTable[244] = -26;
    m_pTempTable[245] = -28;
    m_pTempTable[246] = -31;
    m_pTempTable[247] = -34;
    m_pTempTable[248] = -39;
    m_pTempTable[249] = -40;
    m_pTempTable[250] = -40;
    m_pTempTable[251] = -40;
    m_pTempTable[252] = -40;
    m_pTempTable[253] = -40;
    m_pTempTable[254] = -40;
    m_pTempTable[255] = -40;
}

DeckEngineerMode::~DeckEngineerMode()
{
    MC_LOG << "~DeckEngineerMode()" << LOG_ENDL;
}

int DeckEngineerMode::GetFirmwareVersion()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    return tStatus.m_nDeckVerNo;
}

int DeckEngineerMode::GetLifeTime()
{
    int nLifeTime = DeckManager::GetInstance()->GetLifeTime();

    MC_LOG << "DeckEngineerMode::GetLifeTime(): " << nLifeTime << LOG_ENDL;
    
    return nLifeTime / 3600;    // hour
}

int DeckEngineerMode::GetTemperature()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);

    int nTempCode = tStatus.m_nTemperature;

    if(nTempCode < 0x00)      return 0;
    else if(nTempCode > 0xFF) return 0;

    return m_pTempTable[nTempCode];        
}

int DeckEngineerMode::GetEjectStatus()
{
    return DeckManager::GetInstance()->IsEject();
}

int DeckEngineerMode::GetInsertStatus()
{
    if(DeckManager::GetInstance()->IsEject() == true) 
        return false;
    else 
        return true;
}

int DeckEngineerMode::GetRandomStatus()
{
    return DeckManager::GetInstance()->GetRandomMode();
}

int DeckEngineerMode::GetRepeatStatus()
{
    return DeckManager::GetInstance()->GetRepeatMode();
}

int DeckEngineerMode::GetScanStatus()
{
    return DeckManager::GetInstance()->GetScanMode();
}

int DeckEngineerMode::GetMechError()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);
    
    return tStatus.m_nMechaError;
}

int DeckEngineerMode::GetReadError()
{
    DeckCommonStatus    tStatus;
    DeckManager::GetInstance()->GetCommonStatus(tStatus);
    
    return tStatus.m_nReadError;
}

int DeckEngineerMode::GetRegionCode()
{
    DeckInit1Setting tInit1;
    DeckManager::GetInstance()->GetInitSetting1(tInit1);
    
    return tInit1.m_nRegionCode;
}

int DeckEngineerMode::SetRegionCode(int nCode)
{
    EDeckInit1Setting tSetting;
    GetInitSetting1(tSetting);
    
    tSetting.m_nRegionCode = nCode;
    
    SetInitSetting1(tSetting);

    return 1;
}

int DeckEngineerMode::GetMenuLanguage()
{
    DeckInit1Setting tInit1;
    DeckManager::GetInstance()->GetInitSetting1(tInit1);
    
    return tInit1.m_nPrioMenuLangCode;
}

int DeckEngineerMode::SetMenuLanguage(int Code)
{
    EDeckInit1Setting tSetting;
    GetInitSetting1(tSetting);
    
    tSetting.m_nPrioMenuLangCode = Code;
    
    SetInitSetting1(tSetting);
    
    return true;
}

int DeckEngineerMode::SetInitSetting1(const EDeckInit1Setting &tSetting)
{
    BCD2 tBCD2;
    BCD4 tBCD4;
    InitialSetting1Command tCmd;
    
    tCmd.nOSD                   = tSetting.m_bOSDOff;
    tCmd.nRegionCode            = tSetting.m_nRegionCode;
    tCmd.nParentalLevel         = tSetting.m_nParentalLevel;

    if(tSetting.m_nPrioSubTitleCode == 0) {
        tBCD2.BCD.upper             = 0xF;
        tBCD2.BCD.lower             = 0xF;
        tCmd.nSubTitleCodeUpper     = tBCD2.byte;
        tCmd.nSubTitleCodeLower     = tBCD2.byte;   
        tCmd.nSubTitle              = 1;
    }else{
        tBCD2.BCD.upper             = (tSetting.m_nPrioSubTitleCode / 100)/10;
        tBCD2.BCD.lower             = (tSetting.m_nPrioSubTitleCode / 100)%10;
        tCmd.nSubTitleCodeUpper     = tBCD2.byte;
        tBCD2.BCD.upper             = (tSetting.m_nPrioSubTitleCode % 100)/10;
        tBCD2.BCD.lower             = (tSetting.m_nPrioSubTitleCode % 100)%10;
        tCmd.nSubTitleCodeLower     = tBCD2.byte;   
        tCmd.nSubTitle              = 0;
    }

    tBCD2.BCD.upper             = (tSetting.m_nPrioAudioCode / 100)/10;
    tBCD2.BCD.lower             = (tSetting.m_nPrioAudioCode / 100)%10;
    tCmd.nAudioCodeUpper        = tBCD2.byte;
    tBCD2.BCD.upper             = (tSetting.m_nPrioAudioCode % 100)/10;
    tBCD2.BCD.lower             = (tSetting.m_nPrioAudioCode % 100)%10;
    tCmd.nAudioCodeLower        = tBCD2.byte;

    tBCD2.BCD.upper             = (tSetting.m_nPrioMenuLangCode / 100)/10;
    tBCD2.BCD.lower             = (tSetting.m_nPrioMenuLangCode / 100)%10;
    tCmd.nMenuLangCodeUpper     = tBCD2.byte;
    tBCD2.BCD.upper             = (tSetting.m_nPrioMenuLangCode % 100)/10;
    tBCD2.BCD.lower             = (tSetting.m_nPrioMenuLangCode % 100)%10;
    tCmd.nMenuLangCodeLower     = tBCD2.byte;

    tCmd.nScreenSize            = tSetting.m_nScreenSize;
    tCmd.nAngle                 = tSetting.m_nAngleDisplay;
    tCmd.nUndefined1            = 0;
    
    tBCD4.word                  = tSetting.m_nPinNumber;
    tCmd.nPinNumber1            = tBCD4.BCD.upper1;
    tCmd.nPinNumber2            = tBCD4.BCD.lower1;
    tCmd.nPinNumber3            = tBCD4.BCD.upper2;
    tCmd.nPinNumber4            = tBCD4.BCD.lower2;

    tCmd.nOpenningScreen        = tSetting.m_nOpenningScreen;
    tCmd.nUndefined2            = 0;

    tCmd.nBackColor             = tSetting.m_nBackColor;
    tCmd.nUndefined3            = 0;
    tCmd.nIRE                   = tSetting.m_nIRE;
    tCmd.nPBC                   = tSetting.m_nPBC;
    tCmd.nVideoMode             = tSetting.m_nPictureMode;
    tCmd.nScreenMode            = tSetting.m_nScreenMode;
    
    tCmd.nCountryCodeUpper      = tSetting.m_nCountryCode >> 8;
    tCmd.nCountryCodeLower      = tSetting.m_nCountryCode & 0xFF;
   
    tCmd.nUndefined4            = 0;
    tCmd.nPlayerMode            = tSetting.m_nPlayerMode;
    tCmd.nVideoOutType          = tSetting.m_nVideoOutput;

    return Packet::GetInstace()->InitializationSetting1(&tCmd);
}

int DeckEngineerMode::SetInitSetting2a(const EDeckInit2aSetting &tSetting)
{
    InitialSetting2aCommand tCmd;
    
    tCmd.nAudioOutput       = tSetting.m_nAudioOutput;
    tCmd.nUndefined1        = 0;
    tCmd.nDualMono          = tSetting.m_nDualMono;
    tCmd.nPCMSetting        = tSetting.m_nPCMSetting;
    tCmd.nDSPIFDownMix      = tSetting.m_nDSPIFDownMix;
    tCmd.n2chIFDownMix      = tSetting.m_n2chIFDownMix;
    tCmd.nFrontSp           = tSetting.m_nFrontSp;
    tCmd.nCenterSp          = tSetting.m_nCenterSp;
    tCmd.nRearSp            = tSetting.m_nRearSp;
    tCmd.nSubWoofer         = tSetting.m_nSubwoofer;
    tCmd.nCenterSpDelay     = tSetting.m_nCenterSpDelay;
    tCmd.nRearSpDelay       = tSetting.m_nRearSpDelay;
    tCmd.nCompLevel         = tSetting.m_nCompLevel;
    tCmd.nUndefined2        = 0;
    tCmd.nFrontLSpLevel     = tSetting.m_nFrontLSpLevel;
    tCmd.nFrontRSpLevel     = tSetting.m_nFrontRSpLevel;
    tCmd.nRearLSpLevel      = tSetting.m_nRearLSpLevel;
    tCmd.nRearRSpLevel      = tSetting.m_nRearRSpLevel;
    tCmd.nCenterSpLevel     = tSetting.m_nCenterSpLevel;
    tCmd.nDTSOutput         = tSetting.m_nDTSOutput;
    tCmd.n6chStereo         = tSetting.m_n6chStereo;
    tCmd.nBassManagement    = tSetting.m_nBaseMngr;
    tCmd.nDownSampling      = tSetting.m_nDownSampling;
    tCmd.nUndefined3        = 0;
    tCmd.nUndefined4        = 0;
    tCmd.nDOLPro12          = tSetting.m_nDolPro12;
    tCmd.nCenterBlendLevel  = tSetting.m_nCenterBlendLvl;
    tCmd.nUndefined5        = 0;
    tCmd.nDimension         = tSetting.m_nDimension;
    tCmd.nPanorama          = tSetting.m_nPanorama;
    tCmd.nCenterWidth       = tSetting.m_nCenterWidth;
    tCmd.nUndefined6        = 0;

    return Packet::GetInstace()->InitializationSetting2a(&tCmd);
}

int DeckEngineerMode::SetInitSetting3(const EDeckInit3Setting &tSetting)
{
    InitialSetting3Command tCmd;

    tCmd.nNoFileFolder      = tSetting.m_nNoFileFolder;
    tCmd.nUndefined1        = 0;
    tCmd.nPriorityID3Tag    = tSetting.m_nPriorityID3Tag;
    tCmd.nMP3Play           = tSetting.m_nMP3Play;
    tCmd.nWMAPlay           = tSetting.m_nWMAPlay;
    tCmd.nUndefined2        = 0;
    tCmd.nPVDSVD            = tSetting.m_nPVDSVD;


    tCmd.nUndefined3        = 0;
    tCmd.nRandomTrackUp     = tSetting.m_nRandomTrackUp;
    tCmd.nRepeatCancel      = tSetting.m_nRepeatCancel;
    tCmd.nRandomCancel      = tSetting.m_nRandomCancel;
    tCmd.nFastUpPlay        = tSetting.m_nFastUpPlay;
    tCmd.nFastDownPlay      = tSetting.m_nFastDownPlay;
    tCmd.nTrackDownTime     = tSetting.m_nTrackDownTime;

    tCmd.nUndefined4        = 0;
    tCmd.nSingleSession     = tSetting.m_nSingleSession;
    tCmd.nDVDAudioPlay      = tSetting.m_nDVDAudioPlay;
    tCmd.nHighTempStop      = tSetting.m_nHaltByHighTemp;

    tCmd.nUndefined5        = 0;
    tCmd.nTitleEnd          = tSetting.m_nTitleEnd;
    tCmd.nEjectResume       = tSetting.m_nEjectRsume;

    tCmd.nUndefined6        = 0;
    tCmd.nDirPriority       = tSetting.m_nDirPriority;
    tCmd.nPlayTimeDisplay   = tSetting.m_nPlayTimeDisplay;
    tCmd.nUndefined7        = 0;

    tCmd.nUndefined8        = 0;
    tCmd.nUndefined9        = 0;
    
    return Packet::GetInstace()->InitializationSetting3(&tCmd);
}

int DeckEngineerMode::GetInitSetting1(EDeckInit1Setting &tSetting)
{
    DeckInit1Setting tInit1;
    DeckManager::GetInstance()->GetInitSetting1(tInit1);

    tSetting.m_bOSDOff              = tInit1.m_bOSDOff;
    tSetting.m_nRegionCode          = tInit1.m_nRegionCode;
    tSetting.m_nParentalLevel       = tInit1.m_nParentalLevel;
    tSetting.m_nPrioSubTitleCode    = tInit1.m_nPrioSubTitleCode;
    tSetting.m_nPrioAudioCode       = tInit1.m_nPrioAudioCode;
    tSetting.m_nPrioMenuLangCode    = tInit1.m_nPrioMenuLangCode;
    tSetting.m_nScreenSize          = tInit1.m_nScreenSize;
    tSetting.m_nAngleDisplay        = tInit1.m_nAngleDisplay;
    tSetting.m_nSubTitleDisplay     = tInit1.m_nSubTitleDisplay;
    tSetting.m_nPinNumber           = tInit1.m_nPinNumber;
    tSetting.m_nOpenningScreen      = tInit1.m_nOpenningScreen;
    tSetting.m_nScreenMode          = tInit1.m_nScreenMode;
    tSetting.m_nPictureMode         = tInit1.m_nPictureMode;
    tSetting.m_nPBC                 = tInit1.m_nPBC;
    tSetting.m_nIRE                 = tInit1.m_nIRE;
    tSetting.m_nBackColor           = tInit1.m_nBackColor;
    tSetting.m_nCountryCode         = tInit1.m_nCountryCode;
    tSetting.m_nPlayerMode          = tInit1.m_nPlayerMode;
    tSetting.m_nVideoOutput         = tInit1.m_nVideoOutput;
    tSetting.m_nAngleDisplay        = tInit1.m_nAngleDisplay;

    return 1;
}

int DeckEngineerMode::GetInitSetting2a(EDeckInit2aSetting &tSetting)
{
    DeckInit2aSetting tInit2a;
    DeckManager::GetInstance()->GetInitSetting2a(tInit2a);

    tSetting.m_nAudioOutput     =	tInit2a.m_nAudioOutput;
    tSetting.m_nDualMono        =	tInit2a.m_nDualMono;
    tSetting.m_nPCMSetting      =	tInit2a.m_nPCMSetting;
    tSetting.m_nDSPIFDownMix    =	tInit2a.m_nDSPIFDownMix;
    tSetting.m_n2chIFDownMix    =	tInit2a.m_n2chIFDownMix;
    tSetting.m_nFrontSp         =	tInit2a.m_nFrontSp;
    tSetting.m_nCenterSp        =	tInit2a.m_nCenterSp;
    tSetting.m_nRearSp          =	tInit2a.m_nRearSp;
    tSetting.m_nSubwoofer       =	tInit2a.m_nSubwoofer;
    tSetting.m_nCenterSpDelay   =	tInit2a.m_nCenterSpDelay;
    tSetting.m_nRearSpDelay     =	tInit2a.m_nRearSpDelay;
    tSetting.m_nCompLevel       =	tInit2a.m_nCompLevel;
    tSetting.m_nFrontLSpLevel   =	tInit2a.m_nFrontLSpLevel;
    tSetting.m_nFrontRSpLevel   =	tInit2a.m_nFrontRSpLevel;
    tSetting.m_nRearLSpLevel    =	tInit2a.m_nRearLSpLevel;
    tSetting.m_nRearRSpLevel    =	tInit2a.m_nRearRSpLevel;
    tSetting.m_nCenterSpLevel   =	tInit2a.m_nCenterSpLevel;
    tSetting.m_nSubwooferLevel  =	tInit2a.m_nSubwooferLevel;
    tSetting.m_nDTSOutput       =	tInit2a.m_nDTSOutput;
    tSetting.m_n6chStereo       =	tInit2a.m_n6chStereo;
    tSetting.m_nBaseMngr        =	tInit2a.m_nBaseMngr;
    tSetting.m_nDownSampling    =	tInit2a.m_nDownSampling;
    tSetting.m_nDolPro12        =	tInit2a.m_nDolPro12;
    tSetting.m_nCenterBlendLvl  =	tInit2a.m_nCenterBlendLvl;
    tSetting.m_nDimension       =	tInit2a.m_nDimension;
    tSetting.m_nPanorama        =	tInit2a.m_nPanorama;
    tSetting.m_nCenterWidth     =	tInit2a.m_nCenterWidth;

    return 1;
}

int DeckEngineerMode::GetInitSetting3(EDeckInit3Setting &tSetting)
{
    DeckInit3Setting tInit3;
    DeckManager::GetInstance()->GetInitSetting3(tInit3);

    tSetting.m_nNoFileFolder        = tInit3.m_nNoFileFolder;
    tSetting.m_nPriorityID3Tag      = tInit3.m_nPriorityID3Tag;
    tSetting.m_nMP3Play             = tInit3.m_nMP3Play;
    tSetting.m_nWMAPlay             = tInit3.m_nWMAPlay;
    tSetting.m_nPVDSVD              = tInit3.m_nPVDSVD;
    tSetting.m_nRandomTrackUp       = tInit3.m_nRandomTrackUp;
    tSetting.m_nRepeatCancel        = tInit3.m_nRepeatCancel;
    tSetting.m_nRandomCancel        = tInit3.m_nRandomCancel;
    tSetting.m_nFastUpPlay          = tInit3.m_nFastUpPlay;
    tSetting.m_nFastDownPlay        = tInit3.m_nFastDownPlay;
    tSetting.m_nTrackDownTime       = tInit3.m_nTrackDownTime;
    tSetting.m_nSingleSession       = tInit3.m_nSingleSession;
    tSetting.m_nDVDAudioPlay        = tInit3.m_nDVDAudioPlay;
    tSetting.m_nHaltByHighTemp      = tInit3.m_nHaltByHighTemp;
    tSetting.m_nTitleEnd            = tInit3.m_nTitleEnd;
    tSetting.m_nEjectRsume          = tInit3.m_nEjectRsume;
    tSetting.m_nDirPriority         = tInit3.m_nDirPriority;
    tSetting.m_nPlayTimeDisplay     = tInit3.m_nPlayTimeDisplay;

    return 1;
}

void DeckEngineerMode::StartDiagnostic()
{
    if(m_bStartDiagnostic == false) {
        m_bStartDiagnostic = true;
        connect(DeckManager::GetInstance(), SIGNAL(OnCommErrorWithDeckChanged(int)), this, SLOT(HandleCommErrorWithDeckChanged(int)));
        DeckManager::GetInstance()->StartDiagnostic();
        MC_LOG << "DeckEngineerMode::StartDiagnostic()" << LOG_ENDL;
    }
}

void DeckEngineerMode::EndDiagnostic()
{
    if(m_bStartDiagnostic == true) {
        m_bStartDiagnostic = false;
        disconnect(DeckManager::GetInstance(), SIGNAL(OnCommErrorWithDeckChanged(int)), this, SLOT(HandleCommErrorWithDeckChanged(int)));
        MC_LOG << "DeckEngineerMode::EndDiagnostic()" << LOG_ENDL;
    }
}

void DeckEngineerMode::HandleCommErrorWithDeckChanged(int bError)
{
    if(bError == true && m_nResetCount < 3) 
    {
        m_nResetCount++;
        Packet::GetInstace()->ResetStart();
        MC_HIGH << "DeckEngineerMode::HandleCommErrorWithDeckChanged() m_nResetCount: " << m_nResetCount << LOG_ENDL;
    }
    else if(bError == false && m_nResetCount >= 3) 
    {
        m_nResetCount = 0;
    }
    
    MC_HIGH << "DeckEngineerMode::HandleCommErrorWithDeckChanged(" << bError << ")" << LOG_ENDL;
    
    emit OnCommErrorWithDeckChanged(bError);
}

void DeckEngineerMode::DiscInOutTest()
{ //added by wonseok.heo for in out test
   DeckManager::GetInstance()->CallDiscTestPlay();
}
