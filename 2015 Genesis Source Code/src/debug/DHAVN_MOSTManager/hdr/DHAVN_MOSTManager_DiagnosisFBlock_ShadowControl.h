/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : MOSTManager                                       *
 * PROGRAMMER : Amit Koparde                                      *
 * DATE       : 13 September 2011                                 *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  Diagnosis FBlock_Shadow control,responsible to*
 *                  create instance of Fblock, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS:                                          *
 *
 *    DATE       PROGRAMMER               COMMENT                 *
 ******************************************************************
 * 13 Sep 2011   Amit Koparde          Draft version              *
 * 15 Sep 2011   Amit Koparde          Added Log_Tag define       *
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_DIAGNOSISFBLOCK_SHADOWCONTROL_H
#define DHAVN_MOSTMANAGER_DIAGNOSISFBLOCK_SHADOWCONTROL_H

#include <QtCore>
#include <QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_DiagnosisFBlock.h"
#include "DHAVN_MOSTManager_DiagnosisShadow.h"
#include "DHAVN_MOSTManager_ACSApplication.h"

#define LOG_TAG_DIAG_CNT "DiagnosisControl"


using namespace acs::generated::fblock::Diagnosis;
using namespace k2l::acs::protocols::most;


class CDiagnosisFBlockShadowControl;

class CMainSWVersionStatusListenerIBOX: public IACSMOSTEventListener
{
    CDiagnosisFBlockShadowControl *m_DiagnosisFBlock_ShadowControl;

public:
    CMainSWVersionStatusListenerIBOX(CDiagnosisFBlockShadowControl* context);
    virtual void OnChange();
};

class CVariantCodeStatusListenerHU : public IACSMOSTEventListener
{
    CDiagnosisFBlockShadowControl *m_DiagnosisFBlock_ShadowControl;

public:
    CVariantCodeStatusListenerHU(CDiagnosisFBlockShadowControl* context);
    virtual void OnChange();
    //virtual void OnGetReceived();
};

class CDiagnosisFBlockStateListener : public IACSMOSTEventListener
{
    acs::generated::shadow::Diagnosis::CDiagnosis *m_DiagnosisShadow;
    CDiagnosisFBlockShadowControl* m_DiagnosisFBlockShadowControl;
public:
    CDiagnosisFBlockStateListener(acs::generated::shadow::Diagnosis::CDiagnosis *diagContext,
                                  CDiagnosisFBlockShadowControl* context);
    virtual void OnChange();
};

class CDiagnosisFBlockShadowControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Instance of Diagnosis(0x01) on Head unit side hence Function Block */
    acs::generated::fblock::Diagnosis::CDiagnosis *m_DiagnosisFBlock;
    /** Instance of Diagnosis(0x02) on Ibox side hence Shadow Block */
    acs::generated::shadow::Diagnosis::CDiagnosis *m_DiagnosisShadow;
    /** Instance of Diagnosis(0x03) on AMP side hence Shadow Block */

    /** Instances of Properties and Listeners of Diagnosis on IBOX side */
    acs::generated::shadow::Diagnosis::CSWVersion *m_MainSWVersionIBOX;
    CMainSWVersionStatusListenerIBOX*m_MainSWVersionListenerIBOX;

    /** Instances of Properties and Listeners of Diagnosis on AMP side */

    /** Instances of Properties and Listeners of HMI on head unit side */
    acs::generated::fblock::Diagnosis::CVariantCode *m_VariantCodeHU;
    CVariantCodeStatusListenerHU *m_VariantCodeListenerHU;

    CDiagnosisFBlockStateListener *m_DiagnosisFBlockStateListener;

public:
    CDiagnosisFBlockShadowControl(CMarshaledACSApplication *context);
    ~CDiagnosisFBlockShadowControl();

    bool Initialize();
    virtual bool Register();
    virtual bool UnRegister();
    bool FBlockVisible(const uint deviceID);

    void SetListeners();
    void RemoveListeners();

    bool GetMainSWVersion();
    bool VariantCode(const QList<uint> &variantcode);
    void MainSWVersionStatus();
    void GetVariantCodeStatus_emit();

signals:
    void MainSWVersionStatus(const QStringList mainSWVersion);
    void GetVariantCodeStatus();
};

#endif // DHAVN_MOSTMANAGER_DIAGNOSISFBLOCK_SHADOWCONTROL_H
