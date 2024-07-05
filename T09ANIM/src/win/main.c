/* FILE NAME  : main.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Main startup module.
 */

#include <time.h>

#include "units/units.h"
#include "anim/anim.h"

#define WND_CLASS_NAME "My window class"

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam );

/* Main Program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT x = 0;
  
  SetDbgMemHooks();

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error rigister window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = 
    CreateWindow(WND_CLASS_NAME,
      "Title",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /*** Add units ***/
  DS4_AnimUnitAdd(DS4_UnitCreateCtrl());
  //DS4_AnimUnitAdd(DS4_UnitCreateIsland(Vec3Set(100, 0, 100)));
  //for (i = 0; i < 1 * 30; i++)
  /*
  DS4_AnimUnitAdd(DS4_UnitCreateCow(Vec3Set(Rnd1() * 5, 0, Rnd1() * 5)));      
  DS4_AnimUnitAdd(DS4_UnitCreateFloor(Vec3Set(1, 0, 1)));
  */
  //DS4_AnimUnitAdd(DS4_UnitCreateGrass(Vec3Set(0, 0, 0)));
  /* DS4_AnimUnitAdd(DS4_UnitCreateGrass(Vec3Set(100, 0, 100))); */
  DS4_AnimUnitAdd(DS4_UnitCreateMountain());
  DS4_AnimUnitAdd(DS4_UnitCreateTexture());

  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);

  return msg.wParam;
} /* End of 'WinMain' function */

/* Main window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  /*
  case WM_GETMINMAXINFO:
    FlipFullScreen(hWnd);
  */
  case WM_CREATE:
    SetTimer(hWnd, 30, 5, NULL);
    DS4_AnimInit(hWnd);
    /* FlipFullScreen(hWnd); */
    return 0;
  case WM_SIZE:
    DS4_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 30, 0);
    return 0;
  case WM_MOUSEWHEEL:
    DS4_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;
  case WM_TIMER:
    /* Draw frame */
    DS4_AnimRender();
    /* Copy frame */
    hDC = GetDC(hWnd);
    DS4_AnimCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DS4_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DS4_AnimClose();
    PostQuitMessage(0);
    return 0;
  }            
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 't08anim.c' FILE */