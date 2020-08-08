#ifndef DHAVN_APPAHA_QMLPROPERTY_H
#define DHAVN_APPAHA_QMLPROPERTY_H

#include <QObject>
#include <QTimer>
//#include <DHAVN_LogUtil.h>

class CAppAhaQmlProperty : public QObject
{
    Q_OBJECT
public:
    explicit CAppAhaQmlProperty(QObject *parent = 0);
    static CAppAhaQmlProperty* getInstance();

    void initialize();

    bool getFocusContentList() const;
    void setFocusContentList(const bool isVisible);

    void setActiveStationName(const QString &value);    
public slots:
    void setFocusArea(const int area);
    void setFocusScreen(const int index);

    int getFocusScreen() const;
    int getFocusArea() const;

    void setErrorViewText(const QString &errViewText);    //wsuk.kim rm_pop
    void setErrorViewText2Line(const int errorText2LineIndex);    //wsuk.kim 131223 2 line on Error view.
    QString getErrorViewText() const;    //wsuk.kim rm_pop
    QString getErrorViewText2Line() const;  //wsuk.kim 131223 2 line on Error view.
    QString getActiveStationName();
//wsuk.kim TUNE_TRACK
    void setFirstTrackEntrance(bool isFirstTrackState) { b_first_track_entrance = isFirstTrackState; }
    bool getFirstTrackEntrance() { return b_first_track_entrance; }
//wsuk.kim TUNE_TRACK
private:
    /** Logger instance */
    //USE_LOG_UTIL
    static CAppAhaQmlProperty* _instance;

    QString station_name;
    QString c_err_viewText;    //wsuk.kim rm_pop
    QString c_err_viewText_2line;   //wsuk.kim 131223 2 line on Error view.

    int i_focus_area;
    int i_focus_station;

    bool b_focus_contentlist;
    bool b_first_track_entrance; //wsuk.kim TUNE_TRACK
};

#endif // DHAVN_APPAHA_QMLPROPERTY_H
