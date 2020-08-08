/** @file DHAVN_AppMediaPlayer_VideoModel.h
 *
 * This module provides a base class for controller models.
 * */
#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_BASE_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_BASE_H

#include <QObject>
#include <QtCore/qmetatype.h>
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

/** @class AppMediaPlayer_Video_CModel_Base
 * This class wraps common properties of the media source such as
 * playing position, random and repeat modes and playback status.
 *
 * While this kind of models are not intended to be used in the UI
 * we do not define strict properties as defined by Qt property
 * system. However, the only difference is ubsence of Q_PROPERTY
 * declarations.
 *
 * FIXME: Design issue. We may want to have a trivial implementation
 * of RTTI here to distinguish models without additional code.
 * However let's leave it as is till we're ready with model usage.
 * */
class AppMediaPlayer_Video_CModel_Base : public QObject
{
    Q_OBJECT
public:
    /** @brief The playback status*/
    enum PLAYBACK_STATUS_T
    {
        PLAYBACK_STATUS_PLAYING,
        PLAYBACK_STATUS_PAUSED,
        PLAYBACK_STATUS_STOPPED,
        PLAYBACK_STATUS_FASTFORWARDING_4X,
        PLAYBACK_STATUS_FASTFORWARDING_16X,
        PLAYBACK_STATUS_FASTFORWARDING_20X,
        PLAYBACK_STATUS_REWINDING_4X,
        PLAYBACK_STATUS_REWINDING_16X,
        PLAYBACK_STATUS_REWINDING_20X,
        PLAYBACK_STATUS_INVALID
    };

    /** @brief The random mode*/
    enum RANDOM_MODE_T
    {
        RANDOM_MODE_OFF    = 0,
        RANDOM_MODE_FOLDER = 1,
        RANDOM_MODE_FILE   = 2
    };

    /** @brief The repeat mode*/
    enum REPEAT_MODE_T
    {
        REPEAT_MODE_OFF     = 0,
        REPEAT_MODE_FOLDER  = 1,
        REPEAT_MODE_FILE    = 2
    };

    // { added by kihyung 2012.07.26 for CR 11894
    /** @brief The scan mode*/
    enum SCAN_MODE_T
    {
        SCAN_MODE_OFF     = 0,
        SCAN_MODE_FOLDER  = 1,
        SCAN_MODE_FILE    = 2
    };
    // } added by kihyung

    AppMediaPlayer_Video_CModel_Base( QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_CModel_Base();
    
    /** @property playbackStatus
     * @brief Reflects the playback state (playing, paused, stopped).
     * Read-only.
     * */
    PLAYBACK_STATUS_T playbackStatus() { return m_playbackStatus; }
    void setPlaybackStatus( PLAYBACK_STATUS_T value );
    
    /** @property randomMode
     * @brief Reflects the random mode (off, folder ). Read-only.
     */
    RANDOM_MODE_T randomMode() { return m_randomMode; }
    void setRandomMode( RANDOM_MODE_T value );

    /** @property repeatMode
     * @brief Reflects the repeat mode (off, folder, file ). Read-only.
     * */
    REPEAT_MODE_T repeatMode() { return m_repeatMode; }
    void setRepeatMode( REPEAT_MODE_T value );

    // { added by kihyung 2012.07.26 for CR 11894
    /** @property scanMode
     * @brief Reflects the scan mode (off, folder, file ). Read-only.
     * */
    SCAN_MODE_T scanMode() { return m_scanMode; }
    void setScanMode( SCAN_MODE_T value );
    // } added by kihyung

    /** @property position
     * @brief Relfects the position of the playing media in milliseconds.
     * Read-only.
     * */
    int position() { return m_position; }
    void setPosition( int value );

    /** @property dirty
     * Is set to true if data stored in the model does not reflect
     * the properties of the pipeline. For example, if state is
     * loaded from the settings upon startup.
     * */
    bool dirty() { return m_isDirty; }
    void setDirty( bool value );
    
    /** @property dragging
     * On when user drags progress bar to seek video.
     * */
    bool dragging() { return m_isDragging; }
    void setDragging( bool value );

    // added by Sergey for Loading popup
    bool loadingMode() { return m_loadingMode; }
    void setLoadingMode(bool);
    // added by Sergey for Loading popup

    // added by kihyung 2013.09.15 for ISV 91113
    bool removingMode() { return m_removingMode; }
    void setRemovingMode(bool);
    // added by kihyung 2013.09.15 for ISV 91113
    
signals:
    /** @{ property_notifiers */
    void playbackStatusChanged( AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T value );
    void randomModeChanged( AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T value );
    void repeatModeChanged( AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T value );
    void scanModeChanged( AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T value ); // added by kihyung 2012.07.26 for CR 11894
    void positionChanged( int position );
    void dirtyChanged( bool value );
    void draggingChanged( bool value );
    void loadingModeChanged(bool); // added by Sergey for Loading popup
    void removingModeChanged(bool); // added by kihyung 2013.09.15 for ISV 91113
    /** @} */

private:
    PLAYBACK_STATUS_T        m_playbackStatus;
    RANDOM_MODE_T            m_randomMode;
    REPEAT_MODE_T            m_repeatMode;
    SCAN_MODE_T              m_scanMode; // added by kihyung 2012.07.26 for CR 11894
    int                      m_position;
    bool                     m_isDirty;
    bool                     m_isDragging;
    bool                     m_loadingMode; // added by Sergey for Loading popup
    bool                     m_removingMode; // added by kihyung 2013.09.15 for ISV 91113
};

Q_DECLARE_METATYPE(AppMediaPlayer_Video_CModel_Base::PLAYBACK_STATUS_T)
Q_DECLARE_METATYPE(AppMediaPlayer_Video_CModel_Base::RANDOM_MODE_T)
Q_DECLARE_METATYPE(AppMediaPlayer_Video_CModel_Base::REPEAT_MODE_T)
Q_DECLARE_METATYPE(AppMediaPlayer_Video_CModel_Base::SCAN_MODE_T) // added by kihyung 2012.07.26 for CR 11894

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_BASE_H

