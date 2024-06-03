/* Shiryaeva Daria, 08-4, 24.04.2024, DS4 */

#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "My window class"

/* MyWindoWFunc description */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, 
                               WPARAM wParam, LPARAM lParam );
/* Drawing eye function description */
VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd = NULL;
  MSG msg;
  HDC hDC;

  hDC = GetDC(hWnd);
  SetPixel(hDC, 10, 10, RGB(0, 255, 200));
  ReleaseDC(hWnd, hDC);

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
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT i, j;
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 30, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostQuitMessage(0);
    return 0;
  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hDC, 400, 300, 90, 30, pt.x, pt.y);

    srand(30);
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
      for (i = 0; i < 47; i++)
        DrawEye(hDC, rand() % 1000, rand() % 1000, 47, 18, pt.x, pt.y);
    else
      for (i = 0; i < h; i += 100)
        for (j = 0; j < w; j += 100)
          DrawEye(hDC, j, i, 47, 18, pt.x, pt.y);

    EndPaint(hWnd, &ps);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  HBRUSH hBr, hOldBr;
  HPEN hPen, hOldPen;
  double k, Nx, Ny, dx, dy;

  hBr = CreateSolidBrush(RGB(255, 255, 255));
  hOldBr = SelectObject(hDC, hBr);
  hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 20));
  hOldPen = SelectObject(hDC, hPen);
  Ellipse(hDC, X - R, Y - R, X + R + 2, Y + R + 2);
  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);
  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);

  hBr = CreateSolidBrush(RGB(0, 0, 0));
  hOldBr = SelectObject(hDC, hBr);
  hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
  hOldPen = SelectObject(hDC, hPen);
  dx = Mx - X;
  dy = My - Y;
  k = (R - R1) / sqrt(dx * dx + dy * dy);
  if (k > 1)
  {
    Nx = Mx;
    Ny = My;
  }
  else
  {
    Nx = k * dx + X;
    Ny = k * dy + Y;
  }
  Ellipse(hDC, Nx - R1, Ny - R1, Nx + R1 + 1, Ny + R1 + 1);
  SelectObject(hDC, hOldBr);
  DeleteObject(hBr);
  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}