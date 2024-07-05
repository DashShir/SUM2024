/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 25.06.2024
 * PURPOSE    : 3D animation project.
 *              Control unit sample module.
 */

#include "units.h"
#include "mth/mth.h"

typedef struct tagds4UNIT_CTRL
{
  DS4_UNIT_BASE_FIELDS;
  VEC CamLoc;           /* Camera location */
  VEC CamDir;           /* Camera direction */
  VEC CamUp;            /* Camera up direction */
  VEC CamRight;         /* Camera right direction */
  VEC CamAt;            /* Camera look at position */
  VEC CamDirWoY;         /* Camera direction without Y */
  DBL CamSpeed;         /* Camera speed */
  DBL CamAngleSpeed;    /* Camera angle speed */
} ds4UNIT_CTRL;

/* Control unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_COW *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitInit( ds4UNIT_CTRL *Uni, ds4ANIM *Ani )
{           
  Uni->CamLoc = VecSet(5, 5, 5);
  Uni->CamAt = VecSet(0, 0, 0);
  Uni->CamUp = VecSet(0, 1, 0);
  DS4_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
  Uni->CamDir = VecSet(-DS4_RndMatrView.A[0][2], -DS4_RndMatrView.A[1][2], -DS4_RndMatrView.A[2][2]);
  Uni->CamRight = VecSet(DS4_RndMatrView.A[0][0], DS4_RndMatrView.A[1][0], DS4_RndMatrView.A[2][0]);
  Uni->CamDirWoY = VecSet(Uni->CamDir.X, 0, Uni->CamDir.Z);
  Uni->CamSpeed = 2;
  Uni->CamAngleSpeed = 30;
} /* End of 'DS4_UnitInit' function */
                           
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitClose( ds4UNIT_CTRL *Uni, ds4ANIM *Ani )
{
} /* End of 'DS4_UnitClose' function */

/* Control unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_CTRL *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitResponse( ds4UNIT_CTRL *Uni, ds4ANIM *Ani )
{ 
  DBL CamDirY = Uni->CamDirWoY.Y = 0;
  DBL cos_alpha, sin_alpha, l, rep, X, Y, Z, w, h;
  VEC dv, dv1, dvc;

  /* Move camera straight-back */
  dv = VecAddVec(VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys['W'] - Ani->Keys['S'])),
                 VecMulNum(Uni->CamRight, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys['D'] - Ani->Keys['A'])));
  dvc = VecSet(dv.X, 0, dv.Z);
  dv1 = VecAddVec(dvc, VecMulNum(Uni->CamUp, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys[VK_SHIFT] - Ani->Keys[VK_CONTROL])));
  Uni->CamLoc = VecAddVec(Uni->CamLoc, dv1);
  Uni->CamAt = VecAddVec(Uni->CamAt, dv1);
  
  /* Move camera left-right */
  /*
  w = (Uni->CamAt.Z - Uni->CamLoc.Z);
  h = (Uni->CamAt.X - Uni->CamLoc.X);
  l = sqrt(w * w + h * h);
  cos_alpha = w / l;
  sin_alpha = h / l;

  rep = (Ani->Keys['A'] - Ani->Keys['D']) * Ani->DeltaTime * Uni->CamSpeed;
  X = Uni->CamLoc.X - cos_alpha * rep;
  Z = Uni->CamLoc.Z - (-sin_alpha) * rep;
  Uni->CamLoc = VecSet(X, 1, Z);
  
  Uni->CamAt = VecMulMatr(Uni->CamLoc,
      MatrTranslate(VecSet(X, 1, Z)));
  
  Uni->CamLoc = 
    VecAddVec(Uni->CamLoc, 
      VecMulNum(Uni->CamDirWoY, Ani->DeltaTime * Uni->CamSpeed *
        (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
  Uni->CamDir = 
    VecAddVec(Uni->CamDirWoY, 
      VecMulNum(Uni->CamDirWoY, Ani->DeltaTime * Uni->CamSpeed *
        (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
  */
  
  /* Turn camera by Y */
  
  Uni->CamAt =
    PointTransform(Uni->CamAt,
      MatrMulMatr3(MatrTranslate(VecNeg(Uni->CamLoc)),
                  MatrRotateY((Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]) * Ani->DeltaTime * Uni->CamAngleSpeed),
                  MatrTranslate(Uni->CamLoc))); 
  
  Uni->CamAt =
    PointTransform(Uni->CamAt,
      MatrMulMatr3(MatrTranslate(VecNeg(Uni->CamLoc)),
                  MatrRotateX((Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->DeltaTime * Uni->CamAngleSpeed),
                  MatrTranslate(Uni->CamLoc))); 
    
  /* PointTransform(Uni->CamAt,
        MatrMulMatr3(MatrTranslate(VecNeg(Uni->CamLoc)),
                    MatrRotateX((Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) * Ani->DeltaTime * Uni->CamAngleSpeed),
                    MatrTranslate(Uni->CamLoc)))) */ 
  /* Move camera up */
  /*
  Uni->CamLoc = 
    VecAddVec(Uni->CamLoc, 
      VecMulNum(Uni->CamUp, Ani->DeltaTime * Uni->CamSpeed *
        (Ani->Keys[VK_SHIFT])));
  */

  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
                   MatrRotateY((Ani->Keys[VK_LBUTTON] - Ani->Keys[VK_RBUTTON]) *
                               Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdx / 200));
  
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
                   MatrRotateX((Ani->Keys[VK_LBUTTON] - Ani->Keys[VK_RBUTTON]) *
                               Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdy / 200));
  
         
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
              VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->CamSpeed * Ani->Mdz / 60));
  
  /*
  Uni->CamLoc = 
      VecAddVec(Uni->CamLoc, 
        VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->CamSpeed *
          (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
  */
  DS4_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
  Uni->CamDir = VecSet(-DS4_RndMatrView.A[0][2], -DS4_RndMatrView.A[1][2], -DS4_RndMatrView.A[2][2]);
  Uni->CamRight = VecSet(DS4_RndMatrView.A[0][0], DS4_RndMatrView.A[1][0], DS4_RndMatrView.A[2][0]);
  Uni->CamDirWoY = VecSet(Uni->CamDir.X, CamDirY, Uni->CamDir.Z);
} /* End of 'DS4_UnitResponse' function */

static VOID DS4_UnitRender(ds4UNIT_CTRL *Uni, ds4ANIM *Ani)
{
} /* End of 'DS4_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC DrawPos;
 * RETURNS: None.
 */
ds4UNIT * DS4_UnitCreateCtrl( VOID )
{
  ds4UNIT_CTRL *Uni;
 
  /* Memory allocation */
  if ((Uni = (ds4UNIT_CTRL *)DS4_AnimUnitCreate(sizeof(ds4UNIT_CTRL))) == NULL)
    return NULL;
  
  /* Setup unit methods */
  Uni->Init = (VOID *)DS4_UnitInit;
  Uni->Close = (VOID *)DS4_UnitClose;
  Uni->Response = (VOID *)DS4_UnitResponse;
  Uni->Render = (VOID *)DS4_UnitRender;
  return (ds4UNIT *)Uni;
} /* End of 'DS4_UnitCreateCtrl' function */

/* END OF 'u_ctrl.c' FILE */