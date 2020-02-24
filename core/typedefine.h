#ifndef _STREAMINGNOLOGY_CORE_TYPEDEFINE_H_
#define _STREAMINGNOLOGY_CORE_TYPEDEFINE_H_

/*WIN32 config*/
#ifdef _WIN32
#include <stdint.h>
/*
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
*/
#define fseek64   _fseeki64
#define ftell64   _ftelli64
typedef uint32_t uint;
#else /*end of WIN32 config*/

/*UNIX likes*/
#include <stdint.h>
#include <sys/types.h>
#include <strings.h>
typedef char             int8;		    /* 8 bit signed */
typedef unsigned char    byte;        /* 8 bit unsigned */
typedef unsigned char    uint8;       /* 8 bit unsigned */
typedef unsigned char    uchar;       /* 8 bit unsigned */
typedef short            int16;       /* 16 bit signed */
typedef unsigned short   uint16;	    /* 16 bit unsigned */
typedef unsigned short   ushort;	    /* 16 bit unsigned */
typedef int              int32;       /* 32 bit signed */
typedef unsigned int     uint;
typedef unsigned int     uint32;      /* 32 bit unsigned */
typedef unsigned long    ulong;       /* 32 bit unsigned */
typedef __int64_t        int64;       /* 64 bit signed */
typedef __int64_t        longlong;    /* 64 bit signed */
typedef __uint64_t       uint64;	    /* 64 bit unsigned */
typedef __uint64_t       ulonglong;   /* 64 bit unsigned */
#define fseek64          fseeko
#define ftell64          ftello
typedef unsigned char       uchar;
typedef uint8_t             byte;
typedef unsigned int        uint;
typedef int64_t             longlong;
typedef uint64_t            ulonglong;

#define  stricmp    strcasecmp
#define _stricmp    strcasecmp
#define _strnicmp   strncasecmp
#define sscanf_s    sscanf
#define sprintf_s   snprintf
#define strncpy_s   strncpy
#define _mkdir      mkdir
#define _access     access

#endif /*end of UNIX likes*/

#define __STDC_CONSTANT_MACROS

#endif // _STREAMINGNOLOGY_CORE_TYPEDEFINE_H_
