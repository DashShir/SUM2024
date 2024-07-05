/* FILE NAME  : u_texture.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Textured object unit sample module.
 */

#include "units.h"

/* Texture unit representation type */
typedef struct tagds4UNIT_TEXTURE
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIM Pr;          /* Primitive to draw */
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

  
  if (DS4_RndGridCreateSphere(&G, 1, 30, 18))
  {
    DS4_RndPrimFromGrid(&Uni->Pr, &G);
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
  DS4_RndPrimDraw(&Uni->Pr, MatrTranslate(Vec3Set(-3, 3, 0)));
  DS4_RndPrimDraw(&Uni->Pr, MatrTranslate(Vec3Set(0, 3, 0)));
  DS4_RndPrimDraw(&Uni->Pr, MatrMulMatr3(MatrScale(Vec3Set(1, 0.8, 5)), MatrRotateX(45), MatrTranslate(Vec3Set(3, 3, 0))));
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
