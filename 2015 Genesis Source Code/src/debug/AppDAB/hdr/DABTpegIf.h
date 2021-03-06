/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c DABTpegIf -a DABTpegIf.h:DABTpegIf.cpp com.lge.car.dhavn.dmb.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DABTPEGIF_H_1349340516
#define DABTPEGIF_H_1349340516

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.lge.car.dhavn.Dmb
 */
class DABTpegIf: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.lge.car.dhavn.Dmb")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.lge.car.dhavn.Dmb\">\n"
"    <signal name=\"sendTPEGData\">\n"
"      <arg direction=\"out\" type=\"ay\" name=\"tpegData\"/>\n"
"      <arg direction=\"out\" type=\"i\" name=\"length\"/>\n"
"    </signal>\n"
"    <method name=\"reqTPEGDataOn\"/>\n"
"    <method name=\"reqTPEGDataOff\"/>\n"
"  </interface>\n"
        "")
public:
    DABTpegIf(QObject *parent);
    virtual ~DABTpegIf();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void reqTPEGDataOff();
    void reqTPEGDataOn();
Q_SIGNALS: // SIGNALS
    void sendTPEGData(const QByteArray &tpegData, int length);
};

#endif
