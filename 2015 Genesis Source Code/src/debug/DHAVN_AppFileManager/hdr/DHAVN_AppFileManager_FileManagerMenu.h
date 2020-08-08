#ifndef DHAVN_APPFILEMANAGER_FILEMANAGERMENU_H
#define DHAVN_APPFILEMANAGER_FILEMANAGERMENU_H

#include <DHAVN_OptionMenuModel.h>
#include "DHAVN_AppFileManager_Logger.h"


class UI_Main;

class FileManagerMenu : public QObject
{
    Q_OBJECT

    Q_PROPERTY( OptionMenuModel* optionMenuModel READ getOptionMenuModel
                NOTIFY changeOptionMenuModel);
    // USE_FM_LOG_UTIL
public:
    OptionMenuModel* getOptionMenuModel()
    {
        return m_OptionMenuModel.data();
    }

Q_SIGNALS:
    void changeOptionMenuModel();

public:
    enum MenuContext
    {
        usbContext,
        jukeboxContext,
        copyContext,
        moveContext,
        deleteContext,
        invalidContext
    };

    explicit FileManagerMenu(UI_Main *parent);

    void setMenuContext(MenuContext context);
    Q_INVOKABLE void HandleOptionMenu(int index, QVariant item);

signals:
    void updateMenu(QVariant newModel);

private:
    enum MenuIds
    {
        nowPlayingId,
        copyId,
        cancelCopyId,
        cancelMoveId,
        cancelDeleteId,
        moveId,
        deleteId,
        deleteAllId,
        seeCapacityId,
        headerId = 99
    };

private:
    void processJukeboxContext(int index);
    void processUsbContext(int index);
    void processCopyContext(int index);
    void processMoveContext(int index);
    void processDeleteContext(int index);

private:
    MenuContext context;
    UI_Main *uiControl;

    QScopedPointer<OptionMenuModel> m_OptionMenuModel;

    static OPTION_MENU_T USBMenu[];
    static OPTION_MENU_T cancelCopyingMenu[];
    static OPTION_MENU_T jukeboxMenu[];
    static OPTION_MENU_T cancelMovingMenu[];
    static OPTION_MENU_T cancelDeletingMenu[];
};

#endif // DHAVN_APPFILEMANAGER_FILEMANAGERMENU_H
