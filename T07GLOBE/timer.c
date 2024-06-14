/* FILE NAME: timer.h
 * PROGRAMMER: DS4
 * LAST UPDATE: 14.06.2024
 * PURPOSE: Draw sphere.
 */

#include <time.h>

#include "timer.h"

/* Forward references */
DOUBLE                 
  GLB_Time,             /* Full time */
  GLB_DeltaTime,        /* One ... time */
  GLB_FPS;              /* Frame per second. */
BOOL GLB_IsPause;       /* Pause or not */

static LONG
  StartTime,            /* Start time */
  OldTime,              /* Old time */
  PauseTime,            /* Pause time */
  OldFPSTime,           /* Old frame per second value */
  FrameCount;           /*  */

/* Timer initialisation function.
 * ARGUMENTS:
 *   None. 
 * RETURNS: None.
 */
VOID GLB_TimerInit( VOID )
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  GLB_IsPause = FALSE;
  GLB_FPS = 30;
} /* End of 'GLB_TimerInit' function */

/* Timer responsing function.
 * ARGUMENTS:
 *   None. 
 * RETURNS: None.
 */
VOID GLB_TimerResponse( VOID )
{
  LONG t = clock();

  if (!GLB_IsPause)
  {
    GLB_Time = (DOUBLE)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DOUBLE)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }

  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount / ((DOUBLE)(t - OldFPSTime) / CLOCKS_PER_SEC);
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
} /* End of 'GLB_TimerResponse' function */

/* END OF 'TIMER.C' FILE */