/* FILE NAME  : u_island.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Island unit sample module.
 */

#include "units.h"

/**/
typedef struct tagds4UNIT_ISLAND
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIM Island;        /* Island model */
} ds4UNIT_ISLAND;

/* Island unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_ISLAND *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_ISLAND *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimLoad(&Uni->Island, "bin/models/obj/Island_001.obj");
} /* End of 'DS4_UnitInit' function */

/* Island unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_ISLAND *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_ISLAND *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimFree(&Uni->Island);
} /* End of 'DS4_UnitClose' function */

/* Grass unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_ISLAND *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_ISLAND * Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Island unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_ISLAND *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_ISLAND * Uni, ds4ANIM *Ani)
{
  DS4_RndPrimDraw(&Uni->Island, MatrRotateY(13));
} /* End of 'DS4_UnitRender' function */

/* Island unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateIsland( VEC3 DrawPos )
{
  ds4UNIT_ISLAND *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_ISLAND *)DS4_AnimUnitCreate(sizeof(ds4UNIT_ISLAND))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateIsland' function */

/* END OF 'u_island.c' FILE */