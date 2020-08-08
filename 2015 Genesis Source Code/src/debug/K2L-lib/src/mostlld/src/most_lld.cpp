#include <semaphore.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h> /* TMO */
#include <LLD.h>

//TMO: includes for ioctl on mn0
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h> /* for ARPHRD_ETHER */

#include <ioh_mlb_api.h>


// TMO: mostnet ioctl defines
#define SIOCSETLINKSTATE  SIOCDEVPRIVATE
enum {
  MOSTNET_LINKSTATE_OFF = 0,
  MOSTNET_LINKSTATE_ON
};


//------------------------------------------------------------------------------
// LLD configuration

namespace lld {

    enum {
        DEVICE_ADDRESS = 0x190,
        MLB_SPEED = CLK1024FS,

        CRX_CHANNEL_ADDRESS = 2,
        CTX_CHANNEL_ADDRESS = 4,
        ARX_CHANNEL_ADDRESS = 6,
        ATX_CHANNEL_ADDRESS = 8,
        IPC_TX_CHANNEL_ADDRESS = 10,
        IPC_RX_CHANNEL_ADDRESS = 12,
        

        CRX_BUFFER_SIZE = 128,
        CTX_BUFFER_SIZE = 128,
        ARX_BUFFER_SIZE = 1652,
        ATX_BUFFER_SIZE = 1652,
        IPC_RX_BUFFER_SIZE = 1040,
        IPC_TX_BUFFER_SIZE = 1040
    };

const char *const devices[] = {
   // "/dev/mlb-ch0", // ctrl rx
   // "/dev/mlb-ch1", // ctrl tx
    "/dev/mlb-ch2", // async rx
    "/dev/mlb-ch3", // async tx
    "/dev/mlb-ch5", // IPC rx
    "/dev/mlb-ch6"  // IPC tx 
};

// TMO:
const char *net_device = "mn0";

} // namespace lld

// - to enable LLD traces define macro LLD_VERBOSE in project settings
// - to reroute traces to your destination adjust macro LLD_TRACE or functions lld::log() and lld::error()
//#define LLD_VERBOSE
#if defined(LLD_VERBOSE)
    #include <stdio.h>

    static sem_t lld_trace_sem;

    #define LLD_TRACE(params)  printf params

#if defined( LLD_DUMP_TRAFFIC )
    #define LLD_TRACE_SEMA_INIT()  do sem_init( &lld_trace_sem, true, 1 ); while(false)
    #define LLD_TRACE_SEMA_LOCK()  do sem_wait( &lld_trace_sem ); while(false)
    #define LLD_TRACE_SEMA_UNLOCK()  do sem_post( &lld_trace_sem ); while(false)
#else
    #define LLD_TRACE_SEMA_INIT()  do;while(false)
    #define LLD_TRACE_SEMA_LOCK()  do;while(false)
    #define LLD_TRACE_SEMA_UNLOCK()  do;while(false)
#endif

#else
    #define LLD_TRACE(params)  do;while(false)
    #define LLD_TRACE_SEMA_INIT()  do;while(false)
#endif


namespace lld {

static inline void log(const char *message) {
    LLD_TRACE( ( "lld: %s\n", message ) );
}

static bool error(const char *message) {
    LLD_TRACE( ( "lld.error: %s\n", message ) );
    return false;
}

} // namespace lld

//------------------------------------------------------------------------------

namespace lld {

//------------------------------------------------------------------------------
// helpers and globals

struct rx_t {
    rx_t(uint16_t buff_size) : handle(-1) {
        buffer_size = buff_size;
	data_available = false;
    }

    pthread_cond_t cond;
    pthread_mutex_t cs_data;
    pthread_mutex_t cs_cond;
    bool interrupt_handled;
    bool data_available;
    int handle;
    uint16_t length; // unused
    uint8_t *buffer_ptr;
    uint16_t buffer_size;
};

struct tx_t {
    tx_t(const LLDStubBuffer_t buff_type): handle(-1), buffer_type( buff_type )
    {
        data_cptr = 0;
    }

    int handle;
    uint16_t length;
    const uint8_t *data_cptr;
    const LLDStubBuffer_t buffer_type;
};


//TMO:
struct mostnet_t {
  mostnet_t(const char *devname): handle(-1)  {
    strcpy(ifr_onoff.ifr_name, devname);
    strcpy(ifr_mac.ifr_name, devname);
    ifr_mac.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    // ifr_onoff.ifr_data will be written before ioctl
    // ifr_mac.ifr_hwaddr.sa_data will be written before ioctl
  }
  int handle;
  struct ifreq ifr_onoff;
  struct ifreq ifr_mac;
};

static bool poll_rx(rx_t &rx);


// represents the RX ISR thread
struct service_rx_t {
    service_rx_t(rx_t &rx): _rx( rx ), thread_running(false) {}

    int start_thread() {
      int r;
	r = pthread_mutex_init(&_rx.cs_data, NULL);
	if (r) 
	{
	    return r;
	}
	r = pthread_mutex_init(&_rx.cs_cond, NULL);
	if (r) 
	{
	    return r;
	    pthread_mutex_destroy(&_rx.cs_data);

	}
	r = pthread_cond_init(&_rx.cond, NULL);
	if (r)
	{
	    pthread_mutex_destroy(&_rx.cs_data);
	    pthread_mutex_destroy(&_rx.cs_cond);
	    return r;
	}
        r = pthread_create( &thread_handle, 0, thread, this ); 
	if (r)
	{
	    pthread_cond_destroy(&_rx.cond);
	    pthread_mutex_destroy(&_rx.cs_data);
	    pthread_mutex_destroy(&_rx.cs_cond);
	}
	return r;
    }

    void stop_thread() {
        thread_running = false; 
	void* pvExitValue = NULL;
	pthread_join( thread_handle, &pvExitValue );
	pthread_cond_destroy(&_rx.cond);
	pthread_mutex_destroy(&_rx.cs_data); 
	pthread_mutex_destroy(&_rx.cs_cond);
    }
    
    bool poll() { 
        return poll_rx( _rx ); 
    }

    static void *thread(void *arg) {
        service_rx_t *const p = (service_rx_t *)arg;
	p->thread_running = true;
        while( p->thread_running ) {
	  if (!p->poll()) break;
	}
	lld::log("rx thread exiting");
	return 0;
    }

    rx_t &_rx;
    bool thread_running;
    pthread_t thread_handle;

};

static struct globals {
    globals():

#if defined(LLD_ENABLE_CONTROL)
        crx( CRX_BUFFER_SIZE ),
        ctx( LLDSTUB_BUFFER_CTL ),
        crx_service( crx ),
#endif

#if defined(LLD_ENABLE_ASYNC)
        arx( ARX_BUFFER_SIZE ),
        atx( LLDSTUB_BUFFER_ATX ),
        arx_service( arx ),
#endif
        ipcrx(-1),
        ipctx(-1),
        sys_handle( -1 ),
        mn( net_device ),
        initDoneCondMutex(),
        initDoneCond(),
        initDone(false)
    {
    	if (0 != pthread_mutex_init(&initDoneCondMutex, NULL))
    	{
    		error( "cannot initialize initDoneCondMutex" );
    	}
    	if (0 != pthread_cond_init(&initDoneCond, NULL))
    	{
    		error( "cannot initialize initDoneCond" );
    	}
    }

#if defined(LLD_ENABLE_CONTROL)
    rx_t crx;
    tx_t ctx;
    service_rx_t crx_service;
#endif

#if defined(LLD_ENABLE_ASYNC)
    rx_t arx;
    tx_t atx;
    service_rx_t arx_service;
#endif
    
#if defined (LLD_ENABLE_IPC)
    int ipcrx;
    int ipctx;
#endif

    int sys_handle;

    // TMO:
    mostnet_t mn;

    pthread_mutex_t initDoneCondMutex;
    pthread_cond_t initDoneCond;
    bool initDone;
} g;


//------------------------------------------------------------------------------
// receive part


static bool poll_rx(rx_t &rx)
// context: ISR thread
{
    struct pollfd pollcrx[1];
    int r;
  
    pollcrx[0].fd = rx.handle;
    pollcrx[0].events = POLLIN;
    r = poll(pollcrx, 1, -1); // wait infinitely

    if (r > 0) 
    {
      if (pollcrx[0].revents & POLLIN)
      {
	  pthread_mutex_lock(&rx.cs_data);
	  rx.data_available = true;
	  pthread_mutex_unlock(&rx.cs_data);


	  pthread_mutex_lock(&rx.cs_cond);
	  
	  // notify mostdriverthread (set event)
	  //   mutex is still locked so there's no 
	  //   signal() before the wait() below
	  LLDStubOnInterrupt();

	  // wait until mostdriverthread has read 
	  // data and cleared event
	  rx.interrupt_handled = false;
	  while (!rx.interrupt_handled)
	  {
	      pthread_cond_wait(&rx.cond, &rx.cs_cond);
	  }
	  pthread_mutex_unlock(&rx.cs_cond);
      }
    }

    return (r > 0 || r == -EINTR);
}

static void nonblocking_read(rx_t &rx)
// context: mostdriverthread
{
    ssize_t bytes_read;
    
    do {
        bytes_read = ::read( rx.handle, rx.buffer_ptr, rx.buffer_size );
        if( bytes_read <= 0 ) {
            error( "read() failed" );
        }
    } while( bytes_read <= 0 );

#if defined( LLD_DUMP_TRAFFIC )
    LLD_TRACE_SEMA_LOCK();
    LLD_TRACE( ( "most.app.rx: [%d]", bytes_read ) );
    for( int i = 0; i < bytes_read; i++ )
        LLD_TRACE( ( " %02X", rx.buffer_ptr[i] ) );
    LLD_TRACE( ( "\n" ) );
    LLD_TRACE_SEMA_UNLOCK();
#endif

    rx.length = bytes_read; // unused

    pthread_mutex_lock(&rx.cs_data);
    rx.data_available = false;
    pthread_mutex_unlock(&rx.cs_data);
}

static bool service_rx(rx_t &rx)
// context: mostdriverthread
{
    bool success = false;
    LLDPacketBufferHandle_t handle = 0;
    bool data_available;

    pthread_mutex_lock(&rx.cs_data);
    data_available = rx.data_available;
    pthread_mutex_unlock(&rx.cs_data);

    if (data_available)
    {
      if( !LLDStubGetPacketBuffer( rx.buffer_size, &handle, &rx.buffer_ptr ) )
      {
	  error( "LLDStubGetPacketBuffer() failed" );
	  // the service function is called by lldstub again when buffer is available
      }
      else
      {
	  nonblocking_read( rx );
	  LLDStubOnPacketReceived( handle );
	  success = true;
      }
    }

    return success;
}

static void signal_thread(rx_t &rx)
{
    pthread_mutex_lock( &rx.cs_cond );
    rx.interrupt_handled = true;
    pthread_cond_signal( &rx.cond );
    pthread_mutex_unlock( &rx.cs_cond );
}


//------------------------------------------------------------------------------
// transmit part

static bool poll_tx(tx_t &tx)
{
    struct pollfd pollctx[1];
    int r;
  
    pollctx[0].fd = tx.handle;
    pollctx[0].events = POLLOUT;
    do {
	r = poll(pollctx, 1, -1); // wait infinitely
    } while (-EINTR == r);

    return (r > 0);
}

static bool nonblocking_write(tx_t &tx)
{
    int bytes_written;

    bool success = false;

    do
    {
      bytes_written =  ::write( tx.handle, tx.data_cptr, tx.length );

      // check if whole packet was transmitted
      if ( bytes_written > 0 && bytes_written < tx.length )
      {
	// incomplete packet was transmitted
	// try to send remaining bytes
	tx.data_cptr += bytes_written;
	tx.length -= bytes_written;
	bytes_written = -EAGAIN;
      } 
    }
    while ( -EAGAIN == bytes_written );

    if( bytes_written > 0 )
    {
#if defined( LLD_DUMP_TRAFFIC )
        LLD_TRACE_SEMA_LOCK();
        LLD_TRACE( ( "most.app.tx: [%d]", bytes_written ) );
        for( int i = 0; i < bytes_written; i++ )
            LLD_TRACE( ( " %02X", tx.data_cptr[i] ) );
        LLD_TRACE( ( "\n" ) );
        LLD_TRACE_SEMA_UNLOCK();
#endif
	success = true;
    }

    // buffer can be released synchronously to netservices thread
    LLDStubReleaseTxBuffer( tx.buffer_type );
    tx.data_cptr = 0;

    return success;
}


//------------------------------------------------------------------------------
// service

static void lld_service()
{
#if defined(LLD_ENABLE_CONTROL)
    service_rx( g.crx );
#endif

#if defined(LLD_ENABLE_ASYNC)
    service_rx( g.arx );
#endif
}


//------------------------------------------------------------------------------
// mostnet part

static void mostnet_link_on(bool on)
{
  char state;

  if (on) state = MOSTNET_LINKSTATE_ON;
  else    state = MOSTNET_LINKSTATE_OFF;

  g.mn.ifr_onoff.ifr_data = &state;

  if (g.mn.handle >= 0) {
    ::ioctl(g.mn.handle, SIOCSETLINKSTATE, &g.mn.ifr_onoff);
  }
}

static void mostnet_eui48(const uint8_t eui48[])
{
  int i;
  for (i=0; i<6; i++) {
    g.mn.ifr_mac.ifr_hwaddr.sa_data[i] = eui48[i];
  }

  if (g.mn.handle >= 0) {
    ::ioctl(g.mn.handle, SIOCSIFHWADDR, &g.mn.ifr_mac);
  }
}


//------------------------------------------------------------------------------
// init part

struct channel_settings_t {
    channel_settings_t(int direction, int channel_address, int max_message_size) {
        x.direction = direction;
        x.channel_address = channel_address;
        x.max_message_size = max_message_size;
    }
    ioh_mlb_channel_setting x;
};

struct system_settings_t {
    system_settings_t(int device_address, int mlb_speed) {
        x.device_address = device_address;
        x.mlb_speed = mlb_speed;
    }
    ioh_mlb_system_setting x;
};

static int startup_ioh_mlb()
{
    int device_index = -1;
    int const configured_device_names = sizeof(devices)/sizeof(*devices);

    g.sys_handle = ::open( "/dev/mlb-sys", O_RDWR );
    if( g.sys_handle < 0 )
        return error( "cannot open mlb-sys" );

#if defined(LLD_ENABLE_CONTROL)
    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.crx.handle = ::open( devices[device_index], O_RDWR );
    if( g.crx.handle < 0 )
        return error( "cannot open device for control rx" );

    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.ctx.handle = ::open( devices[device_index], O_RDWR );
    if( g.ctx.handle < 0 )
        return error( "cannot open device for control tx" );
#endif

#if defined(LLD_ENABLE_ASYNC)
    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.arx.handle = ::open( devices[device_index], O_RDWR );
    if( g.arx.handle < 0 )
        return error( "cannot open device for async rx" );

    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.atx.handle = ::open( devices[device_index], O_RDWR );
    if( g.atx.handle < 0 )
        return error( "cannot open device for async tx" );
#endif

    int res = ::ioctl( g.sys_handle, IOH_MLB_SYS_IOCTL_SET_SYSTEM,
        &system_settings_t( DEVICE_ADDRESS, MLB_SPEED ).x );
    if( res )
        return error( "IOH_MLB_SYS_IOCTL_SET_SYSTEM failed" );

#if defined(LLD_ENABLE_CONTROL)
    res = ::ioctl( g.crx.handle, IOH_MLB_CH_IOCTL_SET_CONTROL_CHANNEL,
        &channel_settings_t( RX, CRX_CHANNEL_ADDRESS, CRX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for control rx failed" );

    res = ::ioctl( g.ctx.handle, IOH_MLB_CH_IOCTL_SET_CONTROL_CHANNEL,
        &channel_settings_t( TX, CTX_CHANNEL_ADDRESS, CTX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for control tx failed" );
#endif

#if defined(LLD_ENABLE_ASYNC)
    res = ::ioctl( g.arx.handle, IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL,
        &channel_settings_t( RX, ARX_CHANNEL_ADDRESS, ARX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for async rx failed" );

    res = ::ioctl( g.atx.handle, IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL,
        &channel_settings_t( TX, ATX_CHANNEL_ADDRESS, ATX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for async tx failed" );
#endif

#if defined(LLD_ENABLE_IPC)
    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.ipctx = ::open( devices[device_index], O_RDWR );
    if( g.ipctx < 0 )
        return error( "cannot open device for ipc tx" );

    if( ++device_index >= configured_device_names )
        return error( "too few devices configured" );

    g.ipcrx = ::open( devices[device_index], O_RDWR );
    if( g.ipcrx < 0 )
        return error( "cannot open device for ipc rx" );

    res = ::ioctl( g.ipcrx, IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL,
        &channel_settings_t( RX, IPC_RX_CHANNEL_ADDRESS, IPC_RX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for ipc rx failed" );

    res = ::ioctl( g.ipctx, IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL,
        &channel_settings_t( TX, IPC_TX_CHANNEL_ADDRESS, IPC_TX_BUFFER_SIZE ).x );
    if( res )
        return error( "IOH_MLB_CH_IOCTL_SET_ASYNCHRONOUS_CHANNEL for ipc tx failed" );
#endif
    
    res = ::ioctl( g.sys_handle, IOH_MLB_SYS_IOCTL_START );
    if( res )
        return error( "IOH_MLB_SYS_IOCTL_START failed" );

    //TMO
    g.mn.handle = ::socket(AF_INET, SOCK_DGRAM, 0);
    
    return true;
}

static void shutdown_ioh_mlb()
{

#if defined(LLD_ENABLE_CONTROL)
    close(g.crx.handle);
    g.crx.handle = -1;

    close(g.ctx.handle);
    g.ctx.handle = -1;
#endif

#if defined(LLD_ENABLE_ASYNC)
    close(g.arx.handle);
    g.arx.handle = -1;

    close(g.atx.handle);
    g.atx.handle = -1;
#endif

#if defined(LLD_ENABLE_IPC)
    close(g.ipctx);
    g.ipctx = -1;

    close(g.ipcrx);
    g.ipcrx = -1;
#endif

    close(g.sys_handle);
    g.sys_handle = -1;
  
}

} // namespace lld


//------------------------------------------------------------------------------
// LLD implementation

uint8_t LLDInitialize(eLLDInterface *interface_type_ptr)
{
    LLD_TRACE_SEMA_INIT();

    if( interface_type_ptr )
        *interface_type_ptr = LLD_INTERFACE_MLB;

    if( !lld::startup_ioh_mlb() )
        return lld::error( "startup_ioh_mlb() failed" );

#if defined(LLD_ENABLE_CONTROL)
    if( lld::g.crx_service.start_thread() != 0  )
        return lld::error( "lld::gg.crx_service.start_thread() failed" );
#endif

#if defined(LLD_ENABLE_ASYNC)
    if( lld::g.arx_service.start_thread() != 0  )
        return lld::error( "lld::g.arx_service.start_thread() failed" );
#endif

    //Signal that init is done in case someone needs this
    pthread_mutex_lock(&(lld::g.initDoneCondMutex));
    lld::g.initDone = true;
    pthread_cond_signal(&(lld::g.initDoneCond));
    pthread_mutex_unlock(&(lld::g.initDoneCondMutex));

    return true;
}

uint8_t LLDTerminate(void)
{
  
#if defined(LLD_ENABLE_CONTROL)
    lld::g.crx_service.stop_thread();
#endif

#if defined(LLD_ENABLE_ASYNC)
    lld::g.arx_service.stop_thread();
#endif

    lld::shutdown_ioh_mlb();

    pthread_mutex_lock(&(lld::g.initDoneCondMutex));
    lld::g.initDone = false;
    pthread_mutex_unlock(&(lld::g.initDoneCondMutex));

    return true;
}

uint8_t LLDEnable(void)
{
    lld::log( "start ns" );

    // there could be unserviced rx messages after LLDStubGetPacketBuffer failed
    lld::lld_service();

    return true;
}

uint8_t LLDSendTriggerMessage(void)
{
    // rsc: We have to send the trigger message in order to receive NetState message from inic.
    uint8_t triggerPayload[] = { 0x00, 0x03, 0x02, 0x08, 0x40, 0x00, 0x00, 0x00 };
    int const bytes_written = ::write( lld::g.atx.handle, triggerPayload, sizeof(triggerPayload) );
    
    if (bytes_written < static_cast<int>(sizeof(triggerPayload)))
    {
        return lld::error( "Failed to send trigger message." );
    }
    
    return true;
}

uint8_t LLDDisable(void)
{
    lld::log( "stop ns" );

    return true;
}

uint8_t LLDTransmitPacket(const uint8_t *data_cptr, uint32_t length, eLLDTransport transport_type)
{

    bool success;

    if( data_cptr == 0 || length == 0 )
    {
        return false;
    }

    switch( transport_type )
    {
#if defined(LLD_ENABLE_CONTROL)
    case LLD_TRANSPORT_CONTROL_CHANNEL:
        {
            lld::g.ctx.data_cptr = data_cptr;
            lld::g.ctx.length = length;
            success = poll_tx(lld::g.ctx);
            if (success)
                success = nonblocking_write(lld::g.ctx);
        }
        break;
#endif

#if defined(LLD_ENABLE_ASYNC)
    case LLD_TRANSPORT_ASYNC_CHANNEL:
        {
            lld::g.atx.data_cptr = data_cptr;
            lld::g.atx.length = length;
            success = poll_tx(lld::g.atx);
            if (success)
                success = nonblocking_write(lld::g.atx);
        }
        break;
#endif

    default:
        success = false;
    }

    return success;
}

uint8_t LLDResetINIC(void)
{
    return lld::error( "LLDResetINIC() is not implemented" );
}

uint8_t LLDPollInterrupt(uint32_t timeoutMilliseconds /* -1 for infinite */)
{
    return LLDStubOnInterrupt();
}

uint8_t LLDServiceInterrupt(void)
{
    lld::lld_service();

    return true;
}

uint8_t LLDInterruptDone(void)
{
#if defined(LLD_ENABLE_CONTROL)
    signal_thread( lld::g.crx );
#endif

#if defined(LLD_ENABLE_ASYNC)
    signal_thread( lld::g.arx );
#endif
    return true;
}

//TMO:
void LLDSetMostNetStatusOn(void)
{
    lld::mostnet_link_on(true);
}
void LLDSetMostNetStatusOff(void)
{
    lld::mostnet_link_on(false);
}
void LLDSetMostNetEUI48(const uint8_t eui48[])
{
    lld::mostnet_eui48(eui48);
}

int LLDGetIPCRxHandle()
{
    int handle = -1;
    
#if defined(LLD_ENABLE_IPC)
    handle = lld::g.ipcrx;
#endif
    
    return handle;
}

int LLDGetIPCTxHandle()
{
    int handle = -1;
    
#if defined(LLD_ENABLE_IPC)
    handle = lld::g.ipctx;
#endif
    
    return handle;
}

void LLDWaitForInitDone(void)
{
    pthread_mutex_lock(&(lld::g.initDoneCondMutex));
    while(lld::g.initDone == false)
    {
    	pthread_cond_wait (&(lld::g.initDoneCond), &(lld::g.initDoneCondMutex));
    }
    pthread_mutex_unlock(&(lld::g.initDoneCondMutex));
}
