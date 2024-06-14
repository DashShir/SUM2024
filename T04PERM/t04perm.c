/* FILE NAME:   t03clock.c
 * PROGRAMMER:  DS4
 * LAST UPDATE: 10.06.2024
 * PURPOSE:     Input clock and date.
 */

#include <stdio.h>
#include <windows.h>

#define FILE_NAME "det.txt"

/* Forward references */

/* Permutation size */
INT N;

/* Permutation array */
INT *P;

/* Load file function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) 1 if success, 0, otherwise.
 */
INT Load( VOID )
{
  INT i, x;
  FILE *F;

  N = 0;
  if ((F = fopen(FILE_NAME, "r")) == NULL)
    return 0;
  while (fscanf(F, "%d", &x) == 1)
    ++N;
  rewind(F);
  P = malloc(sizeof(INT) * N);

  for (i = 0; i < N; i++)
    fscanf(F, "%d", &P[i]);
  
  fclose(F);
  return 1;
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
 *   - number:
 *       INT odd;
 * RETURNS:
 *   (INT) 1 if success, 0, otherwise.
 */
INT Add( INT odd )
{
  INT i;
  FILE *F;

  if ((F = fopen("OUT" FILE_NAME, "a")) == NULL)
    return 0;
  for (i = 0; i < N; i++)
    fprintf(F, "%i, ", P[i]);
  fprintf(F, " - %i\n", odd);

  fclose(F);
  return 1;
} /* End of 'Add' function */

/* Add new array to file function.
 * ARGUMENTS: 
 *   - arrary ptr:
 *       (INT *) A;
 *   - size of array:
 *       (INT) N;
 * RETURNS: None.
 */
VOID OutMas( INT *A, INT N, INT odd )
{
  INT i;

  printf("\n");
  for (i = 0; i < N; i++)
    printf("%i ", A[i]);

  printf(" - %i\n", odd);
} /* End of 'OutMas' function */

/* Go for elements of array function.
 * ARGUMENTS: 
 *   - current ptr position:
 *       int Pos;
 * RETURNS: None.
 */
VOID Go( INT Pos, INT odd )
{
  INT i, x, old = 0;

  if (Pos == N)
  {
    Add(odd);
    OutMas(P, N, odd);
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
  SetConsoleTitle("CGSG forever!!!");
  if (!Load())
    return EXIT_FAILURE;
  Go(0, 0);

} /* End of 'main' function */

/* END OF 't04perm.c' FILE */


