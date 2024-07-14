/* FILE NAME  : rndprims.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 08.07.2024
 * PURPOSE    : 3D animation project.
 *              Primitive handle module.
 */

#include <stdio.h>

#include "anim/anim.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ds4PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DS4_RndPrimsCreate( ds4PRIMS *Prs, INT NumOfPrims )
{
  memset(Prs, 0, sizeof(ds4PRIMS));
  if ((Prs->Prims = malloc(sizeof(ds4PRIM) * NumOfPrims)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(ds4PRIM) * NumOfPrims);
  Prs->NumOfPrims = NumOfPrims;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'DS4_RndPrimsCreate' function */

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       vg4PRIMS *Prs;
 * RETURNS: None.
 */
VOID DS4_RndPrimsFree( ds4PRIMS *Prs )
{
  INT i;

  if (Prs->Prims != NULL)
  {
    for (i = 0; i < Prs->NumOfPrims; i++)
      DS4_RndPrimFree(&Prs->Prims[i]);
    free(Prs->Prims);
  }
  memset(Prs, 0, sizeof(ds4PRIMS));
} /* End of 'DS4_RndPrimsFree' function */

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ds4PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DS4_RndPrimsDraw( ds4PRIMS *Prs, MATR World )
{
  MATR m = MatrMulMatr(Prs->Trans, World);
  INT i;

  DS4_RndShdAddonI[0] = Prs->NumOfPrims;
  /* Draw all transparent primitives with front face culling */ 
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DS4_RndMtlGet(Prs->Prims[i].MtlNo)->Trans == 1)
      DS4_RndShdAddonI[1] = i, DS4_RndPrimDraw(&Prs->Prims[i], m);
 
  /* Draw all transparent primitives with front face culling */ 
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DS4_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      DS4_RndShdAddonI[1] = i, DS4_RndPrimDraw(&Prs->Prims[i], m);
 
  /* Draw all transparent primitives with back face culling */ 
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (DS4_RndMtlGet(Prs->Prims[i].MtlNo)->Trans != 1)
      DS4_RndShdAddonI[1] = i, DS4_RndPrimDraw(&Prs->Prims[i], m);
 
  glDisable(GL_CULL_FACE);
} /* End of 'DS4_RndPrimsDraw' function */

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ds4PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL DS4_RndPrimsLoad( ds4PRIMS *Prs, CHAR *FileName )
{
  FILE *F;
  INT flen;
  DWORD p, m, t;
  BYTE *mem, *ptr;
  DWORD Sign; /* == "G3DM" */
  DWORD NumOfPrims;
  DWORD NumOfMaterials;
  DWORD NumOfTextures;
  DWORD NumOfVertexes;
  DWORD NumOfFacetIndexes; /* num of facets * 3 */
  DWORD MtlNo;             /* material number in table below (in material section) */

  memset(Prs, 0, sizeof(ds4PRIMS));
 
  /* Open file */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
 
  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);
 
  /* Allocate memory and load whole file to memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  fread(mem, 1, flen, F);
  fclose(F);
 
  ptr = mem;
  /* Signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }
  NumOfPrims = *(DWORD *)ptr;
  ptr += 4;
  NumOfMaterials = *(DWORD *)ptr;
  ptr += 4;
  NumOfTextures = *(DWORD *)ptr;
  ptr += 4;
 
  if (!DS4_RndPrimsCreate(Prs, NumOfPrims))
  {
    free(mem);
    return FALSE;
  }

  /* Load primitives */
  for (p = 0; p < NumOfPrims; p++)
  {
    ds4VERTEX *vrt;
    INT * ind;

    NumOfVertexes = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 4;
    MtlNo = *(DWORD *)ptr;
    ptr += 4;
    vrt = (VOID *)ptr;
    ptr += sizeof(*vrt) * NumOfVertexes;
    ind = (VOID *)ptr;
    ptr += 4 * NumOfFacetIndexes;
    DS4_RndPrimCreate(&Prs->Prims[p], DS4_RND_PRIM_TRIMESH, vrt, NumOfVertexes, ind, NumOfFacetIndexes);
    Prs->Prims[p].MtlNo = DS4_RndMaterialsSize + MtlNo;
  }

  /* Load materials */
  for (m = 0; m < NumOfMaterials; m++)
  {
    struct
    {
      CHAR Name[300]; /* Material name */
      /* Illumination coefficients */
      VEC3 Ka, Kd, Ks;     /* Ambient, diffuse, specular coefficients */
      FLT Ph;             /* Phong power coefficient – shininess */
      FLT Trans;          /* Transparency factor */
      DWORD Tex[8];       /* Texture references 
                           * (8 time: texture number in G3DM file, -1 if no texture) */
      /* Shader information */
      CHAR ShaderString[300]; /* Additional shader information */
      DWORD Shader;       /* Shader number (uses after load into memory) */
    } *fmat;
    ds4MATERIAL mtl;
 
    fmat = (VOID *)ptr;
    ptr += sizeof(*fmat);
    strncpy(mtl.Name, fmat->Name, DS4_STR_MAX);
    mtl.Ka = fmat->Ka;
    mtl.Kd = fmat->Kd;
    mtl.Ks = fmat->Ks;
    mtl.Ph = fmat->Ph;
    mtl.Trans = fmat->Trans;
    for (t = 0; t < 8; t++)
      mtl.Tex[t] = fmat->Tex[t] == -1 ? -1 : DS4_RndTexturesSize + fmat->Tex[t];
    DS4_RndMtlAdd(&mtl);
  }
 
  /* Load textures */
  for (t = 0; t < NumOfTextures; t++)
  {
    CHAR *Name;
    DWORD W, H, C;
 
    Name = (CHAR *)ptr;
    ptr += 300;
    W = *(DWORD *)ptr;
    ptr += 4;
    H = *(DWORD *)ptr;
    ptr += 4;
    C = *(DWORD *)ptr;
    ptr += 4;
    DS4_RndTexAddImg(Name, W, H, C, ptr);
    ptr += W * H * C;
  }

  return TRUE;
}


/* END OF 'rndprims.c' FILE */