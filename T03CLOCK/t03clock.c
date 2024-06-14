/* Shiryaeva Daria, 08-4, 24.04.2024, DS4 */

/* FILE NAME: t03clock.c
 * PROGRAMMER: DS4
 * LAST UPDATE: 07.06.2024
 * PURPOSE: Input clock and date.
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include <windows.h>

#pragma warning(disable: 4244)

#define WND_CLASS_NAME "My window class"
#define PI 3.14159265358979323846264338327950288419716939937510582

/* Forward references */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam );
VOID DrawClock( HDC hDC, INT Xc, INT Yc, INT Rc, INT Hgh, INT Wgh );

/* Main Program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

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

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
} /* End of 'WinMain' function */

/* Main window message handle function */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  HBRUSH hBr, hOldBr;
  HPEN hPen, hOldPen;
  static HFONT hFnt;
  static BITMAP bmcf, bmlic;
  static HDC hMemDC, hAndDC, hLicDC;
  static INT w, h, a, dh, dw, lic_wng = 0;
  static HBITMAP hBm, hBmCF, hBmLic;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hCFDC = CreateCompatibleDC(hDC);
    hLicDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBm = NULL;

    hBmCF = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE /* | LR_CREATEDIBSECTION */);
    GetObject(hBmCF, sizeof(BITMAP), &bmcf);
    hBmLic = LoadImage(NULL, "licence.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hFnt = NULL;

    SelectObject(hCFDC, hBmCF);
    SelectObject(hLicDC, hBmLic);

    SetTimer(hWnd, 30, 30, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);                                                
    h = HIWORD(lParam);

    if (hFnt != NULL)
      DeleteObject(hFnt);

    hFnt = CreateFont(30 * h / 500, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                      RUSSIAN_CHARSET,
                      OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                      VARIABLE_PITCH | FF_SWISS, "Consolas");
    SelectObject(hMemDC, hFnt);
    
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    DeleteObject(hFnt);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_TIMER:
    hBr = CreateSolidBrush(RGB(0, 90, 30));
    hOldBr = SelectObject(hMemDC, hBr);
    hPen = CreatePen(PS_SOLID, 5, RGB(0, 90, 30));
    hOldPen = SelectObject(hMemDC, hPen);
    Rectangle(hMemDC, 0, 0, w, h);
    SelectObject(hMemDC, hOldBr);
    DeleteObject(hBr);
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);

    a = w >= h ? h : w;
    GetObject(hBmCF, sizeof(BITMAP), &bmcf);
    GetObject(hBmLic, sizeof(BITMAP), &bmlic);

    SetStretchBltMode(hMemDC, COLORONCOLOR);
    StretchBlt(hMemDC, (w - a) / 2, (h - a) / 2, a, a, hCFDC, 0, 0, bmcf.bmWidth, bmcf.bmHeight, SRCCOPY);
    
    InvalidateRect(hWnd, NULL, FALSE);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    
    hDC = GetDC(hWnd);
    
    /*
    BitBlt(hMemDC, pt.x, pt.y, w, h, hAndDC, 0, 0, SRCAND); 
    BitBlt(hMemDC, pt.x, pt.y, w, h, hXORDC, 0, 0, SRCINVERT);
    */

    if (lic_wng != 0)
    {
      BitBlt(hMemDC, (w - bmlic.bmWidth) / 2, (h - bmlic.bmHeight) / 2, w, h, hLicDC, 0, 0, SRCCOPY);
    }
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_KEYDOWN:
    if (LOWORD(wParam) == VK_ESCAPE)
    {
      lic_wng++;
      SendMessage(hWnd, WM_TIMER, 0, 0);
    }
    return 0;
  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteObject(hBmCF);
    DeleteObject(hFnt);
    DeleteDC(hMemDC);
    DeleteDC(hCFDC);
    DeleteDC(hLicDC);
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* End of 't03clock.c' file */