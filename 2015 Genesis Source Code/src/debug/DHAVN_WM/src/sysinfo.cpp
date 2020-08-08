#include <QObject>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QTranslator>
#include <QApplication>
#include "dhavnlog.h"
#include "common.h"

static int CDC = 0;

void get_system_info(int & variant, int & language, int & rseStatus){
	lprintf("get_system_info");
	QDBusConnection conn = QDBusConnection::sessionBus();
	QDBusMessage variantMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskSysOption");
	QDBusMessage langMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskLanguageInfoValue");
	QDBusMessage sysLastMsg = QDBusMessage::createMethodCall("com.lge.car.micom", "/SysInfo", "com.lge.car.micom.sysinfo", "LgeDBus_AskSysLastStatus");

	QDBusMessage variantReply = conn.call(variantMsg, QDBus::Block, 3000);
	QDBusMessage langReply = conn.call(langMsg, QDBus::Block, 3000);
	QDBusMessage lastReply = conn.call(sysLastMsg, QDBus::Block, 3000);

	QByteArray variantArray;
	QByteArray langArray;
	QByteArray lastArray;

	if ( variantReply.arguments().count()){
		variantArray = variantReply.arguments().at(0).toByteArray();
		if ( variantArray.count() > 1)
			variant = (int) variantArray.at(1);
		lprintf("Variant : %d", variant);

                if ( variantArray.count() > 4) {
                    lprintf("variant cdc : %02X, %02X, %02X", (unsigned char)variantArray.at(2), (unsigned char)variantArray.at(3), (unsigned char)variantArray.at(4));
                    CDC = (int) variantArray.at(3) & 0x02;  // 17bit
                }
                lprintf("variant cdc: %d", CDC);
	}

	if ( langReply.arguments().count()){
		langArray = langReply.arguments().at(0).toByteArray();
		if ( langArray.count())
			language = (int) langArray.at(0);	
		lprintf("Language : %d", language);
	}

	if ( lastReply.arguments().count()){
		lastArray = lastReply.arguments().at(0).toByteArray();
		if ( lastArray.count() > 2){
			lprintf("Last Status : %02X, %02X, %02X", (unsigned char)lastArray.at(0), (unsigned char)lastArray.at(1), (unsigned char)lastArray.at(2));
			rseStatus = (int) lastArray.at(1) & 0x08;
			lprintf("RSE : %s", (rseStatus == 0? "OFF":"ON"));
		}
	}
}

void apply_language_info(int variant, int lang){
	static QTranslator * translator = NULL;

	if ( translator != NULL){
		qApp->removeTranslator(translator);
		delete translator;
	}

	translator = new QTranslator;
	switch(lang){
	case LANG_ko_KR:		// Korea
		translator->load(":translation/AppStatus_ko_KR.qm"); break;
	case LANG_en_GB:		// English(UK)
		translator->load(":translation/AppStatus_en_GB.qm"); break;
	case LANG_zh_CN:		// Chinese mandarin
	case LANG_zh_HK:		// Chinese cantonese
		translator->load(":translation/AppStatus_zh_CN.qm"); break;
	case LANG_fr_FR:		// French
		if ( variant == VARIANT_NA)
			translator->load(":translation/AppStatus_fr_CA.qm");
		else
			translator->load(":translation/AppStatus_fr_FR.qm");
		break;
	case LANG_es_ES:		// Spanish
		if ( variant == VARIANT_NA)
			translator->load(":translation/AppStatus_es_US.qm");
		else
			translator->load(":translation/AppStatus_es_ES.qm");
		break;
	case LANG_ar_SA:		// Arabic
		translator->load(":translation/AppStatus_ar_SA.qm"); break;
	case LANG_cs_CZ:		// Czech
		translator->load(":translation/AppStatus_cs_CZ.qm"); break;
	case LANG_da_DK:		// Danish
		translator->load(":translation/AppStatus_da_DK.qm"); break;
	case LANG_de_DE:		// German
		translator->load(":translation/AppStatus_de_DE.qm"); break;
	case LANG_it_IT:		// Italian
		translator->load(":translation/AppStatus_it_IT.qm"); break;
	case LANG_nl_NL:		// Dutch
		translator->load(":translation/AppStatus_nl_NL.qm"); break;
	case LANG_pl_PL:		// Polish
		translator->load(":translation/AppStatus_pl_PL.qm"); break;
	case LANG_pt_PT:		// Portuguese
		translator->load(":translation/AppStatus_pt_PT.qm"); break;
	case LANG_ru_RU:		// Russian
		translator->load(":translation/AppStatus_ru_RU.qm"); break;
	case LANG_sk_SK:		// Slovak
		translator->load(":translation/AppStatus_sk_SK.qm"); break;
	case LANG_sv_SE:		// Swedish
		translator->load(":translation/AppStatus_sv_SE.qm"); break;
	case LANG_tr_TR:		// Turkish
		translator->load(":translation/AppStatus_tr_TR.qm"); break;
        case LANG_fr_CA:
                translator->load(":translation/AppStatus_fr_CA.qm"); break;
        case LANG_es_US:
                translator->load(":translation/AppStatus_es_US.qm"); break;
        case LANG_en_US:		// English(US)
            translator->load(":translation/AppStatus_en_US.qm"); break;
	default:
            if( variant == VARIANT_KOREA) {
                translator->load(":translation/AppStatus_ko_KR.qm"); break;
            } else if( variant == VARIANT_CHINA) {
                translator->load(":translation/AppStatus_zh_CN.qm"); break;
            } else if( variant == VARIANT_EUROPE) {
                translator->load(":translation/AppStatus_en_GB.qm"); break;
            } else if( variant == VARIANT_ME) {
                translator->load(":translation/AppStatus_ar_SA.qm"); break;
            } else if(  variant == VARIANT_NA || variant == VARIANT_GENERAL || variant == VARIANT_CANADA) {
                translator->load(":translation/AppStatus_es_US.qm"); break;
            } else if( variant == VARIANT_RUSIA) {
                translator->load(":translation/AppStatus_ru_RU.qm"); break;
            }
	}

	qApp->installTranslator(translator);
}

int getCDC() {
    lprintf("sysinfo cdc:%d",CDC);
    return CDC;
}
