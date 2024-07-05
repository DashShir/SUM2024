/* FILE NAME  : def.h
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Common declaration module.
 */

#ifndef __def_h_
#define __def_h_

#pragma warning(disable: 4244 4305)

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */
#include <stdlib.h>

#include "mth/mth.h"
 
/* 64 bit integer data type */
typedef long long INT64;
typedef unsigned long long UINT64;

/* Random number generator (0..1) function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (FLT) result random value.
 */
__inline FLT Rnd0( VOID )
{
  return rand() / (FLT)RAND_MAX;
} /* End of 'Rnd0' function */

/* Random number generator (-1..1) function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (FLT) result random value.
 */
__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'Rnd1' function */

#endif /* __def_h_ */
 
/* END OF 'def.h' FILE */
 