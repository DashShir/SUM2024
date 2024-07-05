/* FILE NAME  : rndres.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 05.07.2024
 * PURPOSE    : 3D animation project.
 *          Render resourses module.
 */

#include "rndres.h"

/* Resourses initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndResInit( VOID )
{
  DS4_RndShdInit();
  DS4_RndTexInit();
  DS4_RndMtlInit();
} /* End of 'DS4_RndResInit' function */

/* Resourses close function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndResClose( VOID )
{
  DS4_RndMtlClose();
  DS4_RndTexClose();
  DS4_RndShdClose();
} /* End of 'DS4_RndResClose' function */


/* END OF 'rndres.c' FILE */