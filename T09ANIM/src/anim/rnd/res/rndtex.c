/* FILE NAME  : rndtex.c         
 * PROGRAMMER : DS4
 * LAST UPDATE: 06.07.2024
 * PURPOSE    : 3D animation project.
 *          Render texture module.
 */

#include <stdio.h>
#include <string.h>

#include "anim/rnd/rnd.h"

/* Tetxure stock */
ds4TEXTURE DS4_RndTextures[DS4_MAX_TEXTURES];
INT DS4_RndTexturesSize;

/* Array initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexInit( VOID )
{
  DS4_RndTexturesSize = 0;
} /* End of 'DS4_RndTexInit' function */

/* All textures pushing function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < DS4_RndTexturesSize; i++)
    glDeleteTextures(1, &DS4_RndTextures[i].TexId);
  DS4_RndTexturesSize = 0;
} /* End of 'DS4_RndTexClose' function */

/* Get texture from image function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  if (DS4_RndTexturesSize >= DS4_MAX_TEXTURES)
    return 0;

  strncpy(DS4_RndTextures[DS4_RndTexturesSize].Name, Name, DS4_STR_MAX - 1);
  DS4_RndTextures[DS4_RndTexturesSize].W = W;
  DS4_RndTextures[DS4_RndTexturesSize].H = H;
  glGenTextures(1, &DS4_RndTextures[DS4_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, DS4_RndTextures[DS4_RndTexturesSize].TexId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  mips = log(W > H ? W : H) / log(2);
  mips = mips < 1 ? 1 : mips;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 1 ? GL_R8 : C == 3 ? GL_RGB8 : GL_RGBA8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, C == 1 ? GL_LUMINANCE : C == 3 ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, Bits);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  /*
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  */

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return DS4_RndTexturesSize++;
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
