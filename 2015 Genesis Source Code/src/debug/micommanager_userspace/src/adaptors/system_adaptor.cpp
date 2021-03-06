/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -a system_adaptor -c SystemAdaptor com.lge.car.micom.system.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "system_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class SystemAdaptor
 */

SystemAdaptor::SystemAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

SystemAdaptor::~SystemAdaptor()
{
    // destructor
}

void SystemAdaptor::LgeDBus_AVMInitialRequest()
{
    // handle method call com.lge.car.micom.system.LgeDBus_AVMInitialRequest
    QMetaObject::invokeMethod(parent(), "LgeDBus_AVMInitialRequest");
}

void SystemAdaptor::LgeDBus_AppBootComplete(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_AppBootComplete
    QMetaObject::invokeMethod(parent(), "LgeDBus_AppBootComplete", Q_ARG(uchar, in0));
}

QByteArray SystemAdaptor::LgeDBus_AskRTCInfo()
{
    // handle method call com.lge.car.micom.system.LgeDBus_AskRTCInfo
    QByteArray out0;
    QMetaObject::invokeMethod(parent(), "LgeDBus_AskRTCInfo", Q_RETURN_ARG(QByteArray, out0));
    return out0;
}

void SystemAdaptor::LgeDBus_CarInfoInitialRequest()
{
    // handle method call com.lge.car.micom.system.LgeDBus_CarInfoInitialRequest
    QMetaObject::invokeMethod(parent(), "LgeDBus_CarInfoInitialRequest");
}

void SystemAdaptor::LgeDBus_CarSetInitialRequest()
{
    // handle method call com.lge.car.micom.system.LgeDBus_CarSetInitialRequest
    QMetaObject::invokeMethod(parent(), "LgeDBus_CarSetInitialRequest");
}

void SystemAdaptor::LgeDBus_DATCInitialRequest()
{
    // handle method call com.lge.car.micom.system.LgeDBus_DATCInitialRequest
    QMetaObject::invokeMethod(parent(), "LgeDBus_DATCInitialRequest");
}

QByteArray SystemAdaptor::LgeDBus_EEPROMRead(const QByteArray &in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_EEPROMRead
    QByteArray out0;
    QMetaObject::invokeMethod(parent(), "LgeDBus_EEPROMRead", Q_RETURN_ARG(QByteArray, out0), Q_ARG(QByteArray, in0));
    return out0;
}

void SystemAdaptor::LgeDBus_EEPROMWrite(const QByteArray &in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_EEPROMWrite
    QMetaObject::invokeMethod(parent(), "LgeDBus_EEPROMWrite", Q_ARG(QByteArray, in0));
}

void SystemAdaptor::LgeDBus_ETCFactorySetting(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_ETCFactorySetting
    QMetaObject::invokeMethod(parent(), "LgeDBus_ETCFactorySetting", Q_ARG(uchar, in0));
}

void SystemAdaptor::LgeDBus_MainImageUpgrade(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_MainImageUpgrade
    QMetaObject::invokeMethod(parent(), "LgeDBus_MainImageUpgrade", Q_ARG(uchar, in0));
}

void SystemAdaptor::LgeDBus_PGSInitialRequest()
{
    // handle method call com.lge.car.micom.system.LgeDBus_PGSInitialRequest
    QMetaObject::invokeMethod(parent(), "LgeDBus_PGSInitialRequest");
}

void SystemAdaptor::LgeDBus_ResetHWPeripheral(const QByteArray &in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_ResetHWPeripheral
    QMetaObject::invokeMethod(parent(), "LgeDBus_ResetHWPeripheral", Q_ARG(QByteArray, in0));
}

void SystemAdaptor::LgeDBus_SetDiscIndicationLed(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_SetDiscIndicationLed
    QMetaObject::invokeMethod(parent(), "LgeDBus_SetDiscIndicationLed", Q_ARG(uchar, in0));
}

void SystemAdaptor::LgeDBus_SetFanControl(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_SetFanControl
    QMetaObject::invokeMethod(parent(), "LgeDBus_SetFanControl", Q_ARG(uchar, in0));
}

void SystemAdaptor::LgeDBus_SetHwRTCSet(const QByteArray &in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_SetHwRTCSet
    QMetaObject::invokeMethod(parent(), "LgeDBus_SetHwRTCSet", Q_ARG(QByteArray, in0));
}

void SystemAdaptor::LgeDBus_SetPhoneMicControl(uchar in0)
{
    // handle method call com.lge.car.micom.system.LgeDBus_SetPhoneMicControl
    QMetaObject::invokeMethod(parent(), "LgeDBus_SetPhoneMicControl", Q_ARG(uchar, in0));
}

void SystemAdaptor::LgeDBus_WDTCountClear()
{
    // handle method call com.lge.car.micom.system.LgeDBus_WDTCountClear
    QMetaObject::invokeMethod(parent(), "LgeDBus_WDTCountClear");
}

QByteArray SystemAdaptor::LgeDBus_WDTCountInfo()
{
    // handle method call com.lge.car.micom.system.LgeDBus_WDTCountInfo
    QByteArray out0;
    QMetaObject::invokeMethod(parent(), "LgeDBus_WDTCountInfo", Q_RETURN_ARG(QByteArray, out0));
    return out0;
}

