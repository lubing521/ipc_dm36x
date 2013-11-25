/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

/**
  \file tistdtypes.h
  \brief Primitive C types
*/





#ifndef _TISTDTYPES_H_
#define _TISTDTYPES_H_

typedef unsigned int Bool32;               ///< Boolean type

#ifndef xdc_std__include

#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES

#ifndef TRUE

typedef short   Bool;                 ///< Boolean type

#define TRUE		(1)        ///< Boolean value : TRUE
#define FALSE		(0)        ///< Boolean value : FALSE

#endif

/* unsigned quantities */
typedef unsigned long long Uint64;      ///< Unsigned 64-bit integer
typedef unsigned int Uint32;            ///< Unsigned 32-bit integer
typedef unsigned short Uint16;          ///< Unsigned 16-bit integer
typedef unsigned char Uint8;            ///< Unsigned  8-bit integer

/* signed quantities */
typedef long long Int64;               ///< Signed 64-bit integer
typedef int Int32;                     ///< Signed 32-bit integer
typedef short Int16;                   ///< Signed 16-bit integer
typedef char Int8;                     ///< Signed  8-bit integer

#endif  

#endif /* _TI_STD_TYPES */

#endif /* _TISTDTYPES_H_ */



