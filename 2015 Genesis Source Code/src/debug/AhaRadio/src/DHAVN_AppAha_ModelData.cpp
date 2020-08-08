#include "DHAVN_AppAha_ModelData.h"

ModelData::ModelData(const qulonglong appID, const bool isSelect, const QString &name)
    :m_appID(appID),
     m_isPlay(false),
     m_isSelect(isSelect),
     m_name(name),
     m_imagePath("null")
{
}

qulonglong ModelData::getAppID() const
{
    return m_appID;
}

void ModelData::setAppID(const qulonglong value)
{
    m_appID = value;
}

bool ModelData::getIsSelect() const
{
    return m_isSelect;
}

void ModelData::setIsSelect(const bool value)
{
    m_isSelect = value;
}

bool ModelData::getIsPlay() const
{
    return m_isPlay;
}

void ModelData::setIsPlay(const bool value)
{
    m_isPlay = value;
}

QString ModelData::getName() const
{
    return m_name;
}

void ModelData::setName(const QString &value)
{
    m_name = value;
}

QString ModelData::getImagePath() const
{
    return m_imagePath;
}

void ModelData::setImagePath(const QString &value)
{
    m_imagePath = value;
}
