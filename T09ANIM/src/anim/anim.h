/* FILE NAME  : anim.h
 * PROGRAMMER : DS4
 * LAST UPDATE: 08.07.2024
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

  FLT
    GlobalTime, /* Global time (without pause) */
    Time,       /* Full time */
    DeltaTime,  /* One ... time */
    FPS;        /* Frame per second. */

  BYTE Keys[256];       /* Keys statement on current shot */
  BYTE KeysOld[256];    /* Keys statement on last shot */
  BYTE KeysClick[256];  /* Signs of one-time key pressing */
   
  INT Mx, My, Mz, Mdx, Mdy, Mdz;

  BOOL IsPause; /* Pause or not */
};

/* Animation context */
extern ds4ANIM DS4_Anim;

extern VEC3 DS4_AnimLoc;

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
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimCopyFrame( VOID );

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

/*** 
 * Timer support
 ***/

/* Timer initialisation function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_TimerInit( VOID );

/* Timer responsing function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_TimerResponse( VOID );

/***
 * Keyboard support.
 ***/

/* Keyboard initializing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimKeyboardInit( VOID );

/* Keyboard responsing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimKeyboardResponse( VOID );

/***
 * Mouse support.
 ***/

/* Mouse initializing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimMouseInit( VOID );

/* Mouse responsing function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_AnimGetMouseResponse( VOID );


/* Switching to full screen mode or from it considering monitors quantity.
 * ARGUMENTS: None.
 * RETIRNS: None.
 */
VOID FlipFullScreen( HWND hWnd );

extern INT DS4_MouseWheel;

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */
