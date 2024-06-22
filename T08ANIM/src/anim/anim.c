/* FILE NAME  : anim.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Animation set module.
 */

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
  DS4_Anim.hDC = DS4_hRndDCFrame;
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
  DS4_RndCopyFrame(hDC);
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

  /* if timer, keyboard, mouse, joystick */
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

/* END OF 'anim.c' FILE */