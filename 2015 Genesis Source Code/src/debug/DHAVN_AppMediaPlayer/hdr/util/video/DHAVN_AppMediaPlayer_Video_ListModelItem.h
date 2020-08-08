#ifndef DHAVN_APPMEDIAPLAYER_LISTMODELITEM_H
#define DHAVN_APPMEDIAPLAYER_LISTMODELITEM_H

#include <QObject>

class VListModelItem : public QObject
{
    Q_OBJECT

public:
//    VListModelItem(QObject *parent = 0) : QObject(parent) {};
    VListModelItem(QString text1 = "", QString text2 = "", QObject *parent = 0) : QObject(parent)
    {
        setText1(text1);
        setText2(text2);
    }

    void setText1(QString value) { m_text1 = value; }
    QString text1() const { return  m_text1; }

    void setText2(QString value) { m_text2 = value; }
    QString text2() const { return  m_text2; }


private:
    QString m_text1;
    QString m_text2;
};

#endif // DHAVN_APPMEDIAPLAYER_LISTMODELITEM_H
