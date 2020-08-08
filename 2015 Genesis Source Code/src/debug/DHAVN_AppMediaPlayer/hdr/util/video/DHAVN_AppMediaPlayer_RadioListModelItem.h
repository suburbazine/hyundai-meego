#ifndef DHAVN_APPMEDIAPLAYER_RADIOLISTMODELITEM_H
#define DHAVN_APPMEDIAPLAYER_RADIOLISTMODELITEM_H

#include <QObject>

class RadioListModelItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY (bool enable READ enable); // added by kihyung 2012.07.24 for CR 12054

public:
    RadioListModelItem(QObject *parent = 0) : QObject(parent) {};
    RadioListModelItem(QString name, QObject *parent = 0) : QObject(parent)
    {
        setItemName(name);
        setEnable(true);  // modified by Sergey 20.08.2013 for ITS#184640 
    }

    void setItemName(QString value) { m_itemName = value; }
    QString getItemName() const { return  m_itemName; }

    // { added by kihyung 2012.07.24 for CR 12054
    void setEnable(bool value) { m_enable = value; }
    bool getEnable() const { return m_enable; }

    bool enable() { return getEnable(); }
    // } added by kihyung
private:
    QString m_itemName;
    bool    m_enable; // added by kihyung 2012.07.24 for CR 12054
};

#endif // DHAVN_APPMEDIAPLAYER_RADIOLISTMODELITEM_H
