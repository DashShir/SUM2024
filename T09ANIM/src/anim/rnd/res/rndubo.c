/* FILE NAME  : rndubo.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 10.07.2024
 * PURPOSE    : 3D animation project.
 *              Render UBO module.
 */

#include "anim/rnd/rnd.h"

/* UBOs stock */
ds4UBO DS4_RndUBOs[DS4_MAX_UBOS]; /* Array of UBO */
INT DS4_RndUBOsSize;              /* UBO array store size */

/* UBO handle initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndUBOInit( VOID )
{
  DS4_RndUBOsSize = 0;
} /* End of 'DS4_RndUBOInit' function */

/* UBO handle deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndUBOClose( VOID )
{
  INT i;

  for (i = 0; i < DS4_RndUBOsSize; i++)
    glDeleteBuffers(1, &DS4_RndUBOs[i].UBOId);
  DS4_RndUBOsSize = 0;
} /* End of 'DS4_RndUBOClose' function */

/* Add UBO to stock function.
 * ARGUMENTS:
 *   - UBO name:
 *       CHAR *Name;
 *   - OpenGL UBO bind point:
 *       UINT BindPoint;
 *   - UBO size in bytes:
 *       INT Size;
 *   - UBO data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) UBO stock number (-1 if error is occured).
 */
INT DS4_RndUBOCreate( CHAR *Name, UINT BindPoint, INT Size, VOID *Bits )
{
  if (DS4_RndUBOsSize >= DS4_MAX_UBOS)
    return -1;
  /* Size = COM_MAX(Size, 16); */
  /* !!! Size = (Size + 15) >> 4 << 4; - alignment to 16 bytes */
  glGenBuffers(1, &DS4_RndUBOs[DS4_RndUBOsSize].UBOId);
  glBindBuffer(GL_UNIFORM_BUFFER, DS4_RndUBOs[DS4_RndUBOsSize].UBOId);
  glBufferData(GL_UNIFORM_BUFFER, Size, NULL, GL_STATIC_DRAW);

  if (Bits != NULL)
    glBufferSubData(GL_UNIFORM_BUFFER, 0, Size, Bits);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  /* Add to stock */
  DS4_RndUBOs[DS4_RndUBOsSize].Size = Size;
  DS4_RndUBOs[DS4_RndUBOsSize].BindPoint = BindPoint;
  strncpy(DS4_RndUBOs[DS4_RndUBOsSize].Name, Name, DS4_STR_MAX - 1);
  return DS4_RndUBOsSize++;
} /* End of 'DS4_RndUBOCreate' function */

/* UBO data update function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 *   - UBO offset in bytes:
 *       INT Offset;
 *   - UBO update data size in bytes:
 *       INT Size;
 *   - UBO data ('Size' bytes):
 *       VOID *Bits;
 * RETURNS: None.
 */
VOID DS4_RndUBOUpdate( INT UBONo, INT Offset, INT Size, VOID *Bits )
{
  if (UBONo < 0 || UBONo >= DS4_RndUBOsSize)
    return;
  /* Update UBO data */
  if (Offset >= DS4_RndUBOs[UBONo].Size)
    return;
  if (Offset < 0)
    Offset = 0;
  if (Size == 0)
    Size = DS4_RndUBOs[UBONo].Size;
  if (Offset + Size > DS4_RndUBOs[UBONo].Size)
    Size = DS4_RndUBOs[UBONo].Size - Offset;

  glBindBuffer(GL_UNIFORM_BUFFER, DS4_RndUBOs[UBONo].UBOId);
  glBufferSubData(GL_UNIFORM_BUFFER, Offset, Size, Bits);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
} /* End of 'DS4_RndUBOUpdate' function */

/* UBO apply function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 * RETURNS: None.
 */
VOID DS4_RndUBOApply( INT UBONo )
{
  if (UBONo < 0 || UBONo >= DS4_RndUBOsSize)
    return;

  glBindBufferBase(GL_UNIFORM_BUFFER, DS4_RndUBOs[UBONo].BindPoint,
    DS4_RndUBOs[UBONo].UBOId);
} /* End of 'DS4_RndUBOApply' function */

/* END OF 'rndubo.c' FILE */
