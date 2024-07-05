/* FILE NAME  : input.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 24.06.2024
 * PURPOSE    : 3D animation project.
 *          Input module.
 */

#include "anim.h"

INT DS4_MouseWheel;

/* Keyboard initializing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimKeyboardInit( VOID )
{
  GetKeyboardState(DS4_Anim.Keys);
  memcpy(DS4_Anim.KeysOld, DS4_Anim.Keys, 256);
  memset(DS4_Anim.KeysClick, 0, 256);
} /* End of 'DS4_AnimKeyboardInit' function */

/* Keyboard responsing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(DS4_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    DS4_Anim.Keys[i] >>= 7;
    DS4_Anim.KeysClick[i] = DS4_Anim.Keys[i] && !DS4_Anim.KeysOld[i];
  }
  memcpy(DS4_Anim.KeysOld, DS4_Anim.Keys, 256);
} /* End of 'DS4_AnimKeyboardInit' function */

/* Mouse initializing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(DS4_Anim.hWnd, &pt);
  DS4_Anim.Mx = pt.x;
  DS4_Anim.My = pt.y;
  DS4_Anim.Mdx = DS4_Anim.Mdy = DS4_Anim.Mz = DS4_Anim.Mdz = 0;
} /* End of 'DS4_AnimMouseInit' function */

/* Mouse responsing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimGetMouseResponse( VOID )
{
  POINT pt;
  
  GetCursorPos(&pt);
  ScreenToClient(DS4_Anim.hWnd, &pt);
  /* coordinades to screen */
  DS4_Anim.Mdx = pt.x = DS4_Anim.Mx;
  DS4_Anim.Mdy = pt.x = DS4_Anim.My;
  /* absolute values */
  DS4_Anim.Mx = pt.x;
  DS4_Anim.My = pt.y;

  /* Mouse wheel question */
  DS4_Anim.Mdz = DS4_MouseWheel;
  DS4_Anim.Mz += DS4_MouseWheel;
  DS4_MouseWheel = 0;
} /* End of 'DS4_AnimGetMouseResponse' function */

                               
/* END OF 'input.c' FILE */