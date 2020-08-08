#ifndef DHAVN_QWERTYKEYPAD_CHINESEPINYIN_ITEM_H
#define DHAVN_QWERTYKEYPAD_CHINESEPINYIN_ITEM_H

#include <QObject>

class PinyinListModelItem : public QObject
{
    Q_OBJECT

public:
    PinyinListModelItem(QObject *parent = 0) : QObject(parent) {}
    PinyinListModelItem(QString name, QObject *parent = 0) : QObject(parent)
    {
        setItemName(name);
    }
    PinyinListModelItem(QString name, int size, QObject *parent = 0) : QObject(parent)
    {
        setItemName(name);
        setItemSize(size);
    }

    void setItemName(QString value) { m_itemName = value; }
    Q_INVOKABLE QString getItemName() { return  m_itemName; }

    void setItemSize(int value) { n_itemSize = value; }
    Q_INVOKABLE int getItemSize() { return  n_itemSize; }

private:
    QString m_itemName;
    int n_itemSize;
};


#endif // DHAVN_QWERTYKEYPAD_CHINESEPINYIN_ITEM_H


