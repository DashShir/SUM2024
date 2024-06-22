/* FILE NAME  : rndprim.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include "rnd.h"

/* Camera setting function.
 * ARGUMENTS:
 *   - directions:
 *       VEC Loc, At, Up;  
 * RETURNS: None.
 */
BOOL DS4_RndPrimCreate( ds4PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;
 
  memset(Pr, 0, sizeof(ds4PRIM));
  size = sizeof(ds4VERTEX) * NoofV + sizeof(INT) * NoofI;
 
  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);
 
  return TRUE;
} /* End of 'DS4_RndPrimCreate' function */

/* Camera setting function.
 * ARGUMENTS:
 *   - directions:
 *       VEC Loc, At, Up;  
 * RETURNS: None.
 */
VOID DS4_RndPrimFree( ds4PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(ds4PRIM));
} /* End of 'DS4_RndPrimFree' function */

/* Drawing primitive function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - matrix:
 *       MATR World;  
 * RETURNS: None.
 */
VOID DS4_RndPrimDraw( ds4PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr(World, DS4_RndMatrVP);
  POINT *pnts;
 
  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;
 
  /* Build projection */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);
 
    pnts[i].x = (INT)((p.X + 1) * DS4_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * DS4_RndFrameH / 2);
  }
 
  /* Draw triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(DS4_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(DS4_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(DS4_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(DS4_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
} /* End of 'DS4_RndPrimDraw' function */

/* Creating quadlirateral function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC Org, Du, Dv;  
 * RETURNS: None.
 */
BOOL DS4_RndPrimCreateQuad( ds4PRIM *Pr, VEC Org, VEC Du, VEC Dv )
{
  if (!DS4_RndPrimCreate(Pr, 4, 6))
    return FALSE;
  Pr->V[0].P = Org;
  Pr->V[1].P = VecAddVec(Org, Du);
  Pr->V[2].P = VecAddVec(Pr->V[1].P, Dv);
  Pr->V[3].P = VecAddVec(Org, Dv);

  Pr->I[0] = 0;
  Pr->I[1] = 1;
  Pr->I[2] = 2;

  Pr->I[3] = 0;
  Pr->I[4] = 2;
  Pr->I[5] = 3;
  return TRUE;
} /* End of 'DS4_RndPrimCreateQuad' function */


/* Creating triangle function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC Org, Du, Dv;  
 * RETURNS: None.
 */
BOOL DS4_RndPrimCreateTrian( ds4PRIM *Pr, VEC Org, VEC Du, VEC Dv )
{
  if (!DS4_RndPrimCreate(Pr, 3, 3))
    return FALSE;
  Pr->V[0].P = Org;
  Pr->V[1].P = VecAddVec(Org, Du);
  Pr->V[2].P = VecAddVec(Org, Dv);

  Pr->I[0] = 0;
  Pr->I[1] = 1;
  Pr->I[2] = 2;

  return TRUE;
} /* End of 'DS4_RndPrimCreateTrian' function */

/* Creating sphere function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - center coordinates:
 *       VEC Coord;  
 *   - radius of sphere:
 *       DBL R;
 *   - heigh and width of sphere (parallels and meridians):
 *       DBL Par, Mer;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
BOOL DS4_RndPrimCreateSphere( ds4PRIM *Pr, VEC Coord, DBL R, DBL Par, DBL Mer )
{
  if (!DS4_RndPrimCreate(Pr, Pr->NumOfV, (Pr->NumOfV - 2) * 3))
    return FALSE;

  return TRUE;
}

/* Loading model from .OBJ file function.
 * ARGUMENTS: 
 *   - primitive:
 *       ds4PRIM *Pr;
 *   - model file name:
 *       CHAR *FileName;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
BOOL DS4_RndPrimLoad( ds4PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];
 
  memset(Pr, 0, sizeof(ds4PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
 
  /* Count vertices and indices */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;
      CHAR *ptr = Buf + 2, old = ' ';
 
      while (*ptr != 0)
        n += *ptr != ' ' && old == ' ', old = *ptr++;
      nf += n - 2;
    }
 
  if (!DS4_RndPrimCreate(Pr, nv, nf * 3))
  {
    fclose(F);
    return FALSE;
  }
 
  /* Read model data */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      Pr->V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, c = 0, c0 = 0, c1 = 0, c2 = 0;
      CHAR *ptr = Buf + 2, old = ' ';
 
      while (*ptr != 0)
      {
        if (*ptr != ' ' && old == ' ') /* found next word start */
        {
          sscanf(ptr, "%d", &c);
          if (c < 0)
            c = nv + c;
          else
            c--;
 
          if (n == 0)
            c0 = c;
          else if (n == 1)
            c1 = c;
          else
          {
            /* add new triangle */
            Pr->I[nf++] = c0;
            Pr->I[nf++] = c1;
            Pr->I[nf++] = c;
            c1 = c;
          }
          n++;
        }
        old = *ptr++;
      }
    }
 
  fclose(F);
  return TRUE;
} /* End of 'DS4_RndPrimLoad' file */

/* END OF 'rndprim.c' FILE */

