/* FILE NAME  : u_texture.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 08.07.2024
 * PURPOSE    : 3D animation project.
 *              Textured object unit sample module.
 */

#include <stdio.h>

#include "units.h"

/* Texture unit representation type */
typedef struct tagds4UNIT_TEXTURE
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIM Pr;          /* Primitive to draw */
  ds4PRIM Pr1;         /* Square primitive to draw */
} ds4UNIT_TEXTURE;

/* Textured object unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_TEXTURE *Uni, ds4ANIM *Ani )
{
  ds4GRID G;
  ds4MATERIAL mtl = DS4_RndMtlGetDef();
  
  if (DS4_RndGridCreateSphere(&G, 1, 30, 18))
  {
    DS4_RndPrimFromGrid(&Uni->Pr, &G);
    strcpy(mtl.Name, "Globe");
    mtl.Tex[0] = DS4_RndTexAddFromFile("X:/PICS/GLOBE.G24");
    Uni->Pr.MtlNo = DS4_RndMtlAdd(&mtl);

    DS4_RndGridFree(&G);
  }
  if (DS4_RndGridCreate(&G, 2, 2))
  {
    DS4_RndPrimFromGrid(&Uni->Pr1, &G);
    strcpy(mtl.Name, "Mandrill");
    mtl.Tex[0] = DS4_RndTexAddFromFile("X:/PICS/M.G24");
    Uni->Pr1.MtlNo = DS4_RndMtlAdd(&mtl);

    DS4_RndGridFree(&G);
  }
  /*
  if (DS4_RndGridCreateTorus(&G, 1, 2, 30, 18))
  {
    DS4_RndPrimFromGrid(&Uni->Pr, &G);
    DS4_RndGridFree(&G);
  }
  */
} /* End of 'DS4_UnitInit' function */

/* Textured object unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_TEXTURE *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimFree(&Uni->Pr);
  DS4_RndPrimFree(&Uni->Pr1);
} /* End of 'DS4_UnitClose' function */

/* Textured object unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_TEXTURE *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Textured object unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_TEXTURE * Uni, ds4ANIM *Ani)
{
  /* MATR OldView = DS4_RndMatrView, OldVP = DS4_RndMatrVP, OldProj = DS4_RndMatrProj; */
  static CHAR Buf[1000];

  DS4_RndPrimDraw(&Uni->Pr, MatrTranslate(Vec3Set(-3, 3, 0)));
  DS4_RndPrimDraw(&Uni->Pr, MatrTranslate(Vec3Set(0, 3, 0)));
  DS4_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrScale(Vec3Set(1, 0.8, 5)), MatrRotateX(45), MatrTranslate(Vec3Set(3, 3, 0))));

  DS4_RndPrimDraw(&Uni->Pr1, MatrTranslate(Vec3Set(0, 0, 0)));

  /* DS4_RndMatrVP = MatrOrtho(0, DS4_RndFrameW - 1, -(DS4_RndFrameH - 1), 0, -1, 1); */
  sprintf(Buf, "CGSG forever!!!\nFPS: %.3f\nDS4 Animation", Ani->FPS);
  DS4_RndFntDraw(Buf, Vec3Set(0, 0, 0), 47, Vec3Set(1, 1, 1));

  /*
  DS4_RndMatrView = OldView;
  DS4_RndMatrVP = OldVP;
  DS4_RndMatrProj = OldProj;
  */
} /* End of 'DS4_UnitRender' function */

/* Textured object unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateTexture( VOID )
{
  ds4UNIT_TEXTURE *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_TEXTURE *)DS4_AnimUnitCreate(sizeof(ds4UNIT_TEXTURE))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateFloor' function */

/* END OF 'u_texture.c' FILE */
