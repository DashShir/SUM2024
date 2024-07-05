/* FILE NAME  : rndbase.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 05.07.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include <time.h>

#include "rnd.h"

#include <wglew.h>

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")

/* Frame overlay layer local data */
#define DS4_RND_OVERLAY_W 500
#define DS4_RND_OVERLAY_H 100
static HBITMAP DS4_hRndBmFrame;
static DWORD *DS4_RndFrameBits;

/* Render initialization function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;  
 * RETURNS: None.
 */
VOID DS4_RndInit( HWND hWnd )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };
  BITMAPINFOHEADER bmih;
 
  DS4_hRndWnd = hWnd;
  DS4_hRndDC = GetDC(hWnd);
  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(DS4_hRndDC, &pfd);
  DescribePixelFormat(DS4_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(DS4_hRndDC, i, &pfd);
 
  /* OpenGL init: setup rendering context */
  DS4_hRndGLRC = wglCreateContext(DS4_hRndDC);
  wglMakeCurrent(DS4_hRndDC, DS4_hRndGLRC);
 
  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(DS4_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
    exit(0);
  }
 
  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(DS4_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }
 
  #ifndef NDEBUG
    OutputDebugString(glGetString(GL_VERSION));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_VENDOR));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_RENDERER));
    OutputDebugString("\n");
  #endif /* NDEBUG */

  #ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
      0, NULL, GL_TRUE);
  #endif /* NDEBUG */

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(DS4_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(DS4_hRndDC, NULL, ContextAttribs);
 
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DS4_hRndGLRC);
 
  DS4_hRndGLRC = hRC;
  wglMakeCurrent(DS4_hRndDC, DS4_hRndGLRC);

  wglSwapIntervalEXT(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  /*
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  */
  /*
  glPolygonMode(GL_FRONT, GL_POINT);
  glPolygonMode(GL_BACK, GL_LINE);
  */
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   */
  glClearColor(0.30, 0.47, 0.8, 1);

  /* Camera setup */
  DS4_RndFrameW = 30;
  DS4_RndFrameH = 30;
  DS4_RndProjSize = 0.1;
  DS4_RndProjDist = 0.1;
  DS4_RndProjFarClip = 1000;
  DS4_RndProjSet();  


  DS4_RndCamSet(Vec3Set1(5), Vec3Set1(0), Vec3Set(0, 1, 0));

  /* Frame overlay layer initialization */
  DS4_hRndFrameDC = CreateCompatibleDC(DS4_hRndDC);
  /* Setup DIB section */
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;
  bmih.biPlanes = 1;
  bmih.biCompression = BI_RGB;
  bmih.biWidth = DS4_RND_OVERLAY_W;
  bmih.biHeight = -DS4_RND_OVERLAY_H;
  bmih.biSizeImage = DS4_RND_OVERLAY_W * DS4_RND_OVERLAY_H * 4;
  bmih.biClrUsed = 0;
  bmih.biClrImportant = 0;
  bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 0;
  DS4_hRndBmFrame =
    CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS,
      (VOID **)&DS4_RndFrameBits, NULL, 0);
  SelectObject(DS4_hRndFrameDC, DS4_hRndBmFrame);
  SetTextColor(DS4_hRndFrameDC, RGB(255, 255, 255));
  SetBkColor(DS4_hRndFrameDC, RGB(0, 0, 0));

  DS4_RndResInit();
} /* End of 'DS4_RndInit' function */

/* Render deinitialization function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_RndClose( VOID )
{
  DS4_RndResClose();

  DeleteDC(DS4_hRndFrameDC);
  DeleteObject(DS4_hRndBmFrame);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(DS4_hRndGLRC);
  ReleaseDC(DS4_hRndWnd, DS4_hRndDC);
} /* End of 'DS4_RndClose' function */

/* Resize function.
 * ARGUMENTS:
 *   - height, wight:
 *       INT W, H;  
 * RETURNS: None.
 */
VOID DS4_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

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
VOID DS4_RndCopyFrame( VOID )
{
  SwapBuffers(DS4_hRndDC);
} /* End of 'DS4_RndCopyFrame' function */

/* Render frame start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndStart( VOID )
{
  INT t;
  static INT OldReloadTime = 0;

  if ((t = clock()) - OldReloadTime > CLOCKS_PER_SEC * 2)
  {
    DS4_RndShdUpdate();
    OldReloadTime = t;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} /* End of 'DS4_RndStart' function */

/* Render frame ending function.
 * ARGUMENTS: None.  
 * RETURNS: None.
 */
VOID DS4_RndEnd( VOID )
{
  /* glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */
  glLoadIdentity();
  glPixelZoom(1, -1);
  glRasterPos2d(-1, 1);
  glDrawPixels(DS4_RND_OVERLAY_W, DS4_RND_OVERLAY_H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, DS4_RndFrameBits);
  glBlendFunc(GL_ONE, GL_ZERO);
  glFinish();
} /* End of 'DS4_RndEnd' function */

/* Set projection parameters function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndProjSet( VOID )
{
  FLT rx, ry;
 
  rx = ry = DS4_RndProjSize;
 
  /* Correct aspect ratio */
  if (DS4_RndFrameW >= DS4_RndFrameH)
    rx *= (FLT)DS4_RndFrameW / DS4_RndFrameH;
  else
    ry *= (FLT)DS4_RndFrameH / DS4_RndFrameW;
 
 
  DS4_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      DS4_RndProjDist, DS4_RndProjFarClip);
  DS4_RndMatrVP = MatrMulMatr(DS4_RndMatrView, DS4_RndMatrProj);
}  /* End of 'DS4_RndProjSet' function */

/* Camera setting function.
 * ARGUMENTS:
 *   - camera location:
 *       VEC3 Loc;
 *   - camera view to point:
 *       VEC3 At;
 *   - camera up direction (aproximatly):
 *       VEC3 Up;  
 * RETURNS: None.
 */
VOID DS4_RndCamSet( VEC3 Loc, VEC3 At, VEC3 Up )
{
  DS4_RndMatrView = MatrView(Loc, At, Up);
  DS4_RndMatrVP = MatrMulMatr(DS4_RndMatrView, DS4_RndMatrProj);
  DS4_RndCamRight = Vec3Set(DS4_RndMatrView.A[0][0],
                         DS4_RndMatrView.A[1][0],
                         DS4_RndMatrView.A[2][0]);
  DS4_RndCamUp = Vec3Set(DS4_RndMatrView.A[0][1],
                      DS4_RndMatrView.A[1][1],
                      DS4_RndMatrView.A[2][1]);
  DS4_RndCamDir = Vec3Set(-DS4_RndMatrView.A[0][2],
                       -DS4_RndMatrView.A[1][2],
                       -DS4_RndMatrView.A[2][2]);
 
  DS4_RndCamLoc = Loc;
  DS4_RndCamAt = At;

} /* End of 'DS4_RndCamSet' function */
 
/* END OF 'rndbase.c' FILE */

