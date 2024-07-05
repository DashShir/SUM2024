/* FILE NAME  : rndshd.c         
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *          Render material module.
 */

#include "anim/anim.h"

#include "rndres.h"

/* Material stock */
ds4MATERIAL DS4_RndMaterials[DS4_MAX_MATERIALS]; /* Array of materials */
INT DS4_RndMaterialsSize;                        /* Materials array */

/* Material system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndMtlInit( VOID )
{
  ds4MATERIAL def = DS4_RndMtlGetDef();

  DS4_RndMaterialsSize = 0;
  DS4_RndMtlAdd(&def);
} /* End of 'DS4_RndMtlInit' function */

/* Material system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndMtlClose( VOID )
{
  DS4_RndMaterialsSize = 0;
} /* End of 'DS4_RndMtlClose' function */

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) added material number in DS4_RndMaterials.
 */
INT DS4_RndMtlAdd( ds4MATERIAL *Mtl )
{
  if (DS4_RndMaterialsSize >= DS4_MAX_MATERIALS)
    return 0;

  DS4_RndMaterials[DS4_RndMaterialsSize] = *Mtl;
  return DS4_RndMaterialsSize++;
} /* End of 'DS4_RndMtlAdd' function */

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4MATERIAL) base material.
 */
ds4MATERIAL DS4_RndMtlGetDef( VOID )
{
  ds4MATERIAL def_mtl =
  {
    "Default",
    {0.1, 0.1, 0.1},
    {0.9, 0.9, 0.9},
    {0.3, 0.3, 0.3},
    30, 1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    0
  };

  return def_mtl;
} /* End of 'DS4_RndMtlGetDef' function */

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) shader ProgId.
 */
INT DS4_RndMtlApply( INT MtlNo )
{
  INT i, prg, loc;
  ds4MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= DS4_RndMaterialsSize)
    MtlNo = 0;
  mtl = &DS4_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= DS4_RndShadersSize)
    prg = 0;
  prg = DS4_RndShaders[prg].ProgId;

  if (prg == 0)
    return 0;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, DS4_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, DS4_Anim.GlobalTime);

  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
  if ((loc = glGetUniformLocation(prg, "ShdNo")) != -1)
    glUniform1i(loc, mtl->ShdNo);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0"; 
    tname[9] = '0' + i;

    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, DS4_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'DS4_RndMtlApply' function */

/* Getting material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   None.
 */
ds4MATERIAL * DS4_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo >= DS4_RndMaterialsSize)
    MtlNo = 0;
  return &DS4_RndMaterials[MtlNo];
} /* End of 'DS4_RndMtlGet' function */

/* END OF 'rndmtl.c' FILE */
