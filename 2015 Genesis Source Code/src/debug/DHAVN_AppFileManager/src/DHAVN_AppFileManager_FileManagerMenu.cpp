#include "DHAVN_AppFileManager_UI_main.h"
#include "hdr/DHAVN_AppFileManager_FileManagerMenu.h"
#include <QDebug>
#include <qdeclarative.h>

// modified by Dmitry 17.08.13
OPTION_MENU_T FileManagerMenu::USBMenu[] =
{
    {FileManagerMenu::headerId, "MENU", false, false, false, false, true, NULL, 0, ""},
    // { modified by lyg 2012.10.02 CR 8314
    {FileManagerMenu::nowPlayingId, "STR_MEDIA_NOW_PLAYING", false, false, false, true, true, NULL, 0, ""},
    {FileManagerMenu::copyId, "STR_MEDIA_MNG_COPY_TO_JUKEBOX", false, false, false, true, true, NULL, 0, ""}
    // } modified by lyg 2012.10.02 CR 8314
};

OPTION_MENU_T FileManagerMenu::cancelCopyingMenu[] =
{
    {FileManagerMenu::headerId, "MENU", false, false, false, false, true, NULL, 0, ""},
    {FileManagerMenu::cancelCopyId, "STR_MEDIA_MNG_CANCEL_COPYING", false, false, false, true, true, NULL, 0, ""}     // modified by lyg 2012.10.02 CR 8314
};

OPTION_MENU_T FileManagerMenu::jukeboxMenu[] =
{
    {FileManagerMenu::headerId, "MENU", false, false, false, false, true, NULL, 0, ""},
 // { modified by lyg 2012.10.02 CR 8314
    {FileManagerMenu::nowPlayingId, "STR_MEDIA_NOW_PLAYING", false, false, false, true, true, NULL, 0, ""},
    //{FileManagerMenu::moveId, "STR_MEDIA_MNG_MOVE", false, false, false, true, true, NULL, 0}, //deleted by yungi 2013.2.4 for UX Scenario 2.Jukebox Move
    {FileManagerMenu::deleteId, "STR_MEDIA_MNG_DELETE", false, false, false, true, true, NULL, 0, ""},
    // { modified by eunhye 2012.08.30 CR 13374 
    //{FileManagerMenu::deleteAllId, "STR_MEDIA_MNG_CLEAR_JUKEBOX", false, false, false, true, true, NULL, 0},
    {FileManagerMenu::deleteAllId, "STR_MEDIA_MNG_CLEAR_JUKEBOX", false, false, false, true, true, NULL, 0, ""},
    // } modified by eunhye 2012.08.30
    {FileManagerMenu::seeCapacityId, "STR_MEDIA_MNG_CAPACITY", false, false, false, true, true, NULL, 0, ""}
  // } modified by lyg 2012.10.02 CR 8314
};

OPTION_MENU_T FileManagerMenu::cancelMovingMenu[] =
{
    {FileManagerMenu::headerId, "MENU", false, false, false, false, true, NULL, 0, ""},
    {FileManagerMenu::cancelMoveId, "STR_MEDIA_MNG_CANCEL_MOVING", false, false, false, true, true, NULL, 0, ""}     // modified by lyg 2012.10.02 CR 8314
};

OPTION_MENU_T FileManagerMenu::cancelDeletingMenu[] =
{
    {FileManagerMenu::headerId, "MENU", false, false, false, false, true, NULL, 0, ""},
    {FileManagerMenu::cancelDeleteId, "STR_MEDIA_MNG_CANCEL_DELETING", false, false, false, true, true, NULL, 0, ""}
};

FileManagerMenu::FileManagerMenu(UI_Main *parent) :
    QObject(parent)
  , context(invalidContext)
  , uiControl(parent)
{
    // FM_LOG_INIT_V2("AppFileManager");
    qmlRegisterUncreatableType<OptionMenuModel>("OptionMenuModel", 1, 0, "OptionMenuModel", "OptionMenuModel cannot be created from QML context");
}

void FileManagerMenu::setMenuContext(MenuContext context)
{
    OPTION_MENU_T* optionsMenuData = NULL;
    int optionsMenuSize = 0;
    switch (context)
    {
    case usbContext:
        // { modified by wspark 2013.02.03
        //optionsMenuData = USBMenu;
        //optionsMenuSize = sizeof(USBMenu) / sizeof(OPTION_MENU_T);
		// commented by ravikanth 17-05-13 for ISV 83180
//        if(uiControl->getCurrentFileOperation() == UI_Main::CURRENT_OPERATION_COPY)
//        {
//            optionsMenuData = cancelCopyingMenu;
//            optionsMenuSize = sizeof(cancelCopyingMenu) / sizeof(OPTION_MENU_T);
//        }
//        else
        if(uiControl->getCurrentFileOperation() == UIMainDef::CURRENT_OPERATION_DELETE
                || uiControl->getCurrentFileOperation() == UIMainDef::CURRENT_OPERATION_DELETE_ALL)
        {
            optionsMenuData = cancelDeletingMenu;
            optionsMenuSize = sizeof(cancelDeletingMenu) / sizeof(OPTION_MENU_T);
        }

        else
        {
            optionsMenuData = USBMenu;
            optionsMenuSize = sizeof(USBMenu) / sizeof(OPTION_MENU_T);
        }
        // } modified by wspark
        break;
    case jukeboxContext:
        // { added by wspark 2012.12.13 for CR16396
        if(uiControl->GetStateManager()->getEmptyState() == true)
        {
            jukeboxMenu[1].bEnabled = false;
            jukeboxMenu[2].bEnabled = false;
            jukeboxMenu[3].bEnabled = false;
        }
        else
        {
            jukeboxMenu[1].bEnabled = true;
            jukeboxMenu[2].bEnabled = true;
            jukeboxMenu[3].bEnabled = true;
        }
        // } added by wspark
        optionsMenuData = jukeboxMenu;
        optionsMenuSize = sizeof(jukeboxMenu) / sizeof(OPTION_MENU_T);
        break;
    case copyContext:
        optionsMenuData = cancelCopyingMenu;
        optionsMenuSize = sizeof(cancelCopyingMenu) / sizeof(OPTION_MENU_T);
        break;
    case moveContext:
        optionsMenuData = cancelMovingMenu;
        optionsMenuSize = sizeof(cancelMovingMenu) / sizeof(OPTION_MENU_T);
        break;
    case deleteContext:
        optionsMenuData = cancelDeletingMenu;
        optionsMenuSize = sizeof(cancelDeletingMenu) / sizeof(OPTION_MENU_T);
        //this->menuModel = CreateOptionMenu(cancelDeletingMenu, sizeof(cancelDeletingMenu));
        break;

    case invalidContext:
    default:
        m_OptionMenuModel.reset();
        emit changeOptionMenuModel();
    }

    if(optionsMenuData != NULL && optionsMenuSize > 0)
    {
        if(m_OptionMenuModel.isNull())
        {
            m_OptionMenuModel.reset(new OptionMenuModel(optionsMenuData, optionsMenuSize));
            emit changeOptionMenuModel();
        }
        else
        {
            m_OptionMenuModel.data()->updateModel(optionsMenuData, optionsMenuSize);
        }
    }

    this->context = context;

    //emit updateMenu(this->menuModel);
}

void FileManagerMenu::HandleOptionMenu(int index, QVariant item)
{
    Q_UNUSED(item);
    this->uiControl->setCopyFromMainPlayer(false); //[KOR][ITS][179014][comment](aettie.ji)
    switch (this->context)
    {
    case jukeboxContext:
        processJukeboxContext(index);
        break;
    case usbContext:
        // { modified by wspark 2013.02.03
        //processUsbContext(index);
		// commented by ravikanth 17-05-13 for ISV 83180
//        if(uiControl->getCurrentFileOperation() == UI_Main::CURRENT_OPERATION_COPY)
//        {
//            uiControl->popupEventHandler( UI_Main::POPUP_EVENT_COPY_CANCEL );
//        }
//        else
        if(uiControl->getCurrentFileOperation() == UIMainDef::CURRENT_OPERATION_DELETE
                || uiControl->getCurrentFileOperation() == UIMainDef::CURRENT_OPERATION_DELETE_ALL)
        {
            uiControl->popupEventHandler( UIMainDef::POPUP_EVENT_DELETE_CANCEL);
        }
        else
        {
            processUsbContext(index);
        }
        // } modified by wspark
        break;
    case copyContext:
        processCopyContext(index);
        break;
    case moveContext:
        processMoveContext(index);
        break;
    case deleteContext:
        processDeleteContext(index);
        break;
    default:
        // FM_LOG << "FileManagerMenu::HandleOptionMenu unknown context" << LOG_ENDL;
        break;
    }
}

void FileManagerMenu::processJukeboxContext(int index)
{
    if (this->uiControl != NULL)
    {
        switch (index)
        {
        case nowPlayingId:
            this->uiControl->handleNowPlaying();
            break;
        case moveId:
        {
           this->uiControl->contextMoveId();
            break;
        }
            break;
        case deleteId:
        {
           this->uiControl->contextDeleteId();
            break;
        }
        case deleteAllId:
        {
           this->uiControl->contextDeleteAllId();
            break;
        }
            break;
        case seeCapacityId:
            this->uiControl->showCapacityPopup();
            break;
        }
    }
    else
    {
        // FM_LOG << "FileManagerMenu::processJukeboxContext UI controller is NULL" << LOG_ENDL;
    }
}

void FileManagerMenu::processUsbContext(int index)
{
    if (this->uiControl != NULL)
    {
        switch (index)
        {
        case nowPlayingId:
            this->uiControl->handleNowPlaying();
            break;
        case copyId:
            //this->uiControl->setCopyFromMainPlayer(false); //[KOR][ITS][179014][comment](aettie.ji)
            // { modified by ravikanth 16-04-13
            if(this->uiControl->getCurrentAudioFileOperation() != true)
            {
                this->uiControl->setEditButton(false);
            }
            else
            {
                this->uiControl->popupEventHandler(UIMainDef::POPUP_EVENT_COPY_TO_JUKEBOX_CONFIRM);
            }
            // } modified by ravikanth 16-04-13
            break;
        }
    }
    else
    {
        // FM_LOG << "FileManagerMenu::processUsbContext UI controller is NULL" << LOG_ENDL;
    }
}

void FileManagerMenu::processCopyContext(int index)
{
    if (this->uiControl != NULL)
    {
        switch (index)
        {
        case cancelCopyId:
            this->uiControl->contextInteractiveId();
            break;
        }
    }
    else
    {
        // FM_LOG << "FileManagerMenu::processCopyContext UI controller is NULL" << LOG_ENDL;
    }
}

void FileManagerMenu::processMoveContext(int index)
{
    if (this->uiControl != NULL)
    {
        switch (index)
        {
        case cancelMoveId:
           this->uiControl->cancelMoveMode();
            break;
        }
    }
    else
    {
        // FM_LOG << "FileManagerMenu::processMoveContext UI controller is NULL" << LOG_ENDL;
    }
}

void FileManagerMenu::processDeleteContext(int index)
{
    if (this->uiControl != NULL)
    {
        switch (index)
        {
        case cancelDeleteId:
           this->uiControl->contextInteractiveId();
            break;
        }
    }
    else
    {
        // FM_LOG << "FileManagerMenu::processDeleteContext UI controller is NULL" << LOG_ENDL;
    }
}
// modified by Dmitry 17.08.13

