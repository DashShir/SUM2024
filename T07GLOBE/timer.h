/* FILE NAME:   globe.c
 * PROGRAMMER:  DS4
 * LAST UPDATE: 14.06.2024
 * PURPOSE:     Draw sphere.
 */

#ifndef __timer_h_
#define __timer_h_

#include <commondf.h>

/* Externings */

extern DOUBLE
  GLB_Time,              /* Full time */
  GLB_DeltaTime,         /* One ... time */
  GLB_FPS;               /* Frame per second. */

extern BOOL GLB_IsPause; /* Pause or not */

/* Timer initialisation function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID GLB_TimerInit( VOID );

/* Timer responsing function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID GLB_TimerResponse( VOID );

#endif /* __timer_h_ */

/* END OF 'timer.h' FILE */
