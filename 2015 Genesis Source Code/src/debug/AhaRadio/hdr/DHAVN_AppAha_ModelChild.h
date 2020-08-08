#ifndef DHAVN_APPAHA_MODELCHILD_H
#define DHAVN_APPAHA_MODELCHILD_H

#include "DHAVN_AppAha_ModelBase.h"

class CAppAha_ModelPreset : public CAppAha_ModelBase
{
    Q_OBJECT
public:
    explicit CAppAha_ModelPreset(QObject *parent = 0);

    static CAppAha_ModelPreset* getInstance();

private:

    static CAppAha_ModelPreset* _instance;
};

///////////////////////////////////////////////////////////////////////////////////////////
class CAppAha_ModelLBS : public CAppAha_ModelBase
{
    Q_OBJECT
public:
    explicit CAppAha_ModelLBS(QObject *parent = 0);

    static CAppAha_ModelLBS* getInstance();

private:

    static CAppAha_ModelLBS* _instance;
};

///////////////////////////////////////////////////////////////////////////////////////////
class CAppAha_ModelContent : public CAppAha_ModelBase
{
    Q_OBJECT
public:
    explicit CAppAha_ModelContent(QObject *parent = 0);

    static CAppAha_ModelContent* getInstance();

private:

    static CAppAha_ModelContent* _instance;
};

#endif // DHAVN_APPAHA_MODELCHILD_H
