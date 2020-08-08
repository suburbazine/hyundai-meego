/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_AppPandoraBase.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_AppPandoraBase.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_AppPandoraBase.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_POPUPEnums[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
      11, 0x0,   29,   18,

 // enum data: key, value
      22, uint(POPUPEnums::POPUP_PANDORA_UNDEFINED),
      46, uint(POPUPEnums::POPUP_PANDORA_CALLING_STATE),
      74, uint(POPUPEnums::POPUP_PANDORA_CANNOT_BE_MANIPULATED),
     110, uint(POPUPEnums::POPUP_PANDORA_DRIVING_RESTRICTION),
     144, uint(POPUPEnums::POPUP_PANDORA_INCOMPATIABLE_VERSION),
     180, uint(POPUPEnums::POPUP_PANDORA_LOG_IN),
     201, uint(POPUPEnums::POPUP_PANDORA_MAINTENANCE),
     227, uint(POPUPEnums::POPUP_PANDORA_MAX_SKIP_LIMIT),
     256, uint(POPUPEnums::POPUP_PANDORA_REWIND_NOT_AVAILABLE),
     291, uint(POPUPEnums::POPUP_PANDORA_SEARCH_SELECT_FAILED),
     326, uint(POPUPEnums::POPUP_PANDORA_STATION_DOES_NOT_EXIST),
     363, uint(POPUPEnums::POPUP_PANDORA_STATION_LIST_IS_FULL),
     398, uint(POPUPEnums::POPUP_RECEIVING_STATIONS),
     423, uint(POPUPEnums::POPUP_ARGUMENT_PRESSED_INVALID),
     454, uint(POPUPEnums::POPUP_BOOKMARK_FAILED),
     476, uint(POPUPEnums::POPUP_DELETION_STATION_FAILED),
     506, uint(POPUPEnums::POPUP_EMPTY_INPUT_CRITERIA),
     533, uint(POPUPEnums::POPUP_EXTENDED_SEARCH_FAILED),
     562, uint(POPUPEnums::POPUP_TOKEN_STATION_NOT_VALID),
     592, uint(POPUPEnums::POPUP_INITALIZATION_FAILED),
     619, uint(POPUPEnums::POPUP_NO_PHONE_CONNECTED),
     644, uint(POPUPEnums::POPUP_PANDORA_DISCONNECTED),
     671, uint(POPUPEnums::POPUP_REQUESTED_STATION_RANGE_NOT_VALID),
     711, uint(POPUPEnums::POPUP_REQUIRED_MEMORY_NOT_ALLOCATED),
     747, uint(POPUPEnums::POPUP_SESSION_ALREADY_EXIT),
     774, uint(POPUPEnums::POPUP_TIMEOUT_ERROR),
     794, uint(POPUPEnums::POPUP_TRACK_EXPLAIN_REQUEST_FAILDE),
     829, uint(POPUPEnums::POPUP_TRACK_RATING_FAILED),
     855, uint(POPUPEnums::POPUP_URL_NOT_SUPPORTED),

       0        // eod
};

static const char qt_meta_stringdata_POPUPEnums[] = {
    "POPUPEnums\0PopupIDPnd\0POPUP_PANDORA_UNDEFINED\0"
    "POPUP_PANDORA_CALLING_STATE\0"
    "POPUP_PANDORA_CANNOT_BE_MANIPULATED\0"
    "POPUP_PANDORA_DRIVING_RESTRICTION\0"
    "POPUP_PANDORA_INCOMPATIABLE_VERSION\0"
    "POPUP_PANDORA_LOG_IN\0POPUP_PANDORA_MAINTENANCE\0"
    "POPUP_PANDORA_MAX_SKIP_LIMIT\0"
    "POPUP_PANDORA_REWIND_NOT_AVAILABLE\0"
    "POPUP_PANDORA_SEARCH_SELECT_FAILED\0"
    "POPUP_PANDORA_STATION_DOES_NOT_EXIST\0"
    "POPUP_PANDORA_STATION_LIST_IS_FULL\0"
    "POPUP_RECEIVING_STATIONS\0"
    "POPUP_ARGUMENT_PRESSED_INVALID\0"
    "POPUP_BOOKMARK_FAILED\0"
    "POPUP_DELETION_STATION_FAILED\0"
    "POPUP_EMPTY_INPUT_CRITERIA\0"
    "POPUP_EXTENDED_SEARCH_FAILED\0"
    "POPUP_TOKEN_STATION_NOT_VALID\0"
    "POPUP_INITALIZATION_FAILED\0"
    "POPUP_NO_PHONE_CONNECTED\0"
    "POPUP_PANDORA_DISCONNECTED\0"
    "POPUP_REQUESTED_STATION_RANGE_NOT_VALID\0"
    "POPUP_REQUIRED_MEMORY_NOT_ALLOCATED\0"
    "POPUP_SESSION_ALREADY_EXIT\0"
    "POPUP_TIMEOUT_ERROR\0"
    "POPUP_TRACK_EXPLAIN_REQUEST_FAILDE\0"
    "POPUP_TRACK_RATING_FAILED\0"
    "POPUP_URL_NOT_SUPPORTED\0"
};

void POPUPEnums::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData POPUPEnums::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject POPUPEnums::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_POPUPEnums,
      qt_meta_data_POPUPEnums, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &POPUPEnums::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *POPUPEnums::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *POPUPEnums::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_POPUPEnums))
        return static_cast<void*>(const_cast< POPUPEnums*>(this));
    return QObject::qt_metacast(_clname);
}

int POPUPEnums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CAppPandoraBase[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      88,   14, // methods
       3,  454, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      30,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      28,   16,   16,   16, 0x05,
      55,   44,   16,   16, 0x05,
      86,   74,   16,   16, 0x05,
     115,   16,   16,   16, 0x05,
     128,   16,   16,   16, 0x05,
     141,   16,   16,   16, 0x05,
     165,  156,   16,   16, 0x05,
     196,  187,   16,   16, 0x05,
     215,   16,   16,   16, 0x05,
     248,  235,   16,   16, 0x05,
     279,  269,   16,   16, 0x05,
     312,  300,   16,   16, 0x05,
     343,  333,   16,   16, 0x05,
     366,  333,   16,   16, 0x05,
     391,   16,   16,   16, 0x05,
     414,   16,   16,   16, 0x05,
     434,   16,   16,   16, 0x05,
     460,  450,   16,   16, 0x05,
     480,   16,   16,   16, 0x05,
     500,   16,   16,   16, 0x05,
     521,   16,   16,   16, 0x05,
     549,  531,   16,   16, 0x05,
     569,   16,   16,   16, 0x05,
     609,  594,   16,   16, 0x05,
     650,  628,   16,   16, 0x05,
     674,   16,   16,   16, 0x05,
     689,   16,   16,   16, 0x05,
     722,  714,   16,   16, 0x05,
     739,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     752,   16,   16,   16, 0x0a,
     770,  763,   16,   16, 0x0a,
     802,  763,   16,   16, 0x0a,
     838,  763,   16,   16, 0x0a,
     876,   16,   16,   16, 0x0a,
     902,   16,   16,   16, 0x0a,
     919,   16,   16,   16, 0x0a,
     966,  940,   16,   16, 0x0a,
    1010,  940,   16,   16, 0x0a,
    1056,   16,   16,   16, 0x0a,
    1085, 1072,   16,   16, 0x0a,
    1117, 1109,   16,   16, 0x0a,
    1169,   16,   16,   16, 0x0a,
    1186,   16,   16,   16, 0x0a,
    1206,   16,   16,   16, 0x0a,
    1222,   16,   16,   16, 0x0a,
    1240,  187,   16,   16, 0x0a,
    1270,   16, 1262,   16, 0x0a,
    1299, 1291,   16,   16, 0x0a,
    1323,   16, 1318,   16, 0x0a,
    1341, 1337,   16,   16, 0x0a,
    1375, 1362,   16,   16, 0x0a,

 // methods: signature, parameters, type, tag, flags
    1405,   16, 1318,   16, 0x02,
    1422,   16, 1418,   16, 0x02,
    1438,  269,   16,   16, 0x02,
    1460,   16,   16,   16, 0x02,
    1476,  269,   16,   16, 0x02,
    1496,   16,   16,   16, 0x02,
    1506,   16,   16,   16, 0x02,
    1519,   16,   16,   16, 0x02,
    1538,   16,   16,   16, 0x02,
    1562,   16,   16,   16, 0x02,
    1583,  187,   16,   16, 0x02,
    1612,   16, 1418,   16, 0x02,
    1641, 1631,   16,   16, 0x02,
    1672,   16, 1318,   16, 0x02,
    1695,   16,   16,   16, 0x02,
    1714, 1631,   16,   16, 0x02,
    1744,   16, 1318,   16, 0x02,
    1769,   16, 1318,   16, 0x02,
    1821, 1789, 1418,   16, 0x02,
    1857,   16,   16,   16, 0x02,
    1883,   16,   16,   16, 0x02,
    1906,   16, 1418,   16, 0x02,
    1926,   16, 1318,   16, 0x02,
    1946,  187,   16,   16, 0x02,
    1977,   16, 1318,   16, 0x02,
    1995,   16, 1318,   16, 0x02,
    2011,   16, 1318,   16, 0x02,
    2023,   16, 1318,   16, 0x02,
    2044,   16,   16,   16, 0x02,
    2067,   16,   16,   16, 0x02,
    2099, 2093,   16,   16, 0x02,
    2139,   16, 2126,   16, 0x02,
    2165,   16, 2126,   16, 0x02,
    2191,   16,   16,   16, 0x02,
    2212, 2206,   16,   16, 0x02,
    2232,   16, 1418,   16, 0x02,

 // properties: name, type, flags
    2248, 1318, 0x01495001,
    2264, 1418, 0x02095002,
      44, 1418, 0x02095001,

 // properties: notify_signal_id
      23,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_CAppPandoraBase[] = {
    "CAppPandoraBase\0\0playInBG()\0deviceChanged()\0"
    "languageId\0retranslateUi(int)\0isConnected\0"
    "connectionStatusChanged(int)\0foreground()\0"
    "background()\0networkError()\0isJogKey\0"
    "handleSkipEvent(bool)\0inStatus\0"
    "skipKeyEvent(bool)\0handleRewindEvent()\0"
    "isJogMenuKey\0menuKeyPressed(bool)\0"
    "isJogDial\0backKeyPressed(bool)\0"
    "inPndrError\0handleError(quint16)\0"
    "inPayLoad\0playCommandFromVR(int)\0"
    "ratingCommandFromVR(int)\0"
    "shuffleCommandFromVR()\0deviceListUpdated()\0"
    "avModeChanged()\0isForward\0tuneKeyDialed(bool)\0"
    "tuneCenterPressed()\0tuneCenterReleased()\0"
    "tuneOff()\0inScrollingTicker\0"
    "tickerChanged(bool)\0scrollingTickerChanged()\0"
    "incallingState\0callingState(bool)\0"
    "isInsufficientNetwork\0networkErrorState(bool)\0"
    "callingPopup()\0skipNextLongKeyPressed()\0"
    "popupID\0showPopupID(int)\0popupClose()\0"
    "Notified()\0aEvent\0NotifyFromUIStateManager(Event)\0"
    "NotifyFromUIStateManagerRear(Event)\0"
    "NotifyFromUIStateManagerCommon(Event)\0"
    "onSessionTerminateAcked()\0NotifyAVWidget()\0"
    "StopNotifyAVWidget()\0inRequestType,inPndrError\0"
    "OnRequestCompleted(ERequestType,EPndrError)\0"
    "OnBTRequestCompleted(ERequestType,EPndrError)\0"
    "OnNextTimeOSD()\0inTrackToken\0"
    "OnTrackUpdated(quint32)\0tAM,tAS\0"
    "OnRPMAcquireResponseReceived(TAudioMode,TAudioSink)\0"
    "OnSyncWithIPod()\0onTuneSelectTimer()\0"
    "onNoSkipTimer()\0onPlayMuteTimer()\0"
    "TuneSelectTimer(bool)\0QString\0"
    "getConnectTypeName()\0inState\0"
    "SetPlayState(bool)\0bool\0IsBTPandora()\0"
    "str\0printQMLLog(QString)\0inSampleRate\0"
    "onAudioSampleRateChanged(int)\0"
    "isFrontLCD()\0int\0getlanguageId()\0"
    "ExitApplication(bool)\0HandleHomeKey()\0"
    "HandleBackKey(bool)\0Connect()\0"
    "Disconnect()\0SessionTerminate()\0"
    "SetPandoraStateActive()\0LaunchSoundSetting()\0"
    "SetOptionMenuVisibilty(bool)\0"
    "getCurrentScreen()\0inVisible\0"
    "SetSystemPopupVisibility(bool)\0"
    "IsSystemPopupVisible()\0CloseSystemPopup()\0"
    "SetLocalPopupVisibility(bool)\0"
    "IsLocalPopupVisibility()\0isPopupVisibility()\0"
    "fullString,fontFamily,pixelSize\0"
    "getStringWidth(QString,QString,int)\0"
    "sendTouchCleanUpForApps()\0"
    "LaunchKeypadSettings()\0GetCountryVariant()\0"
    "GetVariantRearUSB()\0SetLoadingPopupVisibilty(bool)\0"
    "IsAVModeChanged()\0IsRunningInBG()\0"
    "IsCalling()\0getScrollingTicker()\0"
    "IfMute_SentAudioPass()\0SendTouchCleanUpForApps()\0"
    "bPlay\0SetShareDataProvider(bool)\0"
    "MODE_STATE_T\0getModeStateHistoryPrev()\0"
    "getModeStateHistoryCurr()\0TrackInfoOSD()\0"
    "index\0SetSeekStatus(bool)\0IsVehicleType()\0"
    "scrollingTicker\0currView\0"
};

void CAppPandoraBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAppPandoraBase *_t = static_cast<CAppPandoraBase *>(_o);
        switch (_id) {
        case 0: _t->playInBG(); break;
        case 1: _t->deviceChanged(); break;
        case 2: _t->retranslateUi((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->connectionStatusChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 4: _t->foreground(); break;
        case 5: _t->background(); break;
        case 6: _t->networkError(); break;
        case 7: _t->handleSkipEvent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->skipKeyEvent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->handleRewindEvent(); break;
        case 10: _t->menuKeyPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->backKeyPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->handleError((*reinterpret_cast< const quint16(*)>(_a[1]))); break;
        case 13: _t->playCommandFromVR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->ratingCommandFromVR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->shuffleCommandFromVR(); break;
        case 16: _t->deviceListUpdated(); break;
        case 17: _t->avModeChanged(); break;
        case 18: _t->tuneKeyDialed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->tuneCenterPressed(); break;
        case 20: _t->tuneCenterReleased(); break;
        case 21: _t->tuneOff(); break;
        case 22: _t->tickerChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: _t->scrollingTickerChanged(); break;
        case 24: _t->callingState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->networkErrorState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->callingPopup(); break;
        case 27: _t->skipNextLongKeyPressed(); break;
        case 28: _t->showPopupID((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->popupClose(); break;
        case 30: _t->Notified(); break;
        case 31: _t->NotifyFromUIStateManager((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 32: _t->NotifyFromUIStateManagerRear((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 33: _t->NotifyFromUIStateManagerCommon((*reinterpret_cast< Event(*)>(_a[1]))); break;
        case 34: _t->onSessionTerminateAcked(); break;
        case 35: _t->NotifyAVWidget(); break;
        case 36: _t->StopNotifyAVWidget(); break;
        case 37: _t->OnRequestCompleted((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EPndrError(*)>(_a[2]))); break;
        case 38: _t->OnBTRequestCompleted((*reinterpret_cast< const ERequestType(*)>(_a[1])),(*reinterpret_cast< const EPndrError(*)>(_a[2]))); break;
        case 39: _t->OnNextTimeOSD(); break;
        case 40: _t->OnTrackUpdated((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 41: _t->OnRPMAcquireResponseReceived((*reinterpret_cast< TAudioMode(*)>(_a[1])),(*reinterpret_cast< TAudioSink(*)>(_a[2]))); break;
        case 42: _t->OnSyncWithIPod(); break;
        case 43: _t->onTuneSelectTimer(); break;
        case 44: _t->onNoSkipTimer(); break;
        case 45: _t->onPlayMuteTimer(); break;
        case 46: _t->TuneSelectTimer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: { QString _r = _t->getConnectTypeName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 48: _t->SetPlayState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 49: { bool _r = _t->IsBTPandora();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 50: _t->printQMLLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 51: _t->onAudioSampleRateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 52: { bool _r = _t->isFrontLCD();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 53: { int _r = _t->getlanguageId();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 54: _t->ExitApplication((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->HandleHomeKey(); break;
        case 56: _t->HandleBackKey((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 57: _t->Connect(); break;
        case 58: _t->Disconnect(); break;
        case 59: _t->SessionTerminate(); break;
        case 60: _t->SetPandoraStateActive(); break;
        case 61: _t->LaunchSoundSetting(); break;
        case 62: _t->SetOptionMenuVisibilty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 63: { int _r = _t->getCurrentScreen();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 64: _t->SetSystemPopupVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 65: { bool _r = _t->IsSystemPopupVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 66: _t->CloseSystemPopup(); break;
        case 67: _t->SetLocalPopupVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 68: { bool _r = _t->IsLocalPopupVisibility();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 69: { bool _r = _t->isPopupVisibility();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 70: { int _r = _t->getStringWidth((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 71: _t->sendTouchCleanUpForApps(); break;
        case 72: _t->LaunchKeypadSettings(); break;
        case 73: { int _r = _t->GetCountryVariant();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 74: { bool _r = _t->GetVariantRearUSB();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 75: _t->SetLoadingPopupVisibilty((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 76: { bool _r = _t->IsAVModeChanged();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 77: { bool _r = _t->IsRunningInBG();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 78: { bool _r = _t->IsCalling();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 79: { bool _r = _t->getScrollingTicker();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 80: _t->IfMute_SentAudioPass(); break;
        case 81: _t->SendTouchCleanUpForApps(); break;
        case 82: _t->SetShareDataProvider((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 83: { MODE_STATE_T _r = _t->getModeStateHistoryPrev();
            if (_a[0]) *reinterpret_cast< MODE_STATE_T*>(_a[0]) = _r; }  break;
        case 84: { MODE_STATE_T _r = _t->getModeStateHistoryCurr();
            if (_a[0]) *reinterpret_cast< MODE_STATE_T*>(_a[0]) = _r; }  break;
        case 85: _t->TrackInfoOSD(); break;
        case 86: _t->SetSeekStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 87: { int _r = _t->IsVehicleType();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAppPandoraBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAppPandoraBase::staticMetaObject = {
    { &AppEngineQML::staticMetaObject, qt_meta_stringdata_CAppPandoraBase,
      qt_meta_data_CAppPandoraBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAppPandoraBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAppPandoraBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAppPandoraBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAppPandoraBase))
        return static_cast<void*>(const_cast< CAppPandoraBase*>(this));
    return AppEngineQML::qt_metacast(_clname);
}

int CAppPandoraBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AppEngineQML::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 88)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 88;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = getScrollingTicker(); break;
        case 2: *reinterpret_cast< int*>(_v) = getlanguageId(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setCurrentView(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CAppPandoraBase::playInBG()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CAppPandoraBase::deviceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CAppPandoraBase::retranslateUi(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CAppPandoraBase::connectionStatusChanged(const int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CAppPandoraBase::foreground()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CAppPandoraBase::background()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CAppPandoraBase::networkError()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void CAppPandoraBase::handleSkipEvent(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CAppPandoraBase::skipKeyEvent(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CAppPandoraBase::handleRewindEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void CAppPandoraBase::menuKeyPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CAppPandoraBase::backKeyPressed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CAppPandoraBase::handleError(const quint16 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CAppPandoraBase::playCommandFromVR(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void CAppPandoraBase::ratingCommandFromVR(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void CAppPandoraBase::shuffleCommandFromVR()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void CAppPandoraBase::deviceListUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void CAppPandoraBase::avModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void CAppPandoraBase::tuneKeyDialed(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void CAppPandoraBase::tuneCenterPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void CAppPandoraBase::tuneCenterReleased()
{
    QMetaObject::activate(this, &staticMetaObject, 20, 0);
}

// SIGNAL 21
void CAppPandoraBase::tuneOff()
{
    QMetaObject::activate(this, &staticMetaObject, 21, 0);
}

// SIGNAL 22
void CAppPandoraBase::tickerChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 22, _a);
}

// SIGNAL 23
void CAppPandoraBase::scrollingTickerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 23, 0);
}

// SIGNAL 24
void CAppPandoraBase::callingState(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 24, _a);
}

// SIGNAL 25
void CAppPandoraBase::networkErrorState(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 25, _a);
}

// SIGNAL 26
void CAppPandoraBase::callingPopup()
{
    QMetaObject::activate(this, &staticMetaObject, 26, 0);
}

// SIGNAL 27
void CAppPandoraBase::skipNextLongKeyPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 27, 0);
}

// SIGNAL 28
void CAppPandoraBase::showPopupID(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 28, _a);
}

// SIGNAL 29
void CAppPandoraBase::popupClose()
{
    QMetaObject::activate(this, &staticMetaObject, 29, 0);
}
static const uint qt_meta_data_LocalizeTrigger[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       1,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // properties: name, type, flags
      37,   29, 0x0a495001,

 // properties: notify_signal_id
       0,

       0        // eod
};

static const char qt_meta_stringdata_LocalizeTrigger[] = {
    "LocalizeTrigger\0\0retrigger()\0QString\0"
    "empty\0"
};

void LocalizeTrigger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LocalizeTrigger *_t = static_cast<LocalizeTrigger *>(_o);
        switch (_id) {
        case 0: _t->retrigger(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LocalizeTrigger::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LocalizeTrigger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LocalizeTrigger,
      qt_meta_data_LocalizeTrigger, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LocalizeTrigger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LocalizeTrigger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LocalizeTrigger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LocalizeTrigger))
        return static_cast<void*>(const_cast< LocalizeTrigger*>(this));
    return QObject::qt_metacast(_clname);
}

int LocalizeTrigger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = empty(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void LocalizeTrigger::retrigger()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
