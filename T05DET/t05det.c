/* FILE NAME:   t04perm.c
 * PROGRAMMER:  DS4
 * LAST UPDATE: 11.06.2024
 * PURPOSE:     Count determinan of mtx
 */

#include <stdio.h>
#include <windows.h>

#define FILE_NAME "det.txt"
#define MAX 10

typedef DOUBLE DBL;

/* Forward references */

/* Permutation size */
INT N;

/* Permutation array */
INT P[MAX];

/* Array */
DBL A[MAX][MAX];

/* Result determinant */
DBL Determinant = 0;

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
VOID Swap( INT *A, INT *B )
{
  INT tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function */

/* Add new array to file function.
 * ARGUMENTS: 
 *   - Determinant:
 *     (DBL) det;
 * RETURNS:
 *   (INT) 1 if success, 0, otherwise.
 */
BOOL Add( DBL det )
{
  INT i;  
  FILE *F;

  if ((F = fopen("OUT" FILE_NAME, "a")) == NULL)
    return FALSE;
  for (i = 0; i < N; i++)
    fprintf(F, "%i, ", P[i]);
  fprintf(F, " - %f\n", det);

  fclose(F);
  return TRUE;
} /* End of 'Add' function */

/* Go for elements of array function.
 * ARGUMENTS: 
 *   - current ptr position:
 *       INT Pos;
 *   - even, odd:
 *       INT odd;
 * RETURNS: None.
 */
VOID Go( INT Pos, INT odd )
{
  INT i, x, old = 0;
  DBL prod = 1;

  if (Pos == N)
  {
    for (i = 0; i < N; i++)
      prod *= A[i][P[i]];
    Determinant += (1 - odd % 2 * 2) * prod;
    
    Add(Determinant);
    odd = old;
    
    return;
  }

  Go(Pos + 1, odd);
  old = odd;
  for (i = Pos + 1; i < N; i++)
  {
    Swap(&P[Pos], &P[i]);
    old++;
    Go(Pos + 1, old);
  }
  x = P[Pos];
  for (i = Pos + 1; i < N; i++)
    P[i - 1] = P[i];
  P[N - 1] = x;
} /* End of 'Go' function */

/* Main program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID main( INT argc, CHAR *argv[] )
{
  INT i;
  
  SetConsoleTitle("CGSG forever!!!");
  if (!Load())
    return;
  for (i = 0; i < MAX; i++)
    P[i] = i;
  Determinant = 0;
  Go(0, 0);
  printf("Det: %f\n", Determinant);
} /* End of 'main' function */

/* END OF 't05det.c' FILE */
