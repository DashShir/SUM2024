/* FILE NAME  : rndmtl.c         
 * PROGRAMMER : DS4
 * LAST UPDATE: 06.07.2024
 * PURPOSE    : 3D animation project.
 *          Render material module.
 */

#include "anim/anim.h"

static struct
{
  char *Name;
  VEC3 amb, dif, spec, shin;
} MatLib[] =
{
  {"Black Plastic", {0.0, 0.0, 0.0},             {0.01, 0.01, 0.01},           {0.5, 0.5, 0.5},               32},
  {"Brass",         {0.329412,0.223529,0.027451}, {0.780392,0.568627,0.113725}, {0.992157,0.941176,0.807843}, 27.8974},
  {"Bronze",        {0.2125,0.1275,0.054},       {0.714,0.4284,0.18144},       {0.393548,0.271906,0.166721},  25.6},
  {"Chrome",        {0.25, 0.25, 0.25},          {0.4, 0.4, 0.4},              {0.774597, 0.774597, 0.774597}, 76.8},
  {"Copper",        {0.19125,0.0735,0.0225},     {0.7038,0.27048,0.0828},      {0.256777,0.137622,0.086014},  12.8},
  {"Gold",          {0.24725,0.1995,0.0745},     {0.75164,0.60648,0.22648},    {0.628281,0.555802,0.366065},  51.2},
  {"Peweter",       {0.10588,0.058824,0.113725}, {0.427451,0.470588,0.541176}, {0.3333,0.3333,0.521569},      9.84615},
  {"Silver",        {0.19225,0.19225,0.19225},   {0.50754,0.50754,0.50754},    {0.508273,0.508273,0.508273},  51.2},
  {"Polished Silver", {0.23125,0.23125,0.23125}, {0.2775,0.2775,0.2775},       {0.773911,0.773911,0.773911},  89.6},
  {"Turquoise",     {0.1, 0.18725, 0.1745},      {0.396, 0.74151, 0.69102},    {0.297254, 0.30829, 0.306678}, 12.8},
  {"Ruby",          {0.1745, 0.01175, 0.01175},  {0.61424, 0.04136, 0.04136},  {0.727811, 0.626959, 0.626959}, 76.8},
  {"Polished Gold", {0.24725, 0.2245, 0.0645},   {0.34615, 0.3143, 0.0903},    {0.797357, 0.723991, 0.208006}, 83.2},
  {"Polished Bronze", {0.25, 0.148, 0.06475},    {0.4, 0.2368, 0.1036},        {0.774597, 0.458561, 0.200621}, 76.8},
  {"Polished Copper", {0.2295, 0.08825, 0.0275}, {0.5508, 0.2118, 0.066},      {0.580594, 0.223257, 0.0695701}, 51.2},
  {"Jade",          {0.135, 0.2225, 0.1575},     {0.135, 0.2225, 0.1575},      {0.316228, 0.316228, 0.316228}, 12.8},
  {"Obsidian",      {0.05375, 0.05, 0.06625},    {0.18275, 0.17, 0.22525},     {0.332741, 0.328634, 0.346435}, 38.4},
  {"Pearl",         {0.25, 0.20725, 0.20725},    {1.0, 0.829, 0.829},          {0.296648, 0.296648, 0.296648}, 11.264},
  {"Emerald",       {0.0215, 0.1745, 0.0215},    {0.07568, 0.61424, 0.07568},  {0.633, 0.727811, 0.633},       76.8},
  {"Black Rubber",  {0.02, 0.02, 0.02},          {0.01, 0.01, 0.01},           {0.4, 0.4, 0.4},                10.0},
};


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
  INT i;
  struct
  {
    VEC4 MtlKa;
    VEC4 MtlKdTrans;
    VEC4 MtlKsPh;
    INT MtlIsTextures[8];
  } mtl_buf;

  if (DS4_RndMaterialsSize >= DS4_MAX_MATERIALS)
    return 0;

  DS4_RndMaterials[DS4_RndMaterialsSize] = *Mtl;

  mtl_buf.MtlKa = Vec4SetVec3(Mtl->Ka, 0);
  mtl_buf.MtlKdTrans = Vec4SetVec3(Mtl->Kd, Mtl->Trans);
  mtl_buf.MtlKsPh = Vec4SetVec3(Mtl->Ks, Mtl->Ph);
  for (i = 0; i < 8; i++)
    mtl_buf.MtlIsTextures[i] = Mtl->Tex[i] != -1;
  DS4_RndMaterials[DS4_RndMaterialsSize].UBONo = DS4_RndUBOCreate(Mtl->Name, DS4_RND_UBO_BIND_MATERIAL, sizeof(mtl_buf), &mtl_buf);
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
  /*
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
  */
  DS4_RndUBOApply(mtl->UBONo);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    /*
    CHAR tname[] = "IsTexture0"; 
    tname[9] = '0' + i;
    */

    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, DS4_RndTextures[mtl->Tex[i]].TexId);
    }
    /*
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    */
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
