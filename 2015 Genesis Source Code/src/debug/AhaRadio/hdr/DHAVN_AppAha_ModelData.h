#ifndef DHAVN_APPAHA_MODELDATA_H
#define DHAVN_APPAHA_MODELDATA_H

#include <QtGui>

class ModelData
{
public:
    explicit ModelData(const qulonglong appID, const bool isSelect, const QString &name);

    qulonglong getAppID() const;
    void setAppID(const qulonglong value);

    bool getIsSelect() const;
    void setIsSelect(const bool value);

    bool getIsPlay() const;
    void setIsPlay(const bool value);

    QString getName() const;
    void setName(const QString &value);

    QString getImagePath() const;
    void setImagePath(const QString &value);

private:
    qulonglong m_appID;
    bool m_isSelect;
    bool m_isPlay;

    QString m_name;
    QString m_imagePath;
};

#endif // DHAVN_APPAHA_MODELDATA_H
