/**
 * event.h
 *
 * Implementation of the Class Event
 */

#if !defined(EA_BECEDDCD_0C7E_4771_9B88_97BD6982995A__INCLUDED_)
#define EA_BECEDDCD_0C7E_4771_9B88_97BD6982995A__INCLUDED_

#include "DHAVN_AppFramework_Def.h"

#include <QMetaType>
#include <QEvent>
#include <QDBusArgument>

const EVT_COMMAND_T EVT_COMMAND_INVALID = 0xFFFFFFFF;

//Forward class declaration
class EventPrivate;

/**
 * Event structure
 * This class is used to exchange information via the AppFramework/UISH event system
 */
class APP_FRAMEWORK_EXPORT Event: public QEvent
{

public:
   /** Default constructor */
   Event();

   Event( EVT_ID_T aEventId, const QByteArray &aData = QByteArray() ); // Broadcast event

   Event( EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, QByteArray const &aData = QByteArray() ); // Private event destination

   Event( EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, EVT_COMMAND_T aCommand, QByteArray const &aData = QByteArray() );

   Event( const Event &aEvent );

   virtual ~Event();
   // Setters
   void SetSource( APP_ID_T aSource );

   void SetEventInt( EVT_ID_T aIntEventId );

   void SetTarget( APP_ID_T aTarget );

   void SetCommand( EVT_COMMAND_T aCommand );


   APP_ID_T         GetSource() const;

   APP_ID_T         GetTarget() const;

   EVT_ID_T         GetEventId() const;

   EVT_COMMAND_T    GetCommand() const;

   const QByteArray& Data() const;


private:

   friend QDataStream & operator<<( QDataStream &arg, const Event &val );

   friend  QDataStream & operator>>( QDataStream &arg, Event &val );

   friend QDBusArgument & operator<<( QDBusArgument &arg, const Event &val );
   friend const QDBusArgument & operator>>( const QDBusArgument &arg, Event &val );

private:
    // future enhancement
   virtual void Reserved_1();
   virtual void Reserved_2();

private:
   //D-Pointer to the EventPrivate class
    EventPrivate * d_ptr;
    Q_DECLARE_PRIVATE(Event)
	
};

Q_DECLARE_METATYPE(Event)

#endif // !defined(EA_BECEDDCD_0C7E_4771_9B88_97BD6982995A__INCLUDED_)
