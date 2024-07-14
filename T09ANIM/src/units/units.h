/* FILE NAME  : units.h
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *          Units declaration module.
 */

#ifndef __units_h_
#define __units_h_

#include "anim/anim.h"

/* Bounce cow unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateCow( VEC3 DrawPos );

/* Control unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS: None.
 */
ds4UNIT * DS4_UnitCreateCtrl( VOID );

/* Floor unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateFloor( VEC3 DrawPos );

/* Grass unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
 ds4UNIT * DS4_UnitCreateGrass( VEC3 DrawPos );

 /* Island unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateIsland( VEC3 DrawPos );

/* Mountain unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateMountain( VOID );

/* Textured object unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateTexture( VOID );

/* Floor unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateG3DM( VOID );

#endif /* __units_h_ */
 
/* END OF 'units.h' FILE */ 