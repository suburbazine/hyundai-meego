#ifndef DHAVN_IPODCONTROLLER_SELECTSORTDBRECORD_H
#define DHAVN_IPODCONTROLLER_SELECTSORTDBRECORD_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <QString>

namespace ExtendedLingo
{

class CSelectSortDBRecord : public CIAPPacket
{
public:
    CSelectSortDBRecord();
    CSelectSortDBRecord(QString categoryValue,int index,QString sortTypeValue,int transId,bool isTidEnabled);
    //void setCategoryAndIndex(QString categoryValue,int indexValue);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
        QString m_Category;
        int m_Index;
        QString m_SortType;
};

} // end of namespace ExtendedLingo

#endif // DHAVN_IPODCONTROLLER_SELECTSORTDBRECORD_H
