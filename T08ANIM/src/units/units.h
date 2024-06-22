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
 *       VEC DrawPos;
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateCow( VEC DrawPos );

#endif /* __units_h_ */
 
/* END OF 'units.h' FILE */ 