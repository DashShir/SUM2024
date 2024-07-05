/* FILE NAME  : u_floor.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Floor unit sample module.
 */

#include "units.h"

typedef struct tagds4UNIT_FLOOR
{
  DS4_UNIT_BASE_FIELDS;
  VEC3 StartPos, Pos;    /* Start and shift positions */
  ds4PRIM Floor;          /* Cow model */
} ds4UNIT_FLOOR;

/* Floor unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_FLOOR *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_FLOOR *Uni, ds4ANIM *Ani )
{
  Uni->Pos = Vec3Set1(0);
  DS4_RndPrimCreateQuad(&Uni->Floor, Vec3Set(0, 0, 0), Vec3Set(1, 0, 0), Vec3Set(0, 0, 1));
} /* End of 'DS4_UnitInit' function */

/* Floor unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_FLOOR *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_FLOOR *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimFree(&Uni->Floor);
} /* End of 'DS4_UnitClose' function */

/* Floor unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_FLOOR *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_FLOOR *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Floor unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_FLOOR * Uni, ds4ANIM *Ani)
{
  DS4_RndPrimDraw(&Uni->Floor, MatrRotateY(21));
} /* End of 'DS4_UnitRender' function */

/* Floor unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateFloor( VEC3 DrawPos )
{
  ds4UNIT_FLOOR *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_FLOOR *)DS4_AnimUnitCreate(sizeof(ds4UNIT_FLOOR))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  Uni->StartPos = DrawPos;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateFloor' function */

/* END OF 'u_floor.c' FILE */