/* FILE NAME:   t06petg.c
 * PROGRAMMER:  DS4
 * LAST UPDATE: 11.06.2024
 * PURPOSE:     Count determinan of mtx
 */

#include <stdio.h>
#include <windows.h>

#define FILE_NAME "petg.txt"
#define MAX 10

typedef DOUBLE DBL;

/* Forward references */

/* Permutation size */
INT N;

/* Array */
DBL A[MAX][MAX];

/* Load file function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) 1 if success, 0, otherwise.
 */
BOOL Load( VOID )
{
  INT i, j;
  FILE *F;

  N = 0;
  if ((F = fopen(FILE_NAME, "r")) == NULL)
    return FALSE;
  
  fscanf(F, "%d", &N);
  if (N < 0)
    N = 0;
  else if (N > MAX)
    N = MAX;

  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);

  fclose(F);
  return TRUE;
} /* End of 'Load' function */

/* Swap two array elements function.
 * ARGUMENTS: 
 *   - elements
 *       INT *A, *B;
 * RETURNS: None.
 */
VOID Swap( DBL *A, DBL *B )
{
  DBL tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* Add new array to file function.
 * ARGUMENTS: 
 *   - det:
 *     (DBL) det;
 * RETURNS:
 *   (INT) 1 if success, 0, otherwise.
 */
BOOL Add( DBL det )
{
  FILE *F;

  if ((F = fopen("OUT" FILE_NAME, "a")) == NULL)
    return FALSE;
  fprintf(F, "Determinator: %f\n", det);

  fclose(F);
  return TRUE;
} /* End of 'Add' function */

/* Count det of mtx.
 * ARGUMENTS: None.
 * RETURNS:
 *   (DBL) det;
 */
VOID Gauss( VOID )
{
  DBL det = 1, coef;
  INT max_raw, max_col;
  INT i, j, k, x, y;
  
  for (i = 0; i < N; i++)
  {
    /* look for maximum matrix element */
    max_raw = max_col = i;
    for (x = i; x < N; x++)
      for (y = i; y < N; y++)
        if (A[x][y] > A[max_raw][max_col])
        {
          max_raw = x;
          max_col = y;
        }
    if (A[max_raw][max_col] == 0)
    {
      det = 0;
      break;
    }
    if (max_raw != i)
    {
      /* Swap max_row row and i row (elemants [i..N-1]) */
      for (x = i; x < N; x++)
        Swap(&A[i][x], &A[max_raw][x]);
      det = -det;
    }
    if (max_col != i)
    {
      /* Swap max_row row and i row (elemants [i..N-1]) */
      for (y = i; y < N; y++)
        Swap(&A[y][i], &A[y][max_col]);
      det = -det;
    }
    /* Subtrack every row j:[i+1..N-1] i row multiplied by A[j][i] / A[i][i] */
    for (j = i + 1; j < N; j++)
    {
      coef = A[j][i] / A[i][i];
      A[j][i] = 0;
      for (k = i + 1; k < N; k++)
        A[j][k] -= A[i][k] * coef;
    }
    det *= A[i][i];
  }
  Add(det);
} /* End of 'Gauss' functoin */

/* Main program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID main( INT argc, CHAR *argv[] )
{
  SetConsoleTitle("CGSG forever!!!");
  if (!Load())
    return;
  Gauss();
} /* End of 'main' function */

/* END OF 't06petg.c' FILE */
