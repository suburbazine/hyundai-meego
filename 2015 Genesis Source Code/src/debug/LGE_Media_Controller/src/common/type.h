#ifndef _TYPE_H
#define _TYPE_H

#ifndef NULL
#  ifdef __cplusplus
#    define NULL        (0L)
#  else /* !__cplusplus */
#    define NULL        ((void*) 0)
#  endif /* !__cplusplus */
#endif

#if !defined WIN32 && !defined _WIN_CE
	#ifndef BOOL
	typedef unsigned char	BOOL;
	#endif

	#ifndef BYTE
	typedef unsigned char	BYTE;
	#endif

	#ifndef DWORD
	typedef unsigned long	DWORD;
	#endif

	#ifndef WORD
	typedef unsigned short	WORD;
	#endif
#else
	#include <windows.h>
#endif

#ifndef TRUE
#define	TRUE			(!FALSE)
#define	FALSE			(0)
#endif

typedef char			INT1;
typedef short			INT2;
typedef int				INT4;

typedef unsigned char	UINT1;
typedef unsigned short	UINT2;
typedef unsigned int	UINT4;

typedef float			FLOAT4;
typedef double			FLOAT8;

#define NULL4			(0x0F)			// 4-bit NULL Value
#define NULL8			(0xFF)			// 8-bit NULL Value
#define NULL12			(0x0FFF)		// 12-bit NULL Value
#define NULL16			(0xFFFF)		// 16-bit NULL Value
#define NULL32			(0xFFFFFFFF)	// 32-bit NULL Value

struct BCD2
{
    union {
        UINT1            byte;
        struct {
            UINT1        lower : 4;
            UINT1        upper : 4;
        } BCD;
    };
};

struct BCD4
{
    union {
        UINT2            word;
        struct {
            UINT2        lower2 : 4;
            UINT2        upper2 : 4;
            UINT2        lower1 : 4;
            UINT2        upper1 : 4;
        } BCD;
    };
};

#endif
