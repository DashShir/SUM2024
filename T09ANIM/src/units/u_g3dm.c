/* FILE NAME  : u_g3dm.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 08.07.2024
 * PURPOSE    : 3D animation project.
 *              Control unit sample module.
 */

#include <stdio.h>

#include "units.h"

typedef struct tagds4UNIT_G3DM
{
  DS4_UNIT_BASE_FIELDS;
  ds4PRIMS G3DM, G3DM1, G3DM2;                      /* G3DM models */
  INT G3DM2NOE7, G3DM2NOE8;                         /* G3DM2 noum of elements in 7th and 8th primitives*/
  BOOL IsG3DM2FireOn;                               /* True if fire in G3DM2 on, False otherwise */
  FLT G3DM2Trans, G3DM2TransOld, G3DM2DeltaTrans;   /* G3DM2 translation values */
} ds4UNIT_G3DM;

/* Floor unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_G3DM *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_G3DM *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimsLoad(&Uni->G3DM, "bin/models/GAZ66.g3dm");
  Uni->G3DM.Trans = MatrRotateX(-90);
  DS4_RndPrimsLoad(&Uni->G3DM1, "bin/models/btr.g3dm");
  Uni->G3DM1.Trans = MatrRotateX(0);
  DS4_RndPrimsLoad(&Uni->G3DM2, "bin/models/Fighter1.g3dm");
  Uni->G3DM2.Trans = MatrRotateX(0);
  Uni->IsG3DM2FireOn = 0;
  Uni->G3DM2NOE7 = Uni->G3DM2.Prims[7].NumOfElements;
  Uni->G3DM2NOE8 = Uni->G3DM2.Prims[8].NumOfElements;
  //Uni->G3DM2Trans = 0;
} /* End of 'DS4_UnitInit' function */

/* Floor unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_G3DM *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_G3DM *Uni, ds4ANIM *Ani )
{
  DS4_RndPrimsFree(&Uni->G3DM);
  DS4_RndPrimsFree(&Uni->G3DM1);
  DS4_RndPrimsFree(&Uni->G3DM2);
} /* End of 'DS4_UnitClose' function */

/* Floor unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_G3DM *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_G3DM *Uni, ds4ANIM *Ani )
{
  INT
    i,
    wheelsBTR[] =
    {
      10, 12, 14, 16, 18, 19, 21, 23,
      5, 11, 13, 15, 17, 20, 22, 24
    },
    wheelsGazBack[] =
    {
      17, 18, 21, 22
    },
    wheelsGazFront[] =
    {
      19, 20, 15, 16
    },
    topBTR[] = 
    {
      6
    };
  INT fighterFire[] = 
    {
      1, 1, 1
    };
  
  Uni->G3DM2Trans = sin(Ani->GlobalTime) * 21;

  /* Wheels BTR rotate */
  for (i = 0; i < sizeof(wheelsBTR) / sizeof(wheelsBTR[0]); i++)
  {
    ds4PRIM *pr = &Uni->G3DM1.Prims[wheelsBTR[i]];
    VEC3 Center = VecDivNum(VecAddVec(pr->MaxBB, pr->MinBB), 2);

    pr->Trans =
      MatrMulMatr3(MatrTranslate(VecNeg(Center)),
                   MatrRotateX(Ani->Time * 90),
                   MatrTranslate(Center));
  }

  /* Top BTR rotate */
  for (i = 0; i < sizeof(topBTR) / sizeof(topBTR[0]); i++)
  {
    ds4PRIM *pr = &Uni->G3DM1.Prims[topBTR[i]];
    VEC3 Center = VecDivNum(VecAddVec(pr->MaxBB, pr->MinBB), 2);

    pr->Trans =
      MatrMulMatr3(MatrTranslate(VecNeg(Center)),
                   MatrRotateY(Ani->Time * 40),
                   MatrTranslate(Center));

    pr = &Uni->G3DM1.Prims[7];
    pr->Trans =
      MatrMulMatr4(MatrTranslate(VecNeg(Center)),
                   MatrRotateX(-(10 + 20 * sin(Ani->Time))),
                   MatrRotateY(Ani->Time * 40),
                   MatrTranslate(Center));
  }

  /* Wheels Gaz66 rotate */
  for (i = 0; i < sizeof(wheelsGazBack) / sizeof(wheelsGazBack[0]); i++)
  {
    ds4PRIM *pr = &Uni->G3DM.Prims[wheelsGazBack[i]];
    VEC3 Center = VecDivNum(VecAddVec(pr->MaxBB, pr->MinBB), 2);

    pr->Trans =
      MatrMulMatr3(MatrTranslate(VecNeg(Center)),
                   MatrRotateX(Ani->Time * 90),
                   MatrTranslate(Center));
  }

  for (i = 0; i < sizeof(wheelsGazFront) / sizeof(wheelsGazFront[0]); i++)
  {
    ds4PRIM *pr = &Uni->G3DM.Prims[wheelsGazFront[i]];
    VEC3 Center = VecDivNum(VecAddVec(pr->MaxBB, pr->MinBB), 2);

    pr->Trans =
      MatrMulMatr4(MatrTranslate(VecNeg(Center)),
                   MatrRotateX(Ani->Time * 90),
                   MatrRotateZ(sin(Ani->Time) * 20),
                   MatrTranslate(Center));
  }

  /* Fighter animation */
  Uni->G3DM2.Prims[3].NumOfElements = 0;
  Uni->G3DM2.Prims[4].NumOfElements = 0;
  for (i = 0; i < Uni->G3DM2.NumOfPrims; i++)
  {
    ds4PRIM *pr = &Uni->G3DM2.Prims[i];
    Uni->G3DM2TransOld = Uni->G3DM2Trans;
    Uni->G3DM2Trans = sin(Ani->GlobalTime) * 21;
    Uni->G3DM2DeltaTrans = Uni->G3DM2Trans - Uni->G3DM2TransOld;
    
    if (Uni->G3DM2DeltaTrans < 0 && Uni->IsG3DM2FireOn == 1)
    {
      Uni->IsG3DM2FireOn = 0;
      Uni->G3DM2.Prims[7].NumOfElements = 0;
      Uni->G3DM2.Prims[8].NumOfElements = 0;
    }
    else if (Uni->G3DM2DeltaTrans >= 0 && Uni->IsG3DM2FireOn == 0)
    {
      Uni->IsG3DM2FireOn = 1;
      Uni->G3DM2.Prims[7].NumOfElements = Uni->G3DM2NOE7;
      Uni->G3DM2.Prims[8].NumOfElements = Uni->G3DM2NOE8;
    }
    pr->Trans = MatrTranslate(Vec3Set(Uni->G3DM2Trans, 0, 0));
  }
  /*
  ds4PRIM_TYPE Type;  
  INT
    VA,               
    VBuf,             
    IBuf;             
  INT NumOfElements;

  MATR Trans;       
  VEC3 MinBB, MaxBB;

  INT MtlNo;   
  */
} /* End of 'DS4_UnitResponse' function */

/* G3DM unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_G3DM * Uni, ds4ANIM *Ani)
{
  static CHAR Buf[1000];

  DS4_RndPrimsDraw(&Uni->G3DM, MatrIdentity());
  DS4_RndPrimsDraw(&Uni->G3DM1, MatrMulMatr(MatrScale(Vec3Set1(0.03)), MatrTranslate(Vec3Set(10, 0, 0))));
  DS4_RndPrimsDraw(&Uni->G3DM2, MatrMulMatr3(MatrScale(Vec3Set1(1)), MatrRotateY(90), MatrTranslate(Vec3Set(20, 0, 0 /* sin(Ani->GlobalTime) * 21 */))));

  /* Text output */
  sprintf(Buf, "Tr: %d\nOldTr: %d\nDelTr: %d", Uni->G3DM2Trans, Uni->G3DM2TransOld, Uni->G3DM2DeltaTrans);
  DS4_RndFntDraw(Buf, Vec3Set(0, -20, 0), 30, Vec3Set(1, 1, 0));
} /* End of 'DS4_UnitRender' function */

/* Floor unit creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4UNIT *) pointer to created unit.
 */
ds4UNIT * DS4_UnitCreateG3DM( VOID )
{
  ds4UNIT_G3DM *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_G3DM *)DS4_AnimUnitCreate(sizeof(ds4UNIT_G3DM))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  /*
  Uni->G3DM2NOE7 = Uni->G3DM2.Prims[7].NumOfElements;
  Uni->G3DM2NOE8 = Uni->G3DM2.Prims[8].NumOfElements;
  Uni->IsG3DM2FireOn = 0;
  */
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateG3DM' function */

/* END OF 'u_g3dm.c' FILE */
