/* FILE NAME  : u_mount.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Mountain unit sample module.
 */

#include "units.h"

typedef struct tagds4UNIT_MOUNT
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIM Land;          /* Land model */
} ds4UNIT_MOUNT;

/* Mountain unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_MOUNT *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_MOUNT *Uni, ds4ANIM *Ani )
{
  HBITMAP hBm;

  if ((hBm = LoadImage(NULL, "bin/textures/hf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    ds4GRID G;
    BITMAP bm;
    BYTE *Bits;
    FLT wt = 100, ht = 100, h, max_height = 10;
    INT x, y;
 
    GetObject(hBm, sizeof(bm), &bm);
    Bits = (BYTE *)bm.bmBits;
    if (DS4_RndGridCreate(&G, bm.bmWidth, bm.bmHeight))
    {
      for (y = 0; y < G.H; y++)
        for (x = 0; x < G.W; x++)
        {
          BYTE *rgb =  &Bits[(G.H - 1 - y) * bm.bmWidthBytes + x * bm.bmBitsPixel / 8];

          h = max_height * rgb[0] / 255.0;
          G.V[y * G.W + x].P = Vec3Set(-wt / 2 + x * wt / bm.bmWidth,
                                       h,
                                       ht / 2 - y * ht / bm.bmHeight);
        }
      DeleteObject(hBm);
      DS4_RndGridAutoNormals(&G);

      if ((hBm = LoadImage(NULL, "bin/textures/hfcolor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
      {
        BITMAP bm;
        BYTE *Bits;
        INT x, y, w, h;
     
        GetObject(hBm, sizeof(bm), &bm);
        Bits = (BYTE *)bm.bmBits;
        w = bm.bmWidth;
        h = bm.bmHeight;
        if (G.W < w)
          w = G.W;
        if (G.H < h)
          h = G.H;
        for (y = 0; y < h; y++)
          for (x = 0; x < w; x++)
          {
            BYTE *rgb =  &Bits[(bm.bmHeight - 1 - y) * bm.bmWidthBytes + x * bm.bmBitsPixel / 8];

            G.V[y * G.W + x].C = Vec4Set(rgb[2] / 255.0 - 0.45,
                                         rgb[1] / 255.0 - 0.2,
                                         rgb[0] / 255.0,
                                         1);
          }
        DeleteObject(hBm);
      }

      DS4_RndPrimFromGrid(&Uni->Land, &G);
      DS4_RndGridFree(&G);
    }
  }
} /* End of 'DS4_UnitInit' function */

/* Mountain unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_MOUNT *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_MOUNT *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimFree(&Uni->Land);
} /* End of 'DS4_UnitClose' function */

/* Mountain unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_MOUNT *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_MOUNT *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitResponse' function */

/* Mountain unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_MOUNT * Uni, ds4ANIM *Ani)
{
  DS4_RndPrimDraw(&Uni->Land, MatrRotateY(0));
} /* End of 'DS4_UnitRender' function */

/* Mountain unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateMountain( VOID )
{
  ds4UNIT_MOUNT *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_MOUNT *)DS4_AnimUnitCreate(sizeof(ds4UNIT_MOUNT))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateFloor' function */

/* END OF 'u_mount.c' FILE */
