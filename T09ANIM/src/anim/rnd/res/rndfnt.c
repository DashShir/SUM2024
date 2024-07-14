/* FILE NAME  : rndfnt.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 10.07.2024
 * PURPOSE    : 3D animation project.
 *              Render fonts module.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* Font description structure */
typedef struct tagds4FONT
{
  DWORD LineH, BaseH; /* Font line height and base line height in pixels */
  FLT AdvanceX[256];  /* Every letter shift right value (0 if no letter present) */
} ds4FONT;

/* Current font description */
/* Font data */
static ds4FONT DS4_RndFntFont;

/* Font characters primitives */
static ds4PRIM DS4_RndFntChars[256];

/* Font material */
static INT DS4_RndFntMtlNo;

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndFntLoad( CHAR *FileName )
{
  INT i;
  DWORD Sign, W, H, *tex;
  FILE *F;
  ds4MATERIAL mtl;
  static ds4VERTEX Chars[256][4];

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  /* Read font signatur */
  if (fread(&Sign, 4, 1, F) != 1 || Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }

  /* Read font header */
  fread(&DS4_RndFntFont, sizeof(ds4FONT), 1, F);

  /* Read font character vertexes */
  fread(Chars, sizeof(ds4VERTEX), 256 * 4, F);

  /* Read font texture */
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);
  if ((tex = malloc(4 * W * H)) == NULL)
  {
    memset(&DS4_RndFntFont, 0, sizeof(DS4_RndFntFont));
    fclose(F);
    return FALSE;
  }
  fread(tex, 4, W * H, F);
  fclose(F);

  /* Create font material */
  mtl = DS4_RndMtlGetDef();
  strncpy(mtl.Name, FileName, DS4_STR_MAX - 1);
  mtl.ShdNo = DS4_RndShdAdd("font");
  mtl.Tex[0] = DS4_RndTexAddImg(FileName, W, H, 4, tex);
  DS4_RndFntMtlNo = DS4_RndMtlAdd(&mtl);

  /* Create font chars primitives */
  for (i = 0; i < 256; i++)
  {
    DS4_RndPrimFree(&DS4_RndFntChars[i]);
    DS4_RndPrimCreate(&DS4_RndFntChars[i], DS4_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    DS4_RndFntChars[i].MtlNo = DS4_RndFntMtlNo;
  }

  return FALSE;
} /* End of 'DS4_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndFntInit( VOID )
{
  DS4_RndFntLoad("bin/fonts/Book.g3df");
} /* End of 'DS4_RndFntInit' function */

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    DS4_RndPrimFree(&DS4_RndFntChars[i]);
  memset(DS4_RndFntChars, 0, sizeof(DS4_RndFntChars));
  memset(&DS4_RndFntFont, 0, sizeof(DS4_RndFntFont));
} /* End of 'DS4_RndFntInit' function */

/* Draw string function.
 * ARGUMENTS:
 *   - string to draw:
 *       CHAR *Str;
 *   - draw position:
 *       VEC3 Pos;
 *   - font size:
 *      FLT Size;
 *   - draw color:
 *       VEC3 Color;
 * RETURNS: None.
 */
VOID DS4_RndFntDraw( CHAR *Str, VEC3 Pos, FLT Size, VEC3 Color )
{
  VEC3 p = Pos;

  DS4_RndShdAddonV[0] = Color;
  while (*Str != 0)
  {
    if (*Str == '\n')
      Pos.X = p.X, Pos.Y -= Size;
    else
      if (DS4_RndFntFont.AdvanceX[(BYTE)*Str] != 0)
      {
        DS4_RndPrimDraw(&DS4_RndFntChars[(BYTE)*Str], MatrMulMatr(MatrScale(Vec3Set(Size, Size, 1)), MatrTranslate(Pos)));
        Pos.X += DS4_RndFntFont.AdvanceX[(BYTE)*Str] * Size;
      }
    Str++;
  }
} /* End of 'DS4_RndFntDraw' function */

/* END OF 'rndfnt.c' FILE */
