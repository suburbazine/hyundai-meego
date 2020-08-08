#include "DHAVN_AppAha_QmlProperty.h"
#include <stdio.h>

CAppAhaQmlProperty* CAppAhaQmlProperty::_instance = NULL;

CAppAhaQmlProperty::CAppAhaQmlProperty(QObject *parent)
    : QObject(parent)
{
    if(!_instance) _instance = this;

    initialize();
    //LOG_INIT_V2("Aha");
}

CAppAhaQmlProperty* CAppAhaQmlProperty::getInstance()
{
    return _instance;
}

void CAppAhaQmlProperty::initialize()
{
    // 1 = Preset,   2 = LBS
    i_focus_station = 1;

    //1  Preset on Left Menu,  3  LBS on Left Menu,  5  Preset list,  7  LBS list,  9  Title,  11 Track View,  13 ContentList
    i_focus_area = 5;
    b_focus_contentlist = false;
    c_err_viewText = "";    //wsuk.kim rm_pop
    c_err_viewText_2line = "STR_AHA_ERROR_VIEW_TEXT2";  //wsuk.kim 131223 2 line on Error view.
    b_first_track_entrance = false;
}

int CAppAhaQmlProperty::getFocusArea() const
{
    return i_focus_area;
}

void CAppAhaQmlProperty::setFocusArea(const int area)
{
    //LOG_LOW<<"setFocusArea" << area <<LOG_ENDL;
    i_focus_area = area;
}

int CAppAhaQmlProperty::getFocusScreen() const
{
    // 1 = Preset,   2 = LBS
    return i_focus_station;
}

void CAppAhaQmlProperty::setFocusScreen(const int index)
{
    // 1 = Preset,   2 = LBS
    i_focus_station = index;
}

bool CAppAhaQmlProperty::getFocusContentList() const
{
    return b_focus_contentlist;
}

void CAppAhaQmlProperty::setFocusContentList(const bool isVisible)
{
    b_focus_contentlist = isVisible;
}

QString CAppAhaQmlProperty::getActiveStationName()
{
    return station_name;
}

void CAppAhaQmlProperty::setActiveStationName(const QString &value)
{
    if( 32 < value.length())                // ITS 0222287, 229174,392 was resolved : 38 --> 35 -> 32
    {
        station_name = value.mid(0, 32);
        station_name += "...";
    }
    else
    {
        station_name = value;
    }
}

QString CAppAhaQmlProperty::getErrorViewText() const
{
    return c_err_viewText;
}

void CAppAhaQmlProperty::setErrorViewText(const QString &errViewText)
{
    c_err_viewText = errViewText;
}

//wsuk.kim 131223 2 line on Error view.
void CAppAhaQmlProperty::setErrorViewText2Line(const int errorText2LineIndex)
{
    if(errorText2LineIndex == 1)
    {
        c_err_viewText_2line = "STR_AHA_PLEASE_TRY_AGAIN_LATER";    //Please try again later.
    }
}

QString CAppAhaQmlProperty::getErrorViewText2Line() const
{
    return c_err_viewText_2line;
}
//wsuk.kim 131223 2 line on Error view.
