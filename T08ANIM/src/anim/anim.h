/* FILE NAME  : anim.h
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *          Anim declaration module.
 */

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define DS4_MAX_UNITS 3000

#define DS4_UNIT_BASE_FIELDS \
  VOID (*Init)( ds4UNIT *Uni, ds4ANIM *Ani );       \
  VOID (*Close)( ds4UNIT *Uni, ds4ANIM *Ani );      \
  VOID (*Response)( ds4UNIT * Uni, ds4ANIM *Ani );  \
  VOID (*Render)( ds4UNIT *Uni, ds4ANIM *Ani )

typedef struct tagds4ANIM ds4ANIM;
typedef struct tagds4UNIT ds4UNIT;
struct tagds4UNIT
{
  DS4_UNIT_BASE_FIELDS;  /* Unit basic fields */
};

struct tagds4ANIM
{
  HWND hWnd;                     /* Window handle */
  HDC hDC;                       /* Device context */

  INT W, H;                      /* Screen width and height */

  ds4UNIT *Units[DS4_MAX_UNITS]; /* Unit action functions */
  INT NumOfUnits;                /* Units quantity */
};

/* Animation context */
extern ds4ANIM DS4_Anim;

/* Animation initialization function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;  
 * RETURNS: None.
 */
VOID DS4_AnimInit( HWND hWnd );

/* Animation initialization function.
 * ARGUMENTS:
 *   - screen height, widht:
 *       INT W, INT H;
 * RETURNS: None.
 */
VOID DS4_AnimResize( INT W, INT H );

/* Animation initialization function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 * RETURNS: None.
 */
VOID DS4_AnimCopyFrame( HDC hDC );

 /* Animation closing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimClose( VOID );

/* Animation rendering function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimRender( VOID );

/* Add unit to nimation system function.
 * ARGUMENTS:
 *   - pointer to unit to be add:
 *       ds4UNIT *Uni;
 * RETURNS: None.
 */
VOID DS4_AnimUnitAdd( ds4UNIT *Uni );

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_AnimUnitCreate( INT Size );

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
