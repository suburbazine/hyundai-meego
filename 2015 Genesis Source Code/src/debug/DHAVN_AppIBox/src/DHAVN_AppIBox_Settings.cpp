#include <QStringList>

#include "DHAVN_AppIBox_Settings.h"

Settings::Settings(Logger &pLogger, QObject *parent) :
    m_pLogger(pLogger),
    QObject(parent)
{
    m_pSettings = new QSettings(INI_FILE, QSettings::IniFormat);
    //LOG(QString("Loading...%1").arg(INI_FILE));

    QStringList generalKeys = m_pSettings->childKeys();
    for (int i = 0; i < generalKeys.count(); i++)
    {
        QString key = generalKeys.at(i);
        insert(key, m_pSettings->value(key));
    }

    QString pipeline = m_pSettings->value(KEY_WIDGET_PIPELINE).toString();
    m_pSettings->beginGroup(QString("Pipeline.").append(pipeline));
    QStringList pipelineKeys = m_pSettings->childKeys();
    for (int i = 0; i < pipelineKeys.count(); i++)
    {
        QString key = pipelineKeys.at(i);
        insert(key, m_pSettings->value(key));
    }
    m_pSettings->endGroup();

    //LOG(QString("Loaded...%1").arg(INI_FILE));
}

Settings::~Settings()
{
    m_map.clear();
    delete m_pSettings;
}

bool Settings::getBool(const char *key)
{
    return m_map.value(key).toBool();
}

const char * Settings::getChars(const char *key)
{
    return m_mapByteArray.value(key).constData();
}

int Settings::getInt(const char *key)
{
    return m_map.value(key).toInt();
}

QString Settings::getString(const char *key)
{
    return m_map.value(key).toString();
}

QString Settings::getString(QString key)
{
    return m_map.value(key).toString();
}

QList<QString> Settings::allKeys()
{
    return m_map.keys();
}

void Settings::insert(QString key, QVariant value)
{
    //LOG(QString("key=%1, value=%2").arg(key).arg(m_pSettings->value(key).toString()));
    m_map.insert(key, value);
    QByteArray ba(value.toByteArray());
    m_mapByteArray.insert(key, ba);
}
