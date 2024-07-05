/* FILE NAME: timer.h
 * PROGRAMMER: DS4
 * LAST UPDATE: 14.06.2024
 * PURPOSE: Draw sphere.
 */

#include <time.h>

#include "anim.h"

static LONG
  StartTime,            /* Start time */
  OldTime,              /* Old time */
  PauseTime,            /* Pause time */
  OldFPSTime,           /* Old frame per second value */
  FrameCount;           /* Frame count for FPS evaluation */

/* Timer initialisation function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_TimerInit( VOID )
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  DS4_Anim.IsPause = FALSE;
  DS4_Anim.FPS = 30;
} /* End of 'DS4_TimerInit' function */

/* Timer responsing function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_TimerResponse( VOID )
{
  LONG t = clock();

  DS4_Anim.GlobalTime = (DBL)(t - StartTime) / CLOCKS_PER_SEC;
  if (!DS4_Anim.IsPause)
  {
    DS4_Anim.Time = (DBL)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    DS4_Anim.DeltaTime = (DBL)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    DS4_Anim.DeltaTime = 0;
  }

  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    DS4_Anim.FPS = FrameCount / ((DBL)(t - OldFPSTime) / CLOCKS_PER_SEC);
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
} /* End of 'DS4_TimerResponse' function */

/* END OF 'TIMER.C' FILE */