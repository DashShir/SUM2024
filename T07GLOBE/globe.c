/* FILE NAME:   globe.c
 * PROGRAMMER:  DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE:     Draw sphere.
 */

#include "mth.h"
#include "globe.h"
#include "timer.h"

/* Geometry data */
static VEC Geom[GRID_H][GRID_W];  

/* Window size */
static INT Ws, Hs;

/* Projection: distance to plane, width and height */
static DBL ProjDist = 0.13, ProjSize = 0.07, ProjFarClip = 30, Wp, Hp;
static MATR MatrProj;

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

  /* Build projection */
  if (Ws >= Hs)
    Wp = ProjSize * Ws / Hs, Hp = ProjSize;
  else
    Wp = ProjSize, Hp = ProjSize * Hs / Ws;
  MatrProj = MatrFrustum(-Wp / 2, Wp / 2, -Hp / 2, Hp / 2, ProjDist, ProjFarClip);
} /* End of 'GLB_Resize' function */

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
  HBRUSH hBr, hOldBr;
  HPEN hPen, hOldPen;
  POINT pnts1[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
  MATR m;
  static POINT pnts[GRID_H][GRID_W];
  
  /* Pen initialization */
  /*
  hBr = CreateSolidBrush(RGB(0, 0, 130));
  hOldBr = SelectObject(hDC, hBr);
  */
  hPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));
  hOldPen = SelectObject(hDC, hPen);

  /* Build transform */
  m = MatrMulMatr8(MatrRotateZ(t * 30),
                   MatrRotateX(sin(t) * 13 * 21),
                   MatrTranslate(VecSet(sin(t), sin(t), 0)),
                   MatrRotateY(t * 47),
                   MatrRotate(90, VecSet(1, 100, 1)),
                   MatrTranslate(VecSet(-30, -21 * sin(t) - 5, -30 * sin(t) - 47)),
                   MatrView(VecSet(5, 5 * sin(t), 5), VecSet1(0), VecSet(0, 1, 0)),
                   MatrProj);
                   

  /* Build projection */
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC P;

      P = VecMulMatr(Geom[i][j], m);
      pnts[i][j].x = (INT)((P.X + 1) * Ws / 2);
      pnts[i][j].y = (INT)((-P.Y + 1) * Hs / 2);
    }

  s = 8;
  
  /* By points */
  /*
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x + s, pnts[i][j].y + s, pnts[i][j].x - s, pnts[i][j].y - s);
  
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
