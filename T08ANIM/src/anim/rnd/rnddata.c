/* FILE NAME  : rnddata.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Data module.
 */

#include "def.h"

HWND DS4_hRndWnd;        /* Work window handle */
HDC DS4_hRndDCFrame;     /* Work window memory device context  */
HBITMAP DS4_hRndBmFrame; /* Work window background bitmap handle */
INT DS4_RndFrameW, DS4_RndFrameH; /* Work window size */
DWORD *DS4_RndFrameBits; /* Frame buffer pixel data (from DIB section) */
DBL
  DS4_RndProjSize = 0.1,      /* Project plane fit square */
  DS4_RndProjDist = 0.1,      /* Distance to project plane from viewer (near) */
  DS4_RndProjFarClip = 300;   /* Distance to project far clip plane (far) */
 
MATR
  DS4_RndMatrView,  /* View coordinate system matrix */
  DS4_RndMatrProj,  /* Projection coordinate system matrix */
  DS4_RndMatrVP; /* Stored (View * Proj) matrix */
 


/* END OF 'rnddat' FILE */