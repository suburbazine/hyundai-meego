#include "util/audio/DHAVN_AppMediaPlayer_Utils.h"

QString AudioUtils::m_strUSBRoot  = QString();    
QString AudioUtils::m_strUSB1Path = QString();
QString AudioUtils::m_strUSB2Path = QString();    


MEDIA_SOURCE_T AudioUtils::DeviceType(MP::DEVICE_T internalType)
{
    switch (internalType)
    {
        case MP::JUKEBOX:
            return MEDIA_SOURCE_JUKEBOX;
        case MP::USB1:
            return MEDIA_SOURCE_USB1;
        case MP::USB2:
            return MEDIA_SOURCE_USB2;
        case MP::DISC:
            return MEDIA_SOURCE_CDDA;
        case MP::IPOD1:
            return MEDIA_SOURCE_IPOD1;
        case MP::IPOD2:
            return MEDIA_SOURCE_IPOD2;
        case MP::AUX:
            return MEDIA_SOURCE_AUX1;
        // { added by wspark 2012.12.10 for sending bluetooth opstate
        case MP::BLUETOOTH:
            return MEDIA_SOURCE_BLUETOOTH;
        // } added by wspark
        default:
            return MEDIA_SOURCE_MAX;
    }
}

MP::DEVICE_T AudioUtils::DeviceType(MODE_STATE_T externalType)
{
    switch (externalType)
    {
        case MODE_STATE_JUKEBOX_AUDIO:
            return MP::JUKEBOX;
        case MODE_STATE_USB1_AUDIO:
            return MP::USB1;
        case MODE_STATE_USB2_AUDIO:
            return MP::USB2;
        case MODE_STATE_DISC_AUDIO:
            return MP::DISC;
        case MODE_STATE_IPOD1_AUDIO:
            return MP::IPOD1;
        case MODE_STATE_IPOD2_AUDIO:
            return MP::IPOD2;
        case MODE_STATE_AUX1_AUDIO:
        case MODE_STATE_AUX2_AUDIO:
            return MP::AUX;
        case MODE_STATE_BT_AUDIO:
            return MP::BLUETOOTH;
        default:
            return MP::UNDEFINED;
    }
}

MP::DEVICE_T AudioUtils::DeviceType(MEDIA_SOURCE_T externalType)
{
    switch (externalType)
    {
        case MEDIA_SOURCE_JUKEBOX:
            return MP::JUKEBOX;
        case MEDIA_SOURCE_USB1:
            return MP::USB1;
        case MEDIA_SOURCE_USB2:
            return MP::USB2;
        case MEDIA_SOURCE_CDDA:
        case MEDIA_SOURCE_MP3:
        case MEDIA_SOURCE_DVDAUDIO:
            return MP::DISC;
        case MEDIA_SOURCE_IPOD1:
            return MP::IPOD1;
        case MEDIA_SOURCE_IPOD2:
            return MP::IPOD2;
        case MEDIA_SOURCE_AUX1:
        case MEDIA_SOURCE_AUX2:
            return MP::AUX;
        case MEDIA_SOURCE_BLUETOOTH:
            return MP::BLUETOOTH;
        default:
            return MP::UNDEFINED;
    }
}

// { modified by eugeny.novikov 2012.10.25 for CR 14047
ETrackerAbstarctor_VolumeType
AudioUtils::VolumeType(MP::DEVICE_T internalType)
{
    switch (internalType)
    {
        case MP::JUKEBOX:
            return eJukeBox;
        case MP::USB1:
            return eUSB_FRONT;
        case MP::USB2:
            return eUSB_REAR;
        case MP::DISC:
            return eDisc;
        case MP::IPOD1:
            return eIPOD_FRONT;
        case MP::IPOD2:
            return eIPOD_REAR;
        default:
            return eDefaultVolume;
    }
}
// } modified by eugeny.novikov

MEDIA_SOURCE_T
AudioUtils::DeviceType(ETrackerAbstarctor_VolumeType externalType)
{
    switch (externalType)
    {
        case eJukeBox:
            return MEDIA_SOURCE_JUKEBOX;
        case eUSB_FRONT:
            return MEDIA_SOURCE_USB1;
        case eUSB_REAR:
            return MEDIA_SOURCE_USB2;
        case eDisc:
            return MEDIA_SOURCE_CDDA;
        case eIPOD_FRONT:
            return MEDIA_SOURCE_IPOD1;
        case eIPOD_REAR:
            return MEDIA_SOURCE_IPOD2;
        default:
            return MEDIA_SOURCE_MAX;
    }
}

// { added by eugeny.novikov 2012.10.25 for CR 14047
MP::DEVICE_T
AudioUtils::DeviceByVolume(ETrackerAbstarctor_VolumeType externalType)
{
    switch (externalType)
    {
        case eJukeBox:
            return MP::JUKEBOX;
        case eUSB_FRONT:
            return MP::USB1;
        case eUSB_REAR:
            return MP::USB2;
        case eDisc:
            return MP::DISC;
        case eIPOD_FRONT:
            return MP::IPOD1;
        case eIPOD_REAR:
            return MP::IPOD2;
        default:
            return MP::UNDEFINED;
    }
}
// } added by eugeny.novikov

QString AudioUtils::GetFrontUSBPath()
{
    if(m_strUSB1Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSB1Path = "/rw_data/media/front_usb";
        }
        else {
            m_strUSB1Path = "/media/front_usb";
        }
    }
    return m_strUSB1Path;
}

QString AudioUtils::GetRearUSBPath()
{
    if(m_strUSB2Path.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSB2Path = "/rw_data/media/rear_usb";
        }
        else {
            m_strUSB2Path = "/media/rear_usb";
        }
    }
    return m_strUSB2Path;
}

QString AudioUtils::GetUSBRoot()
{
    if(m_strUSBRoot.isEmpty()) {
        QFileInfo tFileInfo("/rw_data");
        if(tFileInfo.exists()) {
            m_strUSBRoot = "/rw_data";
        }
        else {
            m_strUSBRoot = "/media";
        }
    }
    return m_strUSBRoot;
}
#if 0
// these functions, moved to TA by jaehwan 2014.01.08
//{ added by jaehwan 2013.10.16 for translation from latin extended char to alphabet char
wchar_t LATIN_EXTEND_ALPAH_TAB [] = {
'A',  //U+00C0
'A',  //U+00C1
'A',  //U+00C2
'A',  //U+00C3
'A',  //U+00C4
'A',  //U+00C5
'A',  //U+00C6 //modified by jaehwan 2013.11.18
'C',  //U+00C7
'E',  //U+00C8
'E',  //U+00C9
'E',  //U+00CA
'E',  //U+00CB
'I',  //U+00CC
'I',  //U+00CD
'I',  //U+00CE
'I',  //U+00CF
' ',  //U+00D0
'N',  //U+00D1
'O',  //U+00D2
'O',  //U+00D3
'O',  //U+00D4
'O',  //U+00D5
'O',  //U+00D6
' ',  //U+00D7
'O',  //U+00D8
'U',  //U+00D9
'U',  //U+00DA
'U',  //U+00DB
'U',  //U+00DC
'Y',  //U+00DD
' ',  //U+00DE
'S',  //U+00DF
'A',  //U+00E0
'A',  //U+00E1
'A',  //U+00E2
'A',  //U+00E3
'A',  //U+00E4
'A',  //U+00E5
' ',  //U+00E6
'C',  //U+00E7
'E',  //U+00E8
'E',  //U+00E9
'E',  //U+00EA
'E',  //U+00EB
'I',  //U+00EC
'I',  //U+00ED
'I',  //U+00EE
'I',  //U+00EF
' ',  //U+00F0
'N',  //U+00F1
'O',  //U+00F2
'O',  //U+00F3
'O',  //U+00F4
'O',  //U+00F5
'O',  //U+00F6
' ',  //U+00F7
'O',  //U+00F8
'U',  //U+00F9
'U',  //U+00FA
'U',  //U+00FB
'U',  //U+00FC
'Y',  //U+00FD
' ',  //U+00FE
'Y',  //U+00FF
'A',  //U+0100
'A',  //U+0101
'A',  //U+0102
'A',  //U+0103
'A',  //U+0104
'A',  //U+0105
'C',  //U+0106
'C',  //U+0107
'C',  //U+0108
'C',  //U+0109
'C',  //U+010A
'C',  //U+010B
'C',  //U+010C
'C',  //U+010D
'D',  //U+010E
'D',  //U+010F
'D',  //U+0110
'D',  //U+0111
'E',  //U+0112
'E',  //U+0113
'E',  //U+0114
'E',  //U+0115
'E',  //U+0116
'E',  //U+0117
'E',  //U+0118
'E',  //U+0119
'E',  //U+011A
'E',  //U+011B
'G',  //U+011C
'G',  //U+011D
'G',  //U+011E
'G',  //U+011F
'G',  //U+0120
'G',  //U+0121
'G',  //U+0122
'G',  //U+0123
'H',  //U+0124
'H',  //U+0125
'H',  //U+0126
'H',  //U+0127
'I',  //U+0128
'I',  //U+0129
'I',  //U+012A
'I',  //U+012B
'I',  //U+012C
'I',  //U+012D
'I',  //U+012E
'I',  //U+012F
'I',  //U+0130
'I',  //U+0131
' ',  //U+0132
' ',  //U+0133
'J',  //U+0134
'J',  //U+0135
'K',  //U+0136
'K',  //U+0137
' ',  //U+0138
'L',  //U+0139
'L',  //U+013A
'L',  //U+013B
'L',  //U+013C
'L',  //U+013D
'L',  //U+013E
'L',  //U+013F
'L',  //U+0140
'L',  //U+0141
'L',  //U+0142
'N',  //U+0143
'N',  //U+0144
'N',  //U+0145
'N',  //U+0146
'N',  //U+0147
'N',  //U+0148
'N',  //U+0149
' ',  //U+014A
' ',  //U+014B
'O',  //U+014C
'O',  //U+014D
'O',  //U+014E
'O',  //U+014F
'O',  //U+0150
'O',  //U+0151
' ',  //U+0152
' ',  //U+0153
'R',  //U+0154
'R',  //U+0155
'R',  //U+0156
'R',  //U+0157
'R',  //U+0158
'R',  //U+0159
'S',  //U+015A
'S',  //U+015B
'S',  //U+015C
'S',  //U+015D
'S',  //U+015E
'S',  //U+015F
'S',  //U+0160
'S',  //U+0161
'T',  //U+0162
'T',  //U+0163
'T',  //U+0164
'T',  //U+0165
'T',  //U+0166
'T',  //U+0167
'U',  //U+0168
'U',  //U+0169
'U',  //U+016A
'U',  //U+016B
'U',  //U+016C
'U',  //U+016D
'U',  //U+016E
'U',  //U+016F
'U',  //U+0170
'U',  //U+0171
'U',  //U+0172
'U',  //U+0173
'W',  //U+0174
'W',  //U+0175
'Y',  //U+0176
'Y',  //U+0177
'Y',  //U+0178
'Z',  //U+0179
'Z',  //U+017A
'Z',  //U+017B
'Z',  //U+017C
'Z',  //U+017D
'Z',  //U+017E
'S',  //U+017F
'B',  //U+0180
'B',  //U+0181
'B',  //U+0182
'B',  //U+0183
' ',  //U+0184
' ',  //U+0185
'O',  //U+0186
'C',  //U+0187
'C',  //U+0188
'D',  //U+0189
'D',  //U+018A
'D',  //U+018B
'D',  //U+018C
' ',  //U+018D
'E',  //U+018E
' ',  //U+018F
'E',  //U+0190
'F',  //U+0191
'F',  //U+0192
'G',  //U+0193
'G',  //U+0194
' ',  //U+0195
' ',  //U+0196
'I',  //U+0197
'K',  //U+0198
'K',  //U+0199
'L',  //U+019A
'L',  //U+019B
'M',  //U+019C
'N',  //U+019D
'N',  //U+019E
'O',  //U+019F
'O',  //U+01A0
'O',  //U+01A1
' ',  //U+01A2
' ',  //U+01A3
'P',  //U+01A4
'P',  //U+01A5
' ',  //U+01A6
' ',  //U+01A7
' ',  //U+01A8
' ',  //U+01A9
' ',  //U+01AA
'T',  //U+01AB
'T',  //U+01AC
'T',  //U+01AD
'T',  //U+01AE
'U',  //U+01AF
'U',  //U+01B0
' ',  //U+01B1
'V',  //U+01B2
'Y',  //U+01B3
'Y',  //U+01B4
'Z',  //U+01B5
'Z',  //U+01B6
' ',  //U+01B7
' ',  //U+01B8
' ',  //U+01B9
' ',  //U+01BA
' ',  //U+01BB
' ',  //U+01BC
' ',  //U+01BD
' ',  //U+01BE
' ',  //U+01BF
' ',  //U+01C0
' ',  //U+01C1
' ',  //U+01C2
' ',  //U+01C3
' ',  //U+01C4
' ',  //U+01C5
' ',  //U+01C6
' ',  //U+01C7
' ',  //U+01C8
' ',  //U+01C9
' ',  //U+01CA
' ',  //U+01CB
' ',  //U+01CC
'A',  //U+01CD
'A',  //U+01CE
'I',  //U+01CF
'I',  //U+01D0
'O',  //U+01D1
'O',  //U+01D2
'U',  //U+01D3
'U',  //U+01D4
'U',  //U+01D5
'U',  //U+01D6
'U',  //U+01D7
'U',  //U+01D8
'U',  //U+01D9
'U',  //U+01DA
'U',  //U+01DB
'U',  //U+01DC
'E',  //U+01DD
'A',  //U+01DE
'A',  //U+01DF
'A',  //U+01E0
'A',  //U+01E1
' ',  //U+01E2
' ',  //U+01E3
'G',  //U+01E4
'G',  //U+01E5
'G',  //U+01E6
'G',  //U+01E7
'K',  //U+01E8
'K',  //U+01E9
'O',  //U+01EA
'O',  //U+01EB
'O',  //U+01EC
'O',  //U+01ED
' ',  //U+01EE
' ',  //U+01EF
'J',  //U+01F0
' ',  //U+01F1
' ',  //U+01F2
' ',  //U+01F3
'G',  //U+01F4
'G',  //U+01F5
' ',  //U+01F6
' ',  //U+01F7
'N',  //U+01F8
'N',  //U+01F9
'A',  //U+01FA
'A',  //U+01FB
' ',  //U+01FC
' ',  //U+01FD
'O',  //U+01FE
'O',  //U+01FF
'A',  //U+0200
'A',  //U+0201
'A',  //U+0202
'A',  //U+0203
'E',  //U+0204
'E',  //U+0205
'E',  //U+0206
'E',  //U+0207
'I',  //U+0208
'I',  //U+0209
'I',  //U+020A
'I',  //U+020B
'O',  //U+020C
'O',  //U+020D
'O',  //U+020E
'O',  //U+020F
'R',  //U+0210
'R',  //U+0211
'R',  //U+0212
'R',  //U+0213
'U',  //U+0214
'U',  //U+0215
'U',  //U+0216
'U',  //U+0217
'S',  //U+0218
'S',  //U+0219
'T',  //U+021A
'T',  //U+021B
' ',  //U+021C
' ',  //U+021D
'H',  //U+021E
'H',  //U+021F
'N',  //U+0220
'D',  //U+0221
' ',  //U+0222
' ',  //U+0223
'Z',  //U+0224
'Z',  //U+0225
'A',  //U+0226
'A',  //U+0227
'E',  //U+0228
'E',  //U+0229
'O',  //U+022A
'O',  //U+022B
'O',  //U+022C
'O',  //U+022D
'O',  //U+022E
'O',  //U+022F
'O',  //U+0230
'O',  //U+0231
'Y',  //U+0232
'Y',  //U+0233
'L',  //U+0234
'N',  //U+0235
'T',  //U+0236
'J',  //U+0237
' ',  //U+0238
'A',  //U+0239
'C',  //U+023A
'C',  //U+023B
'C',  //U+023C
'L',  //U+023D
'T',  //U+023E
'S',  //U+023F
'Z',  //U+0240
' ',  //U+0241
' ',  //U+0242
'B',  //U+0243
'U',  //U+0244
'V',  //U+0245
'E',  //U+0246
'E',  //U+0247
'J',  //U+0248
'J',  //U+0249
'Q',  //U+024A
'Q',  //U+024B
'R',  //U+024C
'R',  //U+024D
'Y',  //U+024E
'Y'   //U+024F
};
//}
//{ added by jaehwan.lee. table for translation hangul jamo to first char of grouping
wchar_t HANGUL_JAMO_TRANS_TAB[] = {
    0x0000,      //3130
    0x3131,      //3131
    0x3131,      //3132
    0x3131,      //3133
    0x3134,      //3134
    0x3134,      //3135
    0x3134,      //3136
    0x3137,      //3137
    0x3137,      //3138
    0x3139,      //3139
    0x3139,      //313A
    0x3139,      //313B
    0x3139,      //313C
    0x3139,      //313D
    0x3139,      //313E
    0x3139,      //313F
    0x3139,      //3140
    0x3141,      //3141
    0x3142,      //3142
    0x3142,      //3143
    0x3142,      //3144
    0x3145,      //3145
    0x3145,      //3146
    0x3147,      //3147
    0x3148,      //3148
    0x3148,      //3149
    0x314A,      //314A
    0x314B,      //314B
    0x314C,      //314C
    0x314D,      //314D
    0x314E,      //314E
    0x3147,      //314F
    0x3147,      //3150
    0x3147,      //3151
    0x3147,      //3152
    0x3147,      //3153
    0x3147,      //3154
    0x3147,      //3155
    0x3147,      //3156
    0x3147,      //3157
    0x3147,      //3158
    0x3147,      //3159
    0x3147,      //315A
    0x3147,      //315B
    0x3147,      //315C
    0x3147,      //315D
    0x3147,      //315E
    0x3147,      //315F
    0x3147,      //3160
    0x3147,      //3161
    0x3147,      //3162
    0x3147,      //3163
    0x0000,      //3164
    0x3134,      //3165
    0x3134,      //3166
    0x3134,      //3167
    0x3134,      //3168
    0x3139,      //3169
    0x3139,      //316A
    0x3139,      //316B
    0x3139,      //316C
    0x3139,      //316D
    0x3141,      //316E
    0x3141,      //316F
    0x3141,      //3170
    0x3141,      //3171
    0x3142,      //3172
    0x3142,      //3173
    0x3142,      //3174
    0x3142,      //3175
    0x3142,      //3176
    0x3142,      //3177
    0x3142,      //3178
    0x3142,      //3179
    0x3145,      //317A
    0x3145,      //317B
    0x3145,      //317C
    0x3145,      //317D
    0x3145,      //317E
    0x3145,      //317F
    0x3147,      //3180
    0x3147,      //3181
    0x3147,      //3182
    0x3147,      //3183
    0x314D,      //3184
    0x314E,      //3185
    0x314E,      //3186
    0x3147,      //3187
    0x3147,      //3188
    0x3147,      //3189
    0x3147,      //318A
    0x3147,      //318B
    0x3147,      //318C
    0x3147,      //318D
    0x3147,      //318E
    0x0000       //318F
};
//}

wchar_t getAlphabetFromLatinExtend(int ucode)
{
    int start_index = 0x00C0;

    if (ucode < 0x00C0 || ucode > 0x024F )
        return wchar_t(' ');

    return LATIN_EXTEND_ALPAH_TAB[ucode - start_index];
}

wchar_t getFirstCharFromHangulJamo(int ucode)
{
   int start_index = 0x3130;

    if (ucode < 0x3130 || ucode > 0x318F ) //modified from "ucode > 0x3190" to "ucode > 0x318F" by honggi.shin 2013.10.25, "0x3190" can make out-of-bound error. catched by static test.
        return wchar_t(0x0000);

    return HANGUL_JAMO_TRANS_TAB[ucode - start_index];
}

//{added by junam 2013.06.19 for iPod sorted first char
wchar_t AudioUtils::getFirstChar(QString title, bool bIPodSort)
{
    static const wchar_t koreanTable[19] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141,
                                               0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149,
                                               0x314A, 0x314B, 0x314C, 0x314D, 0x314E };
    static const QString exceptionWords[]={"a", "an", "the"};
    static const QChar exceptionChars[]={'\''};

    title = title.trimmed();

    bool isKorean = true;
    int nKeyCode;
    bool bExceptionWordFound = false;

    while (!title.isEmpty())
    {
        if(title.at(0).isSpace()
                || title.at(0) == 0xAD) //added by junam 2013.12.03 for arabic
        {
            title.remove(0,1);
            continue;
        }

        if(bIPodSort)
        {
            int idx = 0;

            for( idx = 0; idx < int(sizeof(exceptionChars)/sizeof(QChar)); idx++)
            {
                if(title.at(0)== exceptionChars[idx])
                {
                    title.remove(0,1);
                    break;
                }
            }
            if( idx < int(sizeof(exceptionChars)/sizeof(QChar)) )
            {
                continue;
            }

            if(!bExceptionWordFound)
            {
                for( idx = 0; idx < int(sizeof(exceptionWords)/sizeof(QString)); idx++)
                {
                    if(exceptionWords[idx].size() < title.size() && title.startsWith(exceptionWords[idx], Qt::CaseInsensitive))
                    {
                        if(title.at(exceptionWords[idx].size()).isSpace())
                        {
                            title.remove(0,exceptionWords[idx].size());
                            bExceptionWordFound = true;
                            break;
                        }
                    }
                }
                if( idx < int(sizeof(exceptionWords)/sizeof(QString)) )
                {
                    continue;
                }
            }
        }
        break;
    }

    if( title.isEmpty() )
        nKeyCode = QChar(' ').unicode();
    else
        nKeyCode = title.at(0).unicode();

    int UniValue = 0;

    if( nKeyCode >= 0x3130 && nKeyCode <= 0x318F)
    {
        if(!bIPodSort) {
           wchar_t firstChar = getFirstCharFromHangulJamo(nKeyCode);
           if (firstChar == 0x0000)
               return wchar_t(nKeyCode);
           else
               return firstChar;
        }
        UniValue = nKeyCode - 0x3130;
    }
    else if(nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF )
    {
        UniValue = nKeyCode - 0xAC00;
    }
    else
    {
        UniValue = nKeyCode;
        isKorean = false;
    }

    if( isKorean )
    {
        int final_con = UniValue % 28;
        int initial_con = ( ( UniValue - final_con ) / 28 ) / 21;

        if (initial_con == 1 || initial_con == 4 || initial_con == 8 ||
                initial_con == 10 || initial_con == 13)
        {
            initial_con--;
        }

        if(initial_con < 19)
        {
            return koreanTable[initial_con];
        }
    }
    else if ( ( nKeyCode >= 0x0041 && nKeyCode <= 0x005A ) //alphabet uppercase
             || ( nKeyCode >= 0x0600 && nKeyCode <= 0x06FF )  // arabic
             )
    {
        return wchar_t(nKeyCode);
    }
    else if ( nKeyCode >= 0x0061 && nKeyCode <= 0x007A ) // alphabet lowercase
    {
        return wchar_t(nKeyCode - ('a'-'A')); // change to upper case  modified jaehwan. 2013.11.12
    }
    else
    {
        // { added by jaehwan to fix ITS 208215 ( symbol/special character grouping)
        if (!bIPodSort)
        {
            if ( (nKeyCode >= 0x0000 && nKeyCode <= 0x001F) /* control characters */ //added by jaehwan 2013.11.18
                || (nKeyCode >= 0x00A0 && nKeyCode <= 0x00B4) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode >= 0x00B6 && nKeyCode <= 0x00B9) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode >= 0x00BB && nKeyCode <= 0x00BF) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode == 0x00D7 || nKeyCode == 0x00F7) /* symbol & puntuation (latin1 supplement) */
                //|| (nKeyCode >= 0x01C0 && nKeyCode <= 0x01C3) /* symbol & puntuation (latin extended B)*/ /* removed by jaehwan 2013.11.14 */
                || (nKeyCode >= 0x02B9 && nKeyCode <= 0x02DF) /* Spacing Modifier Letters */
                || (nKeyCode >= 0x02E4 && nKeyCode <= 0x02FF) /* Spacing Modifier Letters */
                || (nKeyCode >= 0x0300 && nKeyCode <= 0x0362) /* Combining Diacritical Marks  */  /* modified by jaehwan 2013.11.14 */
                || (nKeyCode == 0x0374 || nKeyCode == 0x0375  /* Greek and Coptic */
                    || nKeyCode == 0x037E || nKeyCode == 0x0384
                    || nKeyCode == 0x0385 || nKeyCode == 0x0387)
                || (nKeyCode >= 0x0482 && nKeyCode <= 0x0489) /* Cyrillic */
                || (nKeyCode >= 0x0590 && nKeyCode <= 0x05CF) /* Hebrew */
                || (nKeyCode >= 0x2000 && nKeyCode <= 0x205E) /* General Punctuation */
                || (nKeyCode >= 0x2070 && nKeyCode <= 0x209F) /* Superscripts and subscripts */
                || (nKeyCode >= 0x20A0 && nKeyCode <= 0x20CF) /* Currency symbolx */
                || (nKeyCode >= 0x2100 && nKeyCode <= 0x214F) /* Letterlike Symbols */
                || (nKeyCode >= 0x2150 && nKeyCode <= 0x218F) /* Number Forms */
                || (nKeyCode >= 0x2190 && nKeyCode <= 0x21FF) /* Arrows */
                || (nKeyCode >= 0x2200 && nKeyCode <= 0x22FF) /* Mathematical Operators */
                || (nKeyCode >= 0x2300 && nKeyCode <= 0x23FF) /* Miscellaneous Technical */
                || (nKeyCode >= 0x2460 && nKeyCode <= 0x24FF) /* Enclosed Alphanumerics */
                || (nKeyCode >= 0x2500 && nKeyCode <= 0x257F) /* Box Drawing */
                || (nKeyCode >= 0x25A0 && nKeyCode <= 0x25FF) /* Geometric Shapes */
                || (nKeyCode >= 0x2600 && nKeyCode <= 0x26FF) /* MIscellaneous Symbols */
                || (nKeyCode >= 0x2700 && nKeyCode <= 0x27BF) /* Dingbats */
                || (nKeyCode >= 0x3000 && nKeyCode <= 0x303F) /* CJK Symbols and Punctuation */
                || (nKeyCode >= 0x3200 && nKeyCode <= 0x32FF) /* Enclosed CJK Letters and Months */
                || (nKeyCode >= 0x3372 && nKeyCode <= 0x33FF) /* CJK Compatibility */
            )
            {
                return wchar_t('#');
            }
        }
        //} added by jaehwan
        if ( (nKeyCode >= 0x0021 && nKeyCode <= 0x0040)
                 || (nKeyCode >= 0x005B && nKeyCode <= 0x0060)
                 || (nKeyCode >= 0x007B && nKeyCode <= 0x007E)
                 )
        {
            return wchar_t('#');
        }
        else if ( nKeyCode >= 0x00C0 && nKeyCode <= 0x024F ) // Latin Supplement, Extented..
        {
            wchar_t key = getAlphabetFromLatinExtend(nKeyCode);
            if (key == wchar_t(' '))
                return wchar_t(nKeyCode);
            else
                return key;
        }
        else if ( nKeyCode == 0x00BA)
        {
            return wchar_t('O');
        }
        else
        {
            return wchar_t(nKeyCode);
        }
    }

    return 0;
}
#endif

#ifdef AVP_LAST_AUDIOFILE_BACKUP
void AudioUtils::backupFile(QString filePath, qint64 limitedFileSize)
{
    if(filePath == "")
        return;

    QFileInfo tSourceFileInfo(filePath);
    qint64 sourceFileSize = tSourceFileInfo.size();
    QString targetPath = "/common_data/";
    QString targetFileName = tSourceFileInfo.completeBaseName() + ".debug";

    QDir tTargetDir(targetPath);

    if(!tTargetDir.exists()) // target directory must exist!!!
        return;

    tTargetDir.setSorting(QDir::Name | QDir::IgnoreCase);
    tTargetDir.setFilter(QDir::Files);

    if(sourceFileSize != 0 && sourceFileSize <= limitedFileSize) // check file exist & file size
    {
        foreach(QFileInfo fileInfo, tTargetDir.entryInfoList((QStringList() << "*.debug")))    // delete old ".debug" file
        {
            QFile::remove(fileInfo.filePath());
        }

        QFile::copy(filePath, targetPath + targetFileName);
    }
}
#endif
