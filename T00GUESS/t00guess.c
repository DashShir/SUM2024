/* Shiryaeva Daria, 08-4, 24.04.2024, DS4 */

#include <windows.h>
#include <stdio.h>

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  int ans = 0, a = 0, b = 100, m = 50;
  char q[200];

  sprintf(q, "Is it more than %i?", m);
  while ((ans = MessageBox(NULL, q, "Guesser", MB_ICONQUESTION | MB_YESNOCANCEL)) != IDCANCEL && m >= a && m <= b)
  {
    if (ans == IDYES)
      a = m + 1;
    else
      b = m - 1;
    m = (a + b) / 2;
    if (m <= b && m >= a)
      sprintf(q, "Is it more than %i?", m);
  }
  if (m >= a && m <= b)
  {
    sprintf(q, "That is %i", m);
    MessageBox(NULL, q, "Result", MB_OK);
  }
  else
    MessageBox(NULL, "Something went wrong", "Result", MB_OK);
  return 30;
}