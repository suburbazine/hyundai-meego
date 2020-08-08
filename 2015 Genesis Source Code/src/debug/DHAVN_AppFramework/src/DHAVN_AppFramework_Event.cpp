#include "DHAVN_AppFramework_Event_Def.h"

/**
 * Event Private class definition
 */
class EventPrivate
{
private:

    /** Constructor */
    EventPrivate(Event* pParent)
    :m_EventId(EVT_NONE), 
    m_Source(APP_NONE), m_Target(APP_NONE), m_Command(EVT_COMMAND_INVALID),
    q_ptr(pParent) 
    {}

    EventPrivate( Event* pParent, EVT_ID_T aEventId, const QByteArray &aData )
    :m_Data(aData), m_EventId(aEventId), 
    m_Source(APP_NONE), m_Target(APP_NONE), m_Command(EVT_COMMAND_INVALID),
    q_ptr(pParent) 
    {}

    EventPrivate( Event* pParent, EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, QByteArray const &aData )
    :m_Data(aData), m_EventId(aEventId), 
    m_Source(aSource), m_Target(aTarget), m_Command(EVT_COMMAND_INVALID),
    q_ptr(pParent)
    {}

    EventPrivate( Event* pParent, EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, EVT_COMMAND_T aCommand, QByteArray const &aData )
    :m_Data(aData), m_EventId(aEventId), m_Source(aSource), m_Target(aTarget), m_Command(aCommand),
    q_ptr(pParent)
    {}

    EventPrivate( Event* pParent, const Event &aEvent )
    :m_Data(aEvent.d_ptr->m_Data), m_EventId(aEvent.d_ptr->m_EventId), 
    m_Source(aEvent.d_ptr->m_Source), m_Target(aEvent.d_ptr->m_Target), m_Command(aEvent.d_ptr->m_Command),
    q_ptr(pParent) {}

    /** Destructor */
    virtual ~EventPrivate() {}

public:
   QByteArray m_Data;
   EVT_ID_T m_EventId;
   APP_ID_T m_Source;
   APP_ID_T m_Target;
   EVT_COMMAND_T m_Command;

   //Q-Pointer to the AppEngineBase class
   Event * const q_ptr;
   Q_DECLARE_PUBLIC(Event)
};

Event::Event() : QEvent( QEvent::User )
{
    d_ptr = new EventPrivate(this);
}

Event::Event( const Event& aEvent )
: QEvent( QEvent::User )
{
    d_ptr = new EventPrivate(this, aEvent);
}

Event::Event( EVT_ID_T aEventId, const QByteArray &aData )
: QEvent( QEvent::User )
{
    d_ptr = new EventPrivate(this, aEventId, aData);
}

Event::Event( EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, QByteArray const &aData )
: QEvent( QEvent::User )
{
    d_ptr = new EventPrivate(this, aEventId, aSource, aTarget, aData);
}


Event::Event( EVT_ID_T aEventId, APP_ID_T aSource, APP_ID_T aTarget, EVT_COMMAND_T aCommand, QByteArray const &aData )
: QEvent( QEvent::User )
{
    d_ptr = new EventPrivate(this, aEventId, aSource, aTarget, aCommand, aData);
}

Event::~Event()
{
    delete d_ptr;
}

void Event::SetSource( APP_ID_T aSource ) { d_ptr->m_Source = aSource; }
void Event::SetEventInt( EVT_ID_T aIntEventId ) { d_ptr->m_EventId = aIntEventId; }
void Event::SetTarget( APP_ID_T aTarget ) { d_ptr->m_Target = aTarget; }
void Event::SetCommand( EVT_COMMAND_T aCommand ) { d_ptr->m_Command = aCommand; }
APP_ID_T Event::GetSource() const { return d_ptr->m_Source; }
APP_ID_T Event::GetTarget() const { return d_ptr->m_Target; }
EVT_ID_T Event::GetEventId() const { return d_ptr->m_EventId; }
EVT_COMMAND_T Event::GetCommand() const { return d_ptr->m_Command; }
const QByteArray& Event::Data() const { return d_ptr->m_Data; }

APP_FRAMEWORK_EXPORT
QDataStream & operator<<( QDataStream &arg, const Event &val )
{
   arg << (int)val.d_ptr->m_EventId << (int)val.d_ptr->m_Source << (int)val.d_ptr->m_Target << (int)val.d_ptr->m_Command << val.d_ptr->m_Data;
   
   return arg;
}

APP_FRAMEWORK_EXPORT
QDataStream & operator>>( QDataStream &arg, Event &val )
{
   int target = 0, source = 0, eventId = 0, command = EVT_COMMAND_INVALID;
   
   arg >> eventId >> source >> target >> command >> val.d_ptr->m_Data;
   
   val.d_ptr->m_EventId = static_cast< EVT_ID_T >( eventId );
   val.d_ptr->m_Source  = static_cast< APP_ID_T >( source );
   val.d_ptr->m_Target  = static_cast< APP_ID_T >( target );
   val.d_ptr->m_Command = static_cast< EVT_COMMAND_T >( command );

   return arg;
}

APP_FRAMEWORK_EXPORT
QDBusArgument & operator<<( QDBusArgument &arg, const Event &val )
{
    arg.beginStructure();
    arg << (int)val.d_ptr->m_EventId;
    arg << (int)val.d_ptr->m_Source;
    arg << (int)val.d_ptr->m_Target;
    arg << (int)val.d_ptr->m_Command;
    arg << val.d_ptr->m_Data;
    arg.endStructure();
    return arg;
}

APP_FRAMEWORK_EXPORT
const QDBusArgument & operator>>( const QDBusArgument &arg, Event &val )
{
    int target = 0, source = 0, eventId = 0, command = EVT_COMMAND_INVALID;

    arg.beginStructure();
    arg >> eventId;
    arg >> source;
    arg >> target;
    arg >> command;
    arg >> val.d_ptr->m_Data;
    arg.endStructure();

    val.d_ptr->m_EventId = static_cast< EVT_ID_T >( eventId );
    val.d_ptr->m_Source  = static_cast< APP_ID_T >( source );
    val.d_ptr->m_Target  = static_cast< APP_ID_T >( target );
    val.d_ptr->m_Command = static_cast< EVT_COMMAND_T >( command );
    return arg;
}

void 
Event::Reserved_1()
{
}

void 
Event::Reserved_2()
{
}

