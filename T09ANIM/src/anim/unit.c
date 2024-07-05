/* FILE NAME  : unit.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *          Unit setting module.
 */

#include "anim.h"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitInit' function */

/* Unit closing function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitClose' function */

/* Unit responsing function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Unit rendering function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       vg4UNIT *Uni;
 *   - animation context:
 *       vg4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender( ds4UNIT *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_AnimUnitCreate( INT Size )
{
  ds4UNIT *Uni;
 
  /* Memory allocation */
  if (Size < sizeof(ds4UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
 
  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return Uni;
} /* End of 'DS4_AnimUnitCreate' function */

/* END OF 'unit.c' FILE */