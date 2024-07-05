/* FILE NAME  : anim.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 29.06.2024
 * PURPOSE    : 3D animation project.
 *          Animation set module.
 */

#include <stdio.h>

#include "anim.h"

/* Animation context */
ds4ANIM DS4_Anim;

/* Animation initialization function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;  
 * RETURNS: None.
 */
VOID DS4_AnimInit( HWND hWnd )
{
  DS4_Anim.hWnd = hWnd;
  DS4_RndInit(hWnd);
  DS4_Anim.hDC = DS4_hRndDC;

  DS4_TimerInit();
  DS4_AnimKeyboardInit();
  DS4_AnimMouseInit();
} /* End of 'DS4_AnimInit' file */

/* Animation resizing screen function.
 * ARGUMENTS:
 *   - screen height, widht:
 *       INT W, INT H;
 * RETURNS: None.
 */
VOID DS4_AnimResize( INT W, INT H )
{
  DS4_Anim.W = W;
  DS4_Anim.H = H;
  DS4_RndResize(W, H);
} /* End of 'DS4_AnimResize' function */

/* Animation copying function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID DS4_AnimCopyFrame( HDC hDC )  
{
  DS4_RndCopyFrame();
} /* End of 'DS4_AnimCopyFrame' function */

/* Animation closing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < DS4_Anim.NumOfUnits; i++)
  {
    DS4_Anim.Units[i]->Close(DS4_Anim.Units[i], &DS4_Anim);
    free(DS4_Anim.Units[i]);
    DS4_Anim.Units[i] = NULL;
  }
  DS4_Anim.NumOfUnits = 0;
  DS4_RndClose();
} /* End of 'DS4_AnimClose' function */

/* Animation rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimRender( VOID )
{
  INT i;
  static FLT OldTime = 0;

  /* if keyboard, mouse, joystick */
  DS4_TimerResponse();
  DS4_AnimKeyboardResponse();
  DS4_AnimGetMouseResponse();
  
  if (DS4_Anim.GlobalTime - OldTime > 3)
  {
    static CHAR Buf[100];

    TextOut(DS4_hRndFrameDC, 8, 8, Buf, sprintf(Buf, "DS4 Anim. FPS: %.3f\tCoordinates: %f", DS4_Anim.FPS));
    /* SetWindowText(DS4_Anim.hWnd, Buf); */
    OldTime = DS4_Anim.GlobalTime;
  }
  for (i = 0; i < DS4_Anim.NumOfUnits; i++)
    DS4_Anim.Units[i]->Response(DS4_Anim.Units[i], &DS4_Anim);
  DS4_RndStart();
  for (i = 0; i < DS4_Anim.NumOfUnits; i++)
    DS4_Anim.Units[i]->Render(DS4_Anim.Units[i], &DS4_Anim);
  DS4_RndEnd();
} /* End of 'DS4_AnimRender' function */

/* Add unit to nimation system function.
 * ARGUMENTS:
 *   - pointer to unit to be add:
 *       ds4UNIT *Uni;
 * RETURNS: None.
 */
VOID DS4_AnimUnitAdd( ds4UNIT *Uni )
{
  if (DS4_Anim.NumOfUnits < DS4_MAX_UNITS)
  {
    DS4_Anim.Units[DS4_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &DS4_Anim);
  }
} /* End of 'DS4_AnimUnitAdd' function */

/* Switching to full screen mode or from it considering monitors quantity.
 * ARGUMENTS: None.
 * RETIRNS: None.
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE; /* current mode */
  static RECT SaveRC;               /* saved size */
 
  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;
 
    /* saving old screen size */
    GetWindowRect(hWnd, &SaveRC);
 
    /* determining monitor where the window is */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
 
    /* get information for monitor */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
 
    /* go to full screen */
    /* for one monitor:
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    */
    rc = moninfo.rcMonitor;
 
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
 
    SetWindowPos(hWnd, HWND_TOP, /* or HWND_TOPMOST */
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Restorte window size */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */

/* END OF 'anim.c' FILE */