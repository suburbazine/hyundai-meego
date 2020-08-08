#ifndef DHAVN_APPMEDIAPLAYER_MAINUTILS_H
#define DHAVN_APPMEDIAPLAYER_MAINUTILS_H
#include <QObject>
#include <QtCore>
#include <QString>
#include "DHAVN_AppFramework_Event_Def.h" // added by Sergey for SwapDC

class AppMediaPlayer_MainUtils
{
   AppMediaPlayer_MainUtils( );
   ~AppMediaPlayer_MainUtils();

public:
   static AppMediaPlayer_MainUtils * GetInstance();
   static void FreeInstance();
   void copyUChars( QChar *buffer, int capacity, QString source );
   QString convertDuration( int duration );
   QString convertDurationAudio(int duration ); // added by yungi 2012.11.14 for ISV 62667,ISV 62596
   bool isCommonEvent(Event& aEvent); // added by Sergey for SwapDC

private:

   static AppMediaPlayer_MainUtils *m_pInstance;
   static int m_count;

};

#endif // DHAVN_APPMEDIAPLAYER_MAINUTILS_H
