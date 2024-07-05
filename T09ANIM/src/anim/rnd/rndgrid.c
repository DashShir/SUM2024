/* FILE NAME  : rndgrid.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 02.07.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include "rnd.h"

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreate( ds4GRID *G, INT W, INT H )
{
  INT i, j, k;

  memset(G, 0, sizeof(ds4GRID));
  if ((G->V = malloc(sizeof(ds4VERTEX) * H * W)) == NULL)
    return FALSE;
  memset(G->V, 0, sizeof(ds4VERTEX) * H * W);
  G->H = H;
  G->W = W;

  /* Default fill */
  for (i = 0, k = 0; i < H; i++)
    for (j = 0; j < W; j++, k++)
    {
      G->V[k].P = Vec3Set(j, 1, H - 1 - i);
      G->V[k].N = Vec3Set(0, 1, 0);
      G->V[k].T = Vec2Set(j / (W - 1.0), i / (H - 1.0));
      G->V[k].C = Vec4Set(1, 1, 1, 1);
    }


  return TRUE;
} /* End of 'DS4_RndGridCreate' function */

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndGridFree( ds4GRID *G )
{
  if (G->V != NULL)
    free(G->V);
} /*End of 'DS4_RndGridFree' function */

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndGridAutoNormals( ds4GRID *G )
{
  INT i, j;
         
  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = Vec3Set(0, 0, 0);

  for (i = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++)
    {
      ds4VERTEX
        *P00 = G->V + i * G->W + j,
        *P01 = G->V + i * G->W + j + 1,
        *P10 = G->V + (i + 1) * G->W + j,
        *P11 = G->V + (i + 1) * G->W + j + 1;
      VEC3 N;
 
      N = VecNormalize(VecCrossVec(VecSubVec(P00->P, P10->P),
                                   VecSubVec(P11->P, P10->P)));
      P00->N = VecAddVec(P00->N, N);
      P10->N = VecAddVec(P10->N, N);
      P11->N = VecAddVec(P11->N, N);
 
      N = VecNormalize(VecCrossVec(VecSubVec(P11->P, P01->P),
                                   VecSubVec(P00->P, P01->P)));
      P00->N = VecAddVec(P00->N, N);
      P01->N = VecAddVec(P01->N, N);
      P11->N = VecAddVec(P11->N, N);
    }
  for (i = 0; i < G->W * G->H; i++)
    G->V[i].N = VecNormalize(G->V[i].N);
} /* End of 'DS4_RndGridAutoNormals' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndPrimFromGrid( ds4PRIM *Pr, ds4GRID *G )
{
  INT *Ind;
  INT i, j, k;

  memset(Pr, 0, sizeof(ds4PRIM));
 
  if ((Ind = malloc(sizeof(INT) * ((G->H - 1) * (G->W * 2 + 1) - 1))) == NULL)
    return;
  for (i = 0, k = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = (i + 1) * G->W + j;
      Ind[k++] = i * G->W + j;
    }
    if (i != G->H - 2)
      Ind[k++] = -1;
  }
  DS4_RndPrimCreate(Pr, DS4_RND_PRIM_TRISTRIP, G->V, G->W * G->H,
    Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(Ind);
} /* End of 'DS4_RndPrimFromGrid' function */

/* Sphere grid init function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreateSphere( ds4GRID *G, FLT R, INT W, INT H )
{
  INT i, j, k;
  FLT phi, theta;

  if (!DS4_RndGridCreate(G, W, H))
    return FALSE;
  for (i = 0, k = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1), k++)
    {
      G->V[k].N.X = sin(theta) * sin(phi);
      G->V[k].N.Y = cos(theta);
      G->V[k].N.Z = sin(theta) * cos(phi);
      G->V[k].P = Vec3MulNum(G->V[k].N, R);
    }
  return TRUE;
} /* End of 'DS4_RndGridCreateSphere' function */

/* Torus grid init function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - Torus radiuses - inside and outside:
 *       FLT RIn, FLT ROut;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreateTorus( ds4GRID *G, FLT RIn, FLT ROut, INT W, INT H )
{
  INT i, j, k;
  FLT phi, theta;

  if (!DS4_RndGridCreate(G, W, H))
    return FALSE;
  for (i = 0, k = 0, theta = 0; i < H; i++, theta += PI / (H - 1))
    for (j = 0, phi = 0; j < W; j++, phi += 2 * PI / (W - 1), k++)
    {
      G->V[k].N.X = RIn * sin(theta) + (ROut - RIn) * cos(phi);
      G->V[k].N.Y = (ROut - RIn) * sin(phi);
      G->V[k].N.Z = RIn * cos(theta);
      G->V[k].P = G->V[k].N;
    }
  return TRUE;
} /* End of 'DS4_RndGridCreateTorus' function */

/* END OF 'rndgrid.h' FILE */
