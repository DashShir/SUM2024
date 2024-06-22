/* FILE NAME  : main.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 20.06.2024
 * PURPOSE    : 3D animation project.
 *          Main startup module.
 */

#include <time.h>

#include "units/units.h"

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
  INT i, j, k;
  DBL sdX = 0, sdY = 0, sdZ = 0;
  static ds4PRIM Pr, Pr1, Pr2, Pr3;

  switch (Msg)
  {
  /*
  case WM_MINMAXINFO:
    ;
  */
  case WM_CREATE:
    SetTimer(hWnd, 30, 5, NULL);
    DS4_RndInit(hWnd);

    DS4_RndPrimLoad(&Pr3, "COW1.OBJ");

    DS4_RndPrimCreateQuad(&Pr, VecSet(0, 0, 0), VecSet(1, 0, 0), VecSet(0, 0, -5));
    DS4_RndPrimCreateQuad(&Pr1, VecSet(0, 0, 0), VecSet(1, 0, 0), VecSet(0, 1, 0));
    //DS4_RndPrimCreateSphere(&Pr2, VecSet(0, 0, 0), 10, 6, 12);  

   
    return 0;
  case WM_SIZE:
    DS4_RndResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 30, 0);
    return 0;
  case WM_TIMER:
    /* Draw frame */
    DS4_RndStart();
    DS4_RndPrimDraw(&Pr, MatrRotateY(21 * sin(47 * clock() / 20000.0)));
    DS4_RndPrimDraw(&Pr1, MatrRotateY(21 * sin(47 * clock() / 20000.0)));
    
    for (j = 0, sdY = -5; j < 2; j++, sdY += 2.75)
      for (i = 0, sdX = -5; i < 2; i++, sdX += 4) 
        for (k = 0, sdZ = -5; k < 2; k++, sdZ += 3.75)
          DS4_RndPrimDraw(&Pr3, MatrMulMatr(MatrTranslate(VecSet(sdX, sdY, sdZ)), MatrRotateY(21 * sin(47 * clock() / 20000.0))));
    DS4_RndPrimDraw(&Pr1, MatrRotateX(21 * sin(47 * clock() / 20000.0)));

    DS4_RndEnd();

    /* Copy frame */
    hDC = GetDC(hWnd);
    DS4_RndCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    DS4_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DS4_RndPrimFree(&Pr);
    DS4_RndPrimFree(&Pr1);
    //DS4_RndPrimFree(&Pr2);
    DS4_RndPrimFree(&Pr3);
   
    DS4_RndClose();
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 't08anim.c' FILE */