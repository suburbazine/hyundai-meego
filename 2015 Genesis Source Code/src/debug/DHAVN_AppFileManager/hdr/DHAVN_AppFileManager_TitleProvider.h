#ifndef DHAVN_APPFILEMANAGER_TITLEPROVIDER_H
#define DHAVN_APPFILEMANAGER_TITLEPROVIDER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QFont>

class TitleProvider : public QObject
{
   Q_OBJECT
   Q_ENUMS( TITLE_TYPES )
   Q_ENUMS( CURRENT_PATH_TYPES )
   Q_PROPERTY( QString titleText READ getTitleText NOTIFY titleTextChanged )
   Q_PROPERTY( int titleWidth READ getTitleWidth WRITE setTitltWidth )
   Q_PROPERTY( QFont font READ getFont WRITE setFont )

public:

   enum TITLE_TYPES
   {
      TITLE_TYPE_CURRENT_PATH,
      TITLE_TYPE_KEYPAD
   };

   enum CURRENT_PATH_TYPES
   {
      CURRENT_PATH_FOLDER,
      CURRENT_PATH_FILE
   };

   TitleProvider();
   ~TitleProvider();

   QString getTitleText();

   void setTitleType( TITLE_TYPES type );
   void setCurrentPath( const QString & value );
   void setCurrentPlayedFile( const QString & value );

   int getTitleWidth();
   void setTitltWidth( int value );
   QFont getFont();
   void setFont( const QFont & font );

signals:
   void titleTextChanged();

private slots:
   void update();

private:
   QTimer              *m_pTimer;

   TITLE_TYPES         m_CurrentType;
   CURRENT_PATH_TYPES  m_CurrentPathType;

   QString             m_KeyPadTitle;
   QString             m_CurrentPath;
   QString             m_CurrentPlayedFile;

   int                 m_TitleWidth;
   QFont               m_Font;
};

#endif // DHAVN_APPFILEMANAGER_TITLEPROVIDER_H
