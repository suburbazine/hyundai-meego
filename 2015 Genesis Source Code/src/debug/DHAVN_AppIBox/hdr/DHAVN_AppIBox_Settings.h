#ifndef DHAVN_APPIBOX_SETTINGS_H
#define DHAVN_APPIBOX_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QList>

#include <QFile>
#include <QTextStream>
#include <QString>

#include "DHAVN_AppIBox_LogUtils.h"

#define INI_FILE                                "/app/share/AppIBox/qml/AppIBox/AppIBox.ini"

#define KEY_WIDGET_WATCH_BUS                    "widget.watch_bus"
#define KEY_WIDGET_SHOW_DELAY_MS                "widget.show_delay_ms"
#define KEY_WIDGET_SEND_RESPONSE_FG_DELAY_MS    "widget.send_response_FG_delay_ms"
#define KEY_WIDGET_FREEZE_ENABLED               "widget.freeze_enabled"

#define KEY_DRS_FREEZE_ENABLED                  "drs.freeze_enabled"

#define KEY_WIDGET_PIPELINE                     "widget.pipeline"

#define KEY_WIDGET_FRONT_X                      "widget.front.x"
#define KEY_WIDGET_FRONT_Y                      "widget.front.y"
#define KEY_WIDGET_FRONT_W                      "widget.front.w"
#define KEY_WIDGET_FRONT_H                      "widget.front.h"
#define KEY_WIDGET_REAR_X                       "widget.rear.x"
#define KEY_WIDGET_REAR_Y                       "widget.rear.y"
#define KEY_WIDGET_REAR_W                       "widget.rear.w"
#define KEY_WIDGET_REAR_H                       "widget.rear.h"

#define KEY_PIPELINE_NAME                       "pipeline.name"
#define KEY_PIPELINE_SRC                        "pipeline.src"
#define KEY_PIPELINE_EMGD_MAPPER                "pipeline.emgd_mapper"
#define KEY_PIPELINE_ALWAYS_COPY                "pipeline.always_copy"
#define KEY_PIPELINE_QUEUE_SIZE                 "pipeline.queue_size"
#define KEY_PIPELINE_DEVICE                     "pipeline.device"
#define KEY_PIPELINE_CAPS                       "pipeline.caps"
#define KEY_PIPELINE_SINK                       "pipeline.sink"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(Logger &pLogger, QObject *parent = 0);
    ~Settings();

    bool getBool(const char * key);
    const char* getChars(const char * key);
    int getInt(const char * key);
    QString getString(const char * key);
    QString getString(QString key);
    QList<QString> allKeys();

signals:

public slots:

private:
    DEFINE_LOGGER

    QSettings *m_pSettings;
    QMap<QString, QVariant> m_map;
    QMap<QString, QByteArray> m_mapByteArray; // This contains the same values as m_map but as QByteArray.
    void insert(QString key, QVariant value);
};

#endif // DHAVN_APPIBOX_SETTINGS_H
