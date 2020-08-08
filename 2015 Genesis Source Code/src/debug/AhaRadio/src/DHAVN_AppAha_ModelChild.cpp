#include "DHAVN_AppAha_ModelChild.h"

CAppAha_ModelPreset* CAppAha_ModelPreset::_instance = NULL;

CAppAha_ModelPreset::CAppAha_ModelPreset(QObject *parent)
    : CAppAha_ModelBase(parent)
{
    if(!_instance) _instance = this;
}

CAppAha_ModelPreset* CAppAha_ModelPreset::getInstance()
{
    return _instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAppAha_ModelLBS* CAppAha_ModelLBS::_instance = NULL;

CAppAha_ModelLBS::CAppAha_ModelLBS(QObject *parent)
    : CAppAha_ModelBase(parent)
{
    if(!_instance) _instance = this;
}

CAppAha_ModelLBS* CAppAha_ModelLBS::getInstance()
{
    return _instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CAppAha_ModelContent* CAppAha_ModelContent::_instance = NULL;

CAppAha_ModelContent::CAppAha_ModelContent(QObject *parent)
    : CAppAha_ModelBase(parent)
{
    if(!_instance) _instance = this;
}

CAppAha_ModelContent* CAppAha_ModelContent::getInstance()
{
    return _instance;
}

