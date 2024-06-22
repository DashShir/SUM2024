/* FILE NAME  : u_cow.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 22.06.2024
 * PURPOSE    : 3D animation project.
 *              Bounce cow unit sample module.
 */

#include "units.h"

typedef struct tagds4UNIT_COW
{
  DS4_UNIT_BASE_FIELDS;
  VEC Pos;
  ds4PRIM Cow;
} ds4UNIT_COW;

/* Dancing cow unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_COW *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_COW *Uni, ds4ANIM *Ani )
{
  Uni->Pos = VecSet(0, 1, 0);
  DS4_RndPrimLoad(&Uni->Cow, "COW1.OBJ");
} /* End of 'DS4_UnitInit' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_COW *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitClose' function */

/* Bounce cow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_BALL *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_COW * Uni, ds4ANIM *Ani )
{
  
} /* End of 'DS4_UnitResponse' function */

static VOID DS4_UnitRender(ds4UNIT_COW * Uni, ds4ANIM *Ani)
{

} /* End of 'DS4_UnitRender' function */

/* Bounce cow unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateCow( VEC DrawPos )
{
  ds4UNIT_COW *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_COW *)DS4_AnimUnitCreate(sizeof(ds4UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  Uni->Pos = DrawPos;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateCow' function */

/* END OF 'u_cow.c' FILE */