/* FILE NAME  : rnddata.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Data module.
 */

#include "def.h"

HWND DS4_hRndWnd;        /* Work window handle */
HDC DS4_hRndDC;          /* Work window memory device context  */
HGLRC DS4_hRndGLRC;      /* Work OpenGL rendering context */
INT DS4_RndFrameW, DS4_RndFrameH; /* Work window size */
FLT
  DS4_RndProjSize = 0.1,      /* Project plane fit square */
  DS4_RndProjDist = 0.1,      /* Distance to project plane from viewer (near) */
  DS4_RndProjFarClip = 300;   /* Distance to project far clip plane (far) */
 
VEC3 
  DS4_RndCamLoc,         /* Camera location */
  DS4_RndCamAt,          /* Camera destination */
  DS4_RndCamDir,         /* Camera direction */
  DS4_RndCamRight,       /* Camera right direction */
  DS4_RndCamUp;          /* Camera up direction */

MATR
  DS4_RndMatrView,  /* View coordinate system matrix */
  DS4_RndMatrProj,  /* Projection coordinate system matrix */
  DS4_RndMatrVP; /* Stored (View * Proj) matrix */


/* Frame overlay layer device context */
HDC DS4_hRndFrameDC;

/* END OF 'rnddata' FILE */