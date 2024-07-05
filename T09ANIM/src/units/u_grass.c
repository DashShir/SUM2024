/* FILE NAME  : u_grass.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Garss unit sample module.
 */

#include "units.h"

typedef struct tagds4UNIT_GRASS
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIM Grass;        /* Grass model */
} ds4UNIT_GRASS;

/* Grass unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_GRASS *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_GRASS *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimLoad(&Uni->Grass, "bin/models/obj/corn.obj");
} /* End of 'DS4_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_GRASS *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_GRASS *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimFree(&Uni->Grass);
} /* End of 'DS4_UnitClose' function */

/* Grass unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_GRASS *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_GRASS * Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Grass unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_GRASS * Uni, ds4ANIM *Ani)
{
  DS4_RndPrimDraw(&Uni->Grass, MatrMulMatr(MatrRotateY(13), MatrTranslate(/* Vec3Set(55, 14.4, -25) */ Vec3Set(10, 0, 10))));
} /* End of 'DS4_UnitRender' function */

/* Grass unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateGrass( VEC3 DrawPos )
{
  ds4UNIT_GRASS *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_GRASS *)DS4_AnimUnitCreate(sizeof(ds4UNIT_GRASS))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateGrass' function */

/* END OF 'u_grass.c' FILE */