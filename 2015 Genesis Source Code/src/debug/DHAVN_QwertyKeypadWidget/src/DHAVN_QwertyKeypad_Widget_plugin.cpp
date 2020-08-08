#include "DHAVN_QwertyKeypad_Widget_plugin.h"
#include "DHAVN_QwertyKeypad_Widget.h"

#include <QtDeclarative/qdeclarative.h>

#include "DHAVN_QwertyKeypad_Utility.h"
#include "DHAVN_QwertyKeypad_ChinesePinyin.h"
#include "DHAVN_QwertyKeypad_ChineseHWR_Canvas.h"
#include "DHAVN_QwertyKeypad_ChineseHWR.h"


void QwertyKeypadWidgetPlugin::registerTypes(const char* /*uri*/)
{
    qmlRegisterType<QwertyKeypadUtility>("qwertyKeypadUtility", 1, 0, "QwertyKeypadUtility");
    qmlRegisterType<QwertyKeypadChinesePinyin>("ChinesePinyin", 1, 0, "ChinesePinyin");
    qmlRegisterType<QwertyKeypadChineseHWR>("qwertyKeypadChineseHWR", 1, 0, "QwertyKeypadChineseHWR");
    qmlRegisterType<QwertyKeypadChineseHWRCanvas>("qwertyKeypadChineseHWRCanvas", 1, 0, "QwertyKeypadChineseHWRCanvas");
}

Q_EXPORT_PLUGIN2(QwertyKeypadWidget, QwertyKeypadWidgetPlugin)

