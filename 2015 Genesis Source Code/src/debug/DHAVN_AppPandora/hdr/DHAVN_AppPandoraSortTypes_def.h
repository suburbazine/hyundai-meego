#ifndef DHAVN_APPPANDORASORTTYPES_DEF_H
#define DHAVN_APPPANDORASORTTYPES_DEF_H
#include <DHAVN_Pndr_Defs.h>

class PandoraSortTypes : public QObject
{
    Q_OBJECT
    Q_ENUMS(EStationListSortType)
public:
    enum EStationListSortType
    {
        SORT_BY_NONE = SORT_TYPE_NONE,   /** No Sort Order defined yet */
        SORT_BY_DATE = SORT_TYPE_DATE,    /** Sorting of station based on Date */
        SORT_BY_ALPHA = SORT_TYPE_ALPHA  /** Alphabatical sorting of stations */
    };
};



#endif // DHAVN_APPPANDORASORTTYPES_DEF_H
