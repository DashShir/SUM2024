/* FILE NAME: globe.c
 * PROGRAMMER: DS4
 * LAST UPDATE: 14.06.2024
 * PURPOSE: Draw sphere.
 */

#include <math.h>

#include "globe.h"
#include "timer.h"

/* Geometry data */
static VEC Geom[GRID_H][GRID_W];  

/* Window size */
static INT Ws, Hs;

/* Projection: distance to plane, width and height */
static DBL ProjDist = 0.05, ProjSize = 0.1, Wp, Hp;

/* Power extension function.
 * ARGUMENTS:
 *   - value to be powered:
 *       DBL A;
 *   - power value:
 *       DBL B;
 * RETURNS:
 *   (DBL) powered result.
 */
static DBL Power( DBL A, DBL B )
{
  if (A < 0)
    return -pow(-A, B);
  return pow(A, B);
} /* End of 'RotateZ' function */


/* Globe initialisation function.
 * ARGUMENTS:
 *   - radius:
 *       DBL R;  
 * RETURNS: None.
 */
VOID GLB_Init( DBL R )
{
  INT i, j;
  DOUBLE theta, phi;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += PI / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * PI / (GRID_W - 1))
    {
      Geom[i][j].X = 0.5 * Power(sin(theta), 0.30) * Power(sin(phi), 0.30);
      Geom[i][j].Y = 2.8 * R * Power(cos(theta), 0.30);
      Geom[i][j].Z = 0.5 * Power(sin(theta), 0.30) * Power(cos(phi), 0.30);
    }
} /* End of 'GLB_Init' fuinction */

/* Globe resizing function.
 * ARGUMENTS:
 *   - window width and height:
 *       INT W, H; 
 * RETURNS: None.
 */
VOID GLB_Resize( INT W, INT H )
{
  Ws = W;
  Hs = H;
} /* End of 'GLB_Resize' function */

/* Rotate point around Y axis function.
 * ARGUMENTS:
 *   - point coordinates:
 *       VEC P;
 *   - angle in degrees:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector.
 */
static VEC RotateX( VEC P, DBL Angle )
{
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
  VEC NewP;
 
  NewP.X = P.X;
  NewP.Y = P.Z * si + P.Y * co;
  NewP.Z = P.Z * co - P.Y * si;
  return NewP;
} /* End of 'RotateX' function */

/* Rotate point around Y axis function.
 * ARGUMENTS:
 *   - point coordinates:
 *       VEC P;
 *   - angle in degrees:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector.
 */
static VEC RotateY( VEC P, DBL Angle )
{
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
  VEC NewP;
 
  NewP.X = P.X * co - P.Z * si;
  NewP.Y = P.Y;
  NewP.Z = P.X * si + P.Z * co;
  return NewP;
} /* End of 'RotateY' function */

/* Rotate point around Z axis function.
 * ARGUMENTS:
 *   - point coordinates:
 *       VEC P;
 *   - angle in degrees:
 *       DBL Angle;
 * RETURNS:
 *   (VEC) rotated vector.
 */
static VEC RotateZ( VEC P, DBL Angle )
{
  DBL a = Angle * PI / 180, si = sin(a), co = cos(a);
  VEC NewP;
 
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
} /* End of 'RotateZ' function */

/* Globe drawing
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC; 
 * RETURNS: None.
 */
VOID GLB_Draw( HDC hDC )
{
  INT i, j, s;
  DBL Sc;
  DBL t = GLB_Time;
  DBL alphaX = 21 * t, alphaY = 47 * t/* * sin(t)*/, alphaZ = 30 * t;
  HBRUSH hBr, hOldBr;
  HPEN hPen, hOldPen;
  POINT pnts1[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
  static POINT pnts[GRID_H][GRID_W];
  static POINT pnsr[GRID_H][GRID_W];

  /* Pen initialization */
  /*
  hBr = CreateSolidBrush(RGB(0, 0, 130));
  hOldBr = SelectObject(hDC, hBr);
  */
  hPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));
  hOldPen = SelectObject(hDC, hPen);

  /* Build projection */
  if (Ws >= Hs)
    Wp = ProjSize * Ws / Hs, Hp = ProjSize;
  else
    Wp = ProjSize, Hp = ProjSize * Hs / Ws;

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC P;
      DBL xp, yp;

      P = RotateZ(Geom[i][j], alphaZ);
      P = RotateY(P, alphaY);
      P = RotateX(P, alphaX);
      P.Z -= 5;

      xp = P.X * ProjDist / -P.Z;
      yp = P.Y * ProjDist / -P.Z;

      pnts[i][j].x = (INT)(Ws / 2 + xp * Ws / Wp);
      pnts[i][j].y = (INT)(Hs / 2 - yp * Hs / Hp);
    }

  s = 8;
  
  /* By points */
  /*
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x + s, pnts[i][j].y + s, pnts[i][j].x - s, pnts[i][j].y - s);
  */
  /* By lines */
  /* Parallels */
  /*
  for (i = 0; i < GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 1; j < GRID_W; j++)            
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  */
  /* Meridians */
  /*
  for (j = 0; j < GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
    for (i = 1; i < GRID_H; i++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  */
  /* Brush initializtion */
  /*
  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);
  */
  hBr = CreateSolidBrush(RGB(0, 40, 0));
  hOldBr = SelectObject(hDC, hBr);

  /* By polygons */
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      pnts1[0].x = pnts[i][j].x;
      pnts1[0].y = pnts[i][j].y;
      pnts1[1].x = pnts[i][j + 1].x;
      pnts1[1].y = pnts[i][j + 1].y;
      pnts1[2].x = pnts[i + 1][j + 1].x;
      pnts1[2].y = pnts[i + 1][j + 1].y;
      pnts1[3].x = pnts[i + 1][j].x;
      pnts1[3].y = pnts[i + 1][j].y;
      Sc = (pnts1[0].x - pnts1[1].x) * (pnts1[0].y + pnts1[1].y) +
           (pnts1[1].x - pnts1[2].x) * (pnts1[1].y + pnts1[2].y) + 
           (pnts1[2].x - pnts1[3].x) * (pnts1[2].y + pnts1[3].y) + 
           (pnts1[3].x - pnts1[0].x) * (pnts1[3].y + pnts1[0].y);
      if (Sc >= 0)    
        Polygon(hDC, pnts1, 4);
   }

  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);
  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
} /* End of 'GLB_Init' function */

/* END OF 'globe.c' FILE */
