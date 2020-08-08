
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Caption.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_CAPTION_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_CAPTION_H

#include <QObject>


#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_SModel_Caption : public QObject
{
    Q_OBJECT

    /** @property captionEnable
     * Enabled or disabled caption
     */
    Q_PROPERTY( bool captionEnable
                READ captionEnable
                NOTIFY captionEnableChanged)
    /** @property captionSize
     * Size of the caption
     */
    Q_PROPERTY( int captionSize
                READ captionSize
                NOTIFY captionSizeChanged)
    /** @property uiState
     * Current ui state is Jukebox or USB (1/2)
     */
    Q_PROPERTY( QString uiState
                READ uiState
                WRITE setUiState // added by Sergey 13.09.2013 for ITS#188762
                NOTIFY uiStateChanged)

    Q_PROPERTY( bool multipleCaptions
                READ multipleCaptions
                WRITE setMultipleCaptions
                NOTIFY multipleCaptionsChanged) // added by Sergey 09.10.2013 for ITS#187939

    Q_PROPERTY( bool smiCaptions
                READ smiCaptions
                WRITE setSmiCaptions
                NOTIFY smiCaptionsChanged) // added by Sergey 09.10.2013 for ITS#187939

public:
    AppMediaPlayer_Video_SModel_Caption( QObject *parent = NULL );

    void bindWith( AppMediaPlayer_Video_CModel_FS * );

    bool captionEnable() { return m_captionEnable; }
    int captionSize() { return m_captionSize; }
    QString uiState() { return m_uiState; }
    bool multipleCaptions() { return m_bMultipleCaptions; }
    bool smiCaptions() { return m_bSMICaptions; } // added by Sergey 09.10.2013 for ITS#187939

public slots:
    void setCaptionEnable(bool);
    void setCaptionSize(int);
    void setUiState(QString);
    void setMultipleCaptions(bool);
    void setSmiCaptions(bool); // added by Sergey 09.10.2013 for ITS#187939

signals:
    void captionEnableChanged( bool );
    void captionSizeChanged( int );
    void uiStateChanged( QString );
    void multipleCaptionsChanged(bool);
    void smiCaptionsChanged(bool); // added by Sergey 09.10.2013 for ITS#187939


private slots:


private:
    bool m_captionEnable;    
    int m_captionSize;    
    QString m_uiState;    
    bool m_bMultipleCaptions;
    bool m_bSMICaptions; // added by Sergey 09.10.2013 for ITS#187939
    AppMediaPlayer_Video_CModel_FS *m_bindedAppMediaPlayer_Video_CModel_FS;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_CAPTION_H

