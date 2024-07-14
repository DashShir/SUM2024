/* FILE NAME  : rndprim.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 06.07.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include <stdio.h>

#include "anim/anim.h"

/* Create empty primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       ds4PRIM *Pr;
 *   - primitive type:
 *       ds4PRIM_TYPE Type;
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 */
VOID DS4_RndPrimCreate( ds4PRIM *Pr, ds4PRIM_TYPE Type,
                        ds4VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  memset(Pr, 0, sizeof(ds4PRIM));
  
  /* Vertex array */
  glGenVertexArrays(1, &Pr->VA);
  /* Vertex data */
  if (V != NULL && NoofV != 0)
  {
    glBindVertexArray(Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ds4VERTEX) * NoofV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ds4VERTEX), (VOID *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ds4VERTEX), (VOID *)sizeof(VEC3));
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ds4VERTEX), (VOID *)(sizeof(VEC3) + sizeof(VEC2)));
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ds4VERTEX), (VOID *)(sizeof(VEC3) * 2 + sizeof(VEC2)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    DS4_RndPrimEvalBB(&Pr->MinBB, &Pr->MaxBB, V, NoofV);
  }
 
  /* Index data */
  if (Ind != NULL && NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    Pr->NumOfElements = NoofI;
  }
  else
    Pr->NumOfElements = NoofV;

  Pr->Trans = MatrIdentity();
  Pr->Type = Type;

  DS4_RndPrimEvalBB(&Pr->MinBB, &Pr->MaxBB, V, NoofV);
} /* End of 'DS4_RndPrimCreate' function */

/* Camera setting function.
 * ARGUMENTS:
 *   - directions:
 *       VEC3 Loc, At, Up;  
 * RETURNS: None.
 */
VOID DS4_RndPrimFree( ds4PRIM *Pr )
{
  if (Pr->VA != 0)
    glBindVertexArray(Pr->VA);
  if (Pr->VBuf != 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &Pr->VBuf);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &Pr->VA);
  }
  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);
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
  MATR
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w)),
    wvp = MatrMulMatr(w, DS4_RndMatrVP);
  INT
    ProgId = DS4_RndMtlApply(Pr->MtlNo),
    loc, i,
    modes[2],
    gl_type = Pr->Type == DS4_RND_PRIM_LINES ? GL_LINES :
              Pr->Type == DS4_RND_PRIM_TRIMESH ? GL_TRIANGLES :
              Pr->Type == DS4_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
              GL_POINT;

  glUseProgram(ProgId);
  if ((loc = glGetUniformLocation(ProgId, "ScrW")) != -1)
    glUniform1i(loc, DS4_Anim.W);
  if ((loc = glGetUniformLocation(ProgId, "ScrH")) != -1)
    glUniform1i(loc, DS4_Anim.H);
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, World.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, DS4_Anim.Time);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &DS4_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "IsWireframe")) != -1)
  {
    glGetIntegerv(GL_POLYGON_MODE, modes);
    glUniform1i(loc, modes[0] != GL_FILL);
  }
  /* Setup shader addons */
  for (i = 0; i < 5; i++)
  {
    static CHAR Name[] = "AddonI0";

    Name[6] = '0' + i;
    Name[5] = 'I';
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
      glUniform1i(loc, DS4_RndShdAddonI[i]);
    Name[5] = 'F';
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
      glUniform1f(loc, DS4_RndShdAddonF[i]);
    Name[5] = 'V';
    if ((loc = glGetUniformLocation(ProgId, Name)) != -1)
      glUniform3fv(loc, 1, &DS4_RndShdAddonV[i].X);
  }

  /* glLoadMatrixf(wvp.A[0]); */

  /* Build projection */
  /*
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC3 p = VecMulMatr(Pr->V[i].P, wvp);
    / *
    DS4_RndProjPoints[i].x = (INT)((p.X + 1) * DS4_RndFrameW / 2);
    DS4_RndProjPoints[i].y = (INT)((-p.Y + 1) * DS4_RndFrameH / 2);
    * /
  }
  */
 
  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
  {
    glDrawArrays(gl_type, 0, Pr->NumOfElements);
  }
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);

  glUseProgram(0);
} /* End of 'DS4_RndPrimDraw' function */

/* Creating quadlirateral function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC3 Org, Du, Dv;  
 * RETURNS: 
 *   None.
 */
VOID DS4_RndPrimCreateQuad( ds4PRIM *Pr, VEC3 Org, VEC3 Du, VEC3 Dv )
{
  ds4VERTEX V[4] = {{{0}}};
  INT I[6];


  memset(Pr, 0, sizeof(ds4PRIM));
  DS4_RndPrimCreate(Pr, DS4_RND_PRIM_TRIMESH, V, 4, I, 6);

  V[0].P = Org;
  V[1].P = VecAddVec(Org, Du);
  V[2].P = VecAddVec(V[1].P, Dv);
  V[3].P = VecAddVec(Org, Dv);

  I[0] = 0;
  I[1] = 1;
  I[2] = 2;

  I[3] = 0;
  I[4] = 2;
  I[5] = 3;
} /* End of 'DS4_RndPrimCreateQuad' function */


/* Creating triangle function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC3 Org, Du, Dv;  
 * RETURNS: None.
 */
VOID DS4_RndPrimCreateTrian( ds4PRIM *Pr, VEC3 Org, VEC3 Du, VEC3 Dv )
{
  ds4VERTEX V[3] = {{{0}}};
  INT I[3];

  memset(Pr, 0, sizeof(ds4PRIM));
  DS4_RndPrimCreate(Pr, DS4_RND_PRIM_TRIMESH, V, 3, I, 3);

  V[0].P = Org;
  V[1].P = VecAddVec(Org, Du);
  V[2].P = VecAddVec(Org, Dv);

  I[0] = 0;
  I[1] = 1;
  I[2] = 2;

} /* End of 'DS4_RndPrimCreateTrian' function */

/* Creating sphere function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - center coordinates:
 *       VEC3 Coord;  
 *   - radius of sphere:
 *       FLT R;
 *   - heigh and width of sphere (parallels and meridians):
 *       FLT Par, Mer;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
/*
BOOL DS4_RndPrimCreateSphere( ds4PRIM *Pr, VEC3 Coord, FLT R, FLT Par, FLT Mer )
{
  if (!DS4_RndPrimCreate(Pr, Pr->NumOfV, (Pr->NumOfV - 2) * 3)


}
*/

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
  INT k, nv = 0, nf = 0, size, *I;
  ds4VERTEX *V;
  VEC3 L;
  VEC4 Color = {1, 1, 1, 1};
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

  size = sizeof(ds4VERTEX) * nv + sizeof(INT) * nf * 3;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  memset(V, 0, size);
  I = (INT *)(V + nv);
    
  /* Read model data */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
    if (Buf[0] == 'c' && Buf[1] == ' ')
    {
      DBL x = 1, y = 1, z = 1, w = 1;
 
      sscanf(Buf + 2, "%lf %lf %lf %lf", &x, &y, &z, &w);
      Color = Vec4Set(x, y, z, w);
    }
    else if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
 
      sscanf(Buf + 2, "%lf %lf %lf", &x, &y, &z);
      V[nv].C = Color;
      V[nv++].P = Vec3Set(x, y, z);
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
            I[nf++] = c0;
            I[nf++] = c1;
            I[nf++] = c;
            c1 = c;
          }
          n++;
        }
        old = *ptr++;
      }
    }

  DS4_RndPrimTrimeshAutoNormals(V, nv, I, nf);
  L = VecNormalize(Vec3Set(1, 1, 1));
  for (k = 0; k < nv; k++)
  {
    FLT nl = VecDotVec(V[k].N, L);

    if (nl < 0.1)
      nl = 0.1;
    V[k].C = Vec4MulNum(V[k].C, nl);
  }

  DS4_RndPrimCreate(Pr, DS4_RND_PRIM_TRIMESH, V, nv, I, nf);

  if (V != NULL)
    free(V);
    
  fclose(F);
  return TRUE;
} /* End of 'DS4_RndPrimLoad' file */

/* Primitive bound box obtaining function.
 * ARGUMENTS:
 *   - pointer to result min-max vectors:
 *       VEC3 *MinBB, *MaxBB;
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 * RETURNS: None.
 */
VOID DS4_RndPrimEvalBB( VEC3 *MinBB, VEC3 *MaxBB, ds4VERTEX *V, INT NoofV )
{
  INT i;
  VEC3 cur_min = V[0].P, cur_max = V[0].P;

  for (i = 1; i < NoofV; i++)
  {
    /* Checking by X */
    if (V[i].P.X < cur_min.X)
      cur_min.X = V[i].P.X;
    else if (V[i].P.X > cur_max.X)
      cur_max.X = V[i].P.X;
    /* Checking by Y */
    if (V[i].P.Y < cur_min.Y)
      cur_min.Y = V[i].P.Y;
    else if (V[i].P.Y > cur_max.Y)
      cur_max.Y = V[i].P.Y;
    /* Checking by Z */
    if (V[i].P.Z < cur_min.Z)
      cur_min.Z = V[i].P.Z;
    else if (V[i].P.Z > cur_max.Z)
      cur_max.Z = V[i].P.Z;
  }
  MinBB->X = cur_min.X;
  MaxBB->X = cur_max.X;
  MinBB->Y = cur_min.Y;
  MaxBB->Y = cur_max.Y;
  MinBB->Z = cur_min.Z;
  MaxBB->Z = cur_max.Z;
} /* End of 'DS4_RndPrimEvalBB' function */

/* Primitive vertex normals evaluate function.
 * ARGUMENTS:
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID DS4_RndPrimTrimeshAutoNormals( ds4VERTEX *V, INT NoofV, INT *Ind, INT NoofI )
{
  INT i;
  
  /* Reset all vertex normals */
  for (i = 0; i < NoofV; i++)
    V[i].N = Vec3Set1(0);
 
  /* Evaluate all triangles normals */
  for (i = 0; i < NoofI; i += 3)
  {
    ds4VERTEX *V0 = V + Ind[i], *V1 = V + Ind[i + 1], *V2 = V + Ind[i + 2];
    VEC3 N = VecNormalize(VecCrossVec(VecSubVec(V1->P, V0->P), VecSubVec(V2->P, V0->P)));
 
    V0->N = VecAddVec(V0->N, N);
    V1->N = VecAddVec(V1->N, N);
    V2->N = VecAddVec(V2->N, N);
  }
 
  /* Normalize all vertex normals */
  for (i = 0; i < NoofV; i++)
    V[i].N = VecNormalize(V[i].N);
} /* End of 'DS4_RndPrimTrimeshAutoNormals' function */
 
/* END OF 'rndprim.c' FILE */

