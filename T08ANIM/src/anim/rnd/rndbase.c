/* FILE NAME  : rndbase.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include "rnd.h"

/* Render initialization function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;  
 * RETURNS: None.
 */
VOID DS4_RndInit( HWND hWnd )
{
  HDC hDC;

  DS4_hRndWnd = hWnd;
  hDC = GetDC(hWnd);
  DS4_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  DS4_hRndBmFrame = NULL;
  DS4_RndCamSet(VecSet1(5), VecSet1(0), VecSet(0, 1, 0));
} /* End of 'DS4_RndInit' function */

/* Render deinitialization function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_RndClose( VOID )
{
  DeleteDC(DS4_hRndDCFrame);
  if (DS4_hRndBmFrame)
    DeleteObject(DS4_hRndBmFrame);
} /* End of 'DS4_RndClose' function */

/* Resize function.
 * ARGUMENTS:
 *   - height, wight:
 *       INT W, H;  
 * RETURNS: None.
 */
VOID DS4_RndResize( INT W, INT H )
{
  BITMAPINFOHEADER bmih;
 
  if (DS4_hRndBmFrame)
    DeleteObject(DS4_hRndBmFrame);
 
  /* Setup DIB section */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;
  bmih.biPlanes = 1;
  bmih.biCompression = BI_RGB;
  bmih.biWidth = W;
  bmih.biHeight = -H;
  bmih.biSizeImage = W * H * 4;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 0;
  DS4_hRndBmFrame =
    CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
      (VOID **)&DS4_RndFrameBits, NULL, 0);
 
  SelectObject(DS4_hRndDCFrame, DS4_hRndBmFrame);
 
  /* Save heigth and wight */
  DS4_RndFrameW = W;
  DS4_RndFrameH = H;
 
  /* Recount projection */
  DS4_RndProjSet(); 
} /* End of 'DS4_RndResize' function */

/* Frame copy function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;  
 * RETURNS: None.
 */
VOID DS4_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, DS4_RndFrameW, DS4_RndFrameH,
    DS4_hRndDCFrame, 0, 0, SRCCOPY);
} /* End of 'DS4_RndCopyFrame' function */

/* Render frame start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndStart( VOID )
{
  SelectObject(DS4_hRndDCFrame, GetStockObject(DC_BRUSH));
  SelectObject(DS4_hRndDCFrame, GetStockObject(NULL_PEN));
  SetDCBrushColor(DS4_hRndDCFrame, RGB(130, 200, 220));
  Rectangle(DS4_hRndDCFrame, 0, 0, DS4_RndFrameW, DS4_RndFrameH);
  SelectObject(DS4_hRndDCFrame, GetStockObject(NULL_BRUSH));
  SelectObject(DS4_hRndDCFrame, GetStockObject(WHITE_PEN));
} /* End of 'DS4_RndStart' function */

/* Render frame ending function.
 * ARGUMENTS: None.  
 * RETURNS: None.
 */
VOID DS4_RndEnd( VOID )
{
} /* End of 'DS4_RndEnd' function */

/* Set projection parameters function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = DS4_RndProjSize;
 
  /* Correct aspect ratio */
  if (DS4_RndFrameW >= DS4_RndFrameH)
    rx *= (DBL)DS4_RndFrameW / DS4_RndFrameH;
  else
    ry *= (DBL)DS4_RndFrameH / DS4_RndFrameW;
 
 
  DS4_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      DS4_RndProjDist, DS4_RndProjFarClip);
  DS4_RndMatrVP = MatrMulMatr(DS4_RndMatrView, DS4_RndMatrProj);
}  /* End of 'DS4_RndProjSet' function */

/* Camera setting function.
 * ARGUMENTS:
 *   - camera location:
 *       VEC Loc;
 *   - camera view to point:
 *       VEC At;
 *   - camera up direction (aproximatly):
 *       VEC Up;  
 * RETURNS: None.
 */
VOID DS4_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  DS4_RndMatrView = MatrView(Loc, At, Up);
  DS4_RndMatrVP = MatrMulMatr(DS4_RndMatrView, DS4_RndMatrProj);
} /* End of 'DS4_RndCamSet' function */
 
/* END OF 'rndbase.c' FILE */

