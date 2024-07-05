/* FILE NAME  : rndshd.c         
 * PROGRAMMER : DS4
 * LAST UPDATE: 29.06.2024
 * PURPOSE    : 3D animation project.
 *          Render shader module.
 */

#include <stdio.h>
#include <glew.h>

#include "rndres.h"

/* Save log to file function.
 * ARGUMENTS:
 *   - shader prefix:
 *       CHAR *FileNamePrefix;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - error text:
 *       CHAR *Text;
 * RETURNS: None.
 */
static VOID DS4_RndShdLog( CHAR *FileNamePrefix, CHAR *ShaderName, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("bin/shaders/shd{30}vg4.log", "a")) == NULL)
    return;
  fprintf(F, "%s : %s\n%s\n\n", FileNamePrefix, ShaderName, Text);
  fclose(F);
} /* End of 'DS4_RndShdLog' function */

/* Load shader text from file function.
 * ARGUMENTS:
 *   - text file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text.
 */
static CHAR * DS4_RndLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  INT flen;
  CHAR *txt;

  if ((F = fopen(FileName, "rb")) == NULL)
    return NULL;

  /* Measure file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);

  /* Allocate memory */
  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);

  /* Load text */
  rewind(F);
  fread(txt, 1, flen, F);

  fclose(F);
  return txt;
} /* End of 'DS4_RndLoadTextFromFile' function */

/* Load shader program function.
 * ARGUMENTS:
 *   - shader folder prefix (in 'BIN/SHADERS/***'):
 *       CHAR *FileNamePrefix;
 * RETUNS:
 *   (UINT) load shader program Id.
 */
static UINT DS4_RndShdLoad( CHAR *FileNamePrefix )
{
  struct
  {
    CHAR *Name;  /* Shader name (e.g. "VERT") */
    INT Type;    /* Shader type (e.g. GL_VERTEX_SHADER) */
    INT Id;      /* Created shader Id */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER, 0},
    {"FRAG", GL_FRAGMENT_SHADER, 0},
  };
  INT i, prg, res, NoofS = sizeof(shd) / sizeof(shd[0]);
  BOOL is_ok = TRUE;
  static CHAR Buf[10000];
  char * txt;

  for (i = 0; i < NoofS; i++)
  {
    /* Build shader name */
    sprintf(Buf, "bin/shaders/%s/%s.glsl", FileNamePrefix, shd[i].Name);

    /* Load shader text from file */
    txt = DS4_RndLoadTextFromFile(Buf);
    if (txt == NULL)
    {
      DS4_RndShdLog(FileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }

    /* Create shader */
    shd[i].Id = glCreateShader(shd[i].Type);
    if (shd[i].Id == 0)
    {
      free(txt);
      DS4_RndShdLog(FileNamePrefix, shd[i].Name, "Error shader create");
      is_ok = FALSE;
      break;
    }

    /* Send shader source text to OpenGL */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);

    /* Errors handle */
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      DS4_RndShdLog(FileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  /* Create program */
  if (is_ok)
    if ((prg = glCreateProgram()) == 0)
    {
      DS4_RndShdLog(FileNamePrefix, "PROG", "Error create program");
      is_ok = FALSE;
    }
    else
    {
      /* Attach shader programs */
      for (i = 0; i < NoofS; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link program */
      glLinkProgram(prg);
      /* Errors handle */
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        DS4_RndShdLog(FileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }

  /* Handle errors */
  if (!is_ok)
  {
    /* Delete all shaders */
    for (i = 0; i < NoofS; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Delete program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }
  return prg;
} /* End of 'DS4_RndShdLoad' function */

/* Delete shader program function.
 * ARGUMENTS:
 *   - shader program Id:
 *       UINT ProgId;
 * RETUNS: None.
 */
static VOID DS4_RndShdFree( UINT ProgId )
{
  INT shds[5], n, i;

  if (ProgId == 0 || !glIsProgram(ProgId))
    return;
  glGetAttachedShaders(ProgId, 5, &n, shds);
  for (i = 0; i < n; i++)
    if (glIsShader(shds[i]))
    {
      glDetachShader(ProgId, shds[i]);
      glDeleteShader(shds[i]);
    }
  glDeleteProgram(ProgId);
} /* End of 'DS4_RndShdFree' function */

/***
 * Shaders stock functions
 ***/

/* Array of shaders */
ds4SHADER DS4_RndShaders[DS4_MAX_SHADERS];

/* Shadres array store size */
INT DS4_RndShadersSize;

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT DS4_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  INT i;

  for (i = 0; i < DS4_RndShadersSize; i++)
    if (strcmp(ShaderFileNamePrefix, DS4_RndShaders[i].Name) == 0)
      return i;

  if (DS4_RndShadersSize >= DS4_MAX_SHADERS)
    return 0;

  strncpy(DS4_RndShaders[DS4_RndShadersSize].Name, ShaderFileNamePrefix,
          DS4_STR_MAX - 1);
  DS4_RndShaders[DS4_RndShadersSize].ProgId =
    DS4_RndShdLoad(ShaderFileNamePrefix);
  return DS4_RndShadersSize++;
} /* End of 'DS4_RndShdAdd' function */

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < DS4_RndShadersSize; i++)
  {
    DS4_RndShdFree(DS4_RndShaders[i].ProgId);
    DS4_RndShaders[i].ProgId = DS4_RndShdLoad(DS4_RndShaders[i].Name);
  }
} /* End of 'DS4_RndShdUpdate' function */

/* Shader storage delete function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShaderDelete( INT ShdNo )
{
} /* End of 'DS4_RndShaderDelete' function */

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdInit( VOID )
{
  DS4_RndShadersSize = 0;
  DS4_RndShdAdd("default");
} /* End of 'DS4_RndShdInit' function */

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < DS4_RndShadersSize; i++)
    DS4_RndShdFree(DS4_RndShaders[i].ProgId);
  DS4_RndShadersSize = 0;
} /* End of 'DS4_RndShdClose' function */

/* END OF 'rndshd.c' FILE */