/* FILE NAME  : rndtex.c         
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *          Render texture module.
 */

#include <stdio.h>

#include "rndres.h"

/* Tetxure stock */
ds4TEXTURE DS4_RndTextures[DS4_MAX_TEXTURES];
INT DS4_RndTexturesSize;

/* Array initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexInit( VOID )
{

} /* End of 'DS4_RndTexInit' function */

/* All textures pushing function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexClose( VOID )
{
} /* End of 'DS4_RndTexClose' function */

/* Get texture from image function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  return 1;
} /* End of 'DS4_RndTexAddImg' function */

/* Get texture from file function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTexAddFromFile( CHAR *FileName )
{
  HBITMAP hBm;
  FILE *F;
  INT res = -1;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    BITMAP bm;

    GetObject(hBm, sizeof(BITMAP), &bm);
    res = DS4_RndTexAddImg(FileName, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel / 8, bm.bmBits);
    DeleteObject(hBm);
    return res;
  }
  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w = 0, h = 0, flen, bpp = 0;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    if (flen == 4 + w * h * 3)
      bpp = 3;
    else if (flen == 4 + w * h * 4)
      bpp = 4;
    if (bpp != 0)
    {
      /* Case of .G24 or .G32 image file */
      BYTE *mem;

      if ((mem = malloc(w * h * bpp)) != NULL)
      {
        fseek(F, 4, SEEK_SET);
        fread(mem, bpp, w * h, F);
        res = DS4_RndTexAddImg(FileName, w, h, bpp, mem);
        free(mem);
      }
    }
    fclose(F);
  }
  return res;
} /* End of 'DS4_RndTexAddFromFile' function */

/* END OF 'rndtex.c' FILE */
