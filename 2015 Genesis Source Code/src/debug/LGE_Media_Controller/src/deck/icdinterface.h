#ifndef ICDINTERFACE_H
#define ICDINTERFACE_H

#include "type.h"
#include "common.h"

class StatusData;

class ICDInterface
{
public:
    virtual void        Init()                                  = 0;
    virtual void        StartMedia()                            = 0;
    virtual void        EndMedia()                              = 0;
    virtual void        ProcessPacket(StatusData *p)            = 0;
};

#endif // IMEDIAINTERFACE_H
