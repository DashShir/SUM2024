/* FILE NAME  : u_ctrl.c
 * PROGRAMMER : DS4
 * LAST UPDATE: 04.07.2024
 * PURPOSE    : 3D animation project.
 *              Control unit sample module.
 */

#include <stdio.h>

#include "units.h"
#include "mth/mth.h"

typedef struct tagds4UNIT_CTRL
{
  DS4_UNIT_BASE_FIELDS;
  VEC3 CamSaveLoc;       /* Camera location to save */
  VEC3 CamSaveDir;       /* Camera diretcion to save */
  VEC3 CamSaveUp;        /* Camera up direction to save */
  VEC3 CamSaveRight;     /* Camera right direction to save */
  VEC3 CamSaveAt;        /* Camera look at position to save */
  
  VEC3 CamHomeLoc;       /* Camera location at home */
  VEC3 CamHomeDir;       /* Camera diretcion at home */
  VEC3 CamHomeUp;        /* Camera up direction at home */
  VEC3 CamHomeRight;     /* Camera right direction at home */
  VEC3 CamHomeAt;        /* Camera look at position at home */
  
  VEC3 CamLoc;           /* Camera location */
  VEC3 CamDir;           /* Camera direction */
  VEC3 CamUp;            /* Camera up direction */
  VEC3 CamRight;         /* Camera right direction */
  VEC3 CamAt;            /* Camera look at position */
  VEC3 CamDirWoY;        /* Camera direction without Y */
  FLT CamSpeed;         /* Camera speed */
  FLT CamAngleSpeed;    /* Camera angle speed */

  BOOL CamSpeedMode;          /* Camera speed mode */
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
  Uni->CamLoc = Uni->CamSaveLoc = Uni->CamHomeLoc = Vec3Set(5, 5, 5);
  Uni->CamAt = Uni->CamSaveAt = Uni->CamHomeAt = Vec3Set(0, 0, 0);
  Uni->CamUp = Uni->CamSaveUp = Uni->CamHomeUp = Vec3Set(0, 1, 0);
  DS4_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
  Uni->CamDir = Uni->CamSaveDir = Uni->CamHomeDir = Vec3Set(-DS4_RndMatrView.A[0][2], -DS4_RndMatrView.A[1][2], -DS4_RndMatrView.A[2][2]);
  Uni->CamRight = Uni->CamSaveRight = Uni->CamHomeRight = Vec3Set(DS4_RndMatrView.A[0][0], DS4_RndMatrView.A[1][0], DS4_RndMatrView.A[2][0]);
  Uni->CamDirWoY = Vec3Set(Uni->CamDir.X, 0, Uni->CamDir.Z);
  Uni->CamSpeed = 2;
  Uni->CamAngleSpeed = 30;
  Uni->CamSpeedMode = FALSE;
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
  FLT CamDirY = Uni->CamDirWoY.Y = 0;
  FLT Dist, cosT, cosP, sinP, sinT, plen, Azimuth, Elevator, Wp, Hp, sx, sy;
  VEC3 dv, dv1, dvc, dv2 /*, NewLoc */;

  if (Ani->Keys[VK_LSHIFT] && (Ani->KeysClick['W'] || Ani->Keys['W']))
  {
    INT modes[2];

    glGetIntegerv(GL_POLYGON_MODE, modes);
    if (Ani->KeysClick['W'])
      glPolygonMode(GL_FRONT_AND_BACK, modes[0] == GL_FILL ? GL_LINE : GL_FILL);
    return;
  }


  if (Ani->Keys['C'])
  {
    Uni->CamSaveAt = Uni->CamAt;
    Uni->CamSaveDir = Uni->CamDir;
    Uni->CamSaveLoc = Uni->CamLoc;
    Uni->CamSaveUp = Uni->CamUp;
    Uni->CamSaveRight = Uni->CamRight;
  }

  if (Ani->Keys['H'])
  {
    Uni->CamAt = Uni->CamSaveAt;
    Uni->CamDir = Uni->CamSaveDir;
    Uni->CamLoc = Uni->CamSaveLoc;
    Uni->CamUp = Uni->CamSaveUp;
    Uni->CamRight = Uni->CamSaveRight;
  }
 
  if (Ani->Keys['F'] && Ani->Keys[VK_LCONTROL])
  {
    Uni->CamAt = Uni->CamHomeAt;
    Uni->CamDir = Uni->CamHomeDir;
    Uni->CamLoc = Uni->CamHomeLoc;
    Uni->CamUp = Uni->CamHomeUp;
    Uni->CamRight = Uni->CamHomeRight;
  }
 
  if (Ani->Keys['F'] && Ani->Keys['W'] && !Ani->Keys[VK_SHIFT])
  {
    Uni->CamSpeedMode = !Uni->CamSpeedMode;
    if (Uni->CamSpeedMode)
    {
      Uni->CamAngleSpeed += 45;
      Uni->CamSpeed += 28;
    }
    else
    {
      Uni->CamAngleSpeed -= 45;
      Uni->CamSpeed -= 28;
    }
  }
 
  if (Ani->Keys[VK_LCONTROL])
  {

    /* Move camera straight-back */
    dv = VecAddVec(Vec3MulNum(DS4_RndCamDir, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys['W'] - Ani->Keys['S'])),
                   Vec3MulNum(DS4_RndCamRight, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys['D'] - Ani->Keys['A'])));
    dvc = Vec3Set(dv.X, 0, dv.Z);
    dv1 = VecAddVec(dvc, Vec3MulNum(DS4_RndCamUp, Ani->DeltaTime * Uni->CamSpeed * (Ani->Keys[VK_RSHIFT] - Ani->Keys[VK_RCONTROL])));
    dv2 = Vec3MulNum(DS4_RndCamUp, Ani->DeltaTime * Uni->CamAngleSpeed * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) / 8);
    DS4_RndCamLoc = Uni->CamLoc = VecAddVec(DS4_RndCamLoc, dv1);
    DS4_RndCamAt = Uni->CamAt = VecAddVec(DS4_RndCamAt, dv1);
    DS4_RndCamAt = Uni->CamAt = VecAddVec(DS4_RndCamAt, dv2);

      
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
    Uni->CamLoc = Vec3Set(X, 1, Z);
    
    Uni->CamAt = VecMulMatr(Uni->CamLoc, MatrTranslate(Vec3Set(X, 1, Z)));
    
    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc, 
        Vec3MulNum(Uni->CamDirWoY, Ani->DeltaTime * Uni->CamSpeed *
          (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
    Uni->CamDir = 
      VecAddVec(Uni->CamDirWoY, 
        Vec3MulNum(Uni->CamDirWoY, Ani->DeltaTime * Uni->CamSpeed *
          (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
    */
    
    /* Turn camera by Y */
    
    /* Uni->CamAt = Vec3MulNum(Uni->CamRight, Ani->DeltaTime * Uni->CamAngleSpeed * (Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) / 10); */

    /*
    Uni->CamAt = 
      PointTransform(Uni->CamAt,
        MatrMulMatr3(MatrTranslate(VecNeg(Uni->CamLoc)),
                     MatrRotate((Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) * Ani->DeltaTime * Uni->CamAngleSpeed, Vec3Set(0, Uni->CamDir.Y, 0)),
                     MatrTranslate(Uni->CamLoc))); 
    */
     
    /*
    DS4_RndCamAt = Uni->CamAt = VecAddVec(DS4_RndCamAt, Vec3MulNum(DS4_RndCamRight, Ani->DeltaTime * Uni->CamAngleSpeed * (Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]) / 8));
    */

    /*
    Uni->CamAt =
      PointTransform(Uni->CamAt,
        MatrMulMatr3(MatrTranslate(VecNeg(Uni->CamLoc)),
          MatrRotate((Ani->Keys[VK_DOWN] - Ani->Keys[VK_UP]) * Ani->DeltaTime * Uni->CamAngleSpeed, Vec3Set(Uni->CamDir.X, 0, 0)),
                    MatrTranslate(Uni->CamLoc))); 
    */
    /* Move camera up */
    /*
    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc, 
        Vec3MulNum(Uni->CamUp, Ani->DeltaTime * Uni->CamSpeed *
          (Ani->Keys[VK_SHIFT])));
    */
    /*
    Uni->CamLoc =
      PointTransform(Uni->CamLoc,
                     MatrMulMatr3(
                       MatrTranslate(VecNeg(Uni->CamLoc)), 
                       MatrRotateY(Ani->Keys[VK_LBUTTON] *
                         Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdx / 200), 
                       MatrTranslate(Uni->CamLoc)));
    */
    
    /*
    Uni->CamLoc = 
      PointTransform(Uni->CamLoc, 
      MatrTranslate(VecMulMatr(Vec3MulNum(Uni->CamLoc, Ani->Keys[VK_LBUTTON] * Ani->DeltaTime * Uni->CamSpeed * Ani->Mdx / 200), MatrRotateY(15))));
    */
    /*
    Uni->CamLoc = 
      VecAddVec(Uni->CamLoc, 
      VecMulMatr(Vec3MulNum(Uni->CamLoc, Ani->Keys[VK_LBUTTON] * Ani->DeltaTime * Uni->CamSpeed * Ani->Mdx / 200), MatrRotateY(15)));

    */
    /*
    Uni->CamLoc =
      PointTransform(Uni->CamLoc,
                     MatrRotate(Ani->Keys[VK_LBUTTON] *
                     Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdx / 200, Vec3Set(0, Uni->CamDir.Y, 0)));
    
    Uni->CamLoc =
      PointTransform(Uni->CamLoc,
                     MatrRotate(Ani->Keys[VK_RBUTTON] *
                     Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdy / 200, Vec3Set(Uni->CamDir.X, 0, 0)));
    Uni->CamLoc =
      PointTransform(Uni->CamLoc,
                     MatrRotate(Ani->Keys[VK_MBUTTON] *
                     Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdx / 200, Vec3Set(0, 0, Uni->CamDir.Z)));
    */
    
    if (Ani->Mdx > 30)
      dv.X = 30;
    
    dv = VecSubVec(DS4_RndCamAt, DS4_RndCamLoc);
    Dist = sqrt(VecDotVec(dv, dv));
   
    cosT = (DS4_RndCamLoc.Y - DS4_RndCamAt.Y) / Dist;
    sinT = sqrt(1 - cosT * cosT);

    plen = Dist * sinT;
     
    cosP = (DS4_RndCamLoc.Z - DS4_RndCamAt.Z) / plen;
    sinP = (DS4_RndCamLoc.X - DS4_RndCamAt.X) / plen;
     
    Azimuth = atan2(sinP, cosP) * 180 / PI;
    Elevator = atan2(sinT, cosT) * 180 / PI;

    Azimuth += Ani->DeltaTime *
      (21 * -30 * 1 * Ani->Keys[VK_LBUTTON] * Ani->Mdx * (1 + Ani->Keys[VK_SHIFT] * 30) +
       47 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));

    Elevator += Ani->DeltaTime *
      (21 * -30 * 1 * Ani->Keys[VK_LBUTTON] * Ani->Mdy * (1 + Ani->Keys[VK_SHIFT] * 30) +
       47 * 1 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));
     
    Dist -= Ani->DeltaTime *
      (20 * 1 * Ani->Mdz * (1 + Ani->Keys[VK_SHIFT] * 30) +
       20 * 8 * 1 * (1 + Ani->Keys[VK_SHIFT] * 30) *
          (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR])); 

    if (Elevator < 1)
      Elevator = 1;
    if (Elevator > 179)
      Elevator = 179;
    if (Dist < 0.1)
      Dist = 0.1;

    /*
    NewLoc = PointTransform(Vec3Set(0, Dist, 0),
                            MatrMulMatr(MatrRotateX(Elevator),
                                        MatrRotateY(Azimuth)));
    DS4_RndCamLoc = VecAddVec(NewLoc, DS4_RndCamAt);
    */

    Wp = DS4_RndProjSize;
    Hp = DS4_RndProjSize;
     
    if (Ani->W > Ani->H)
      Wp *= (FLT)Ani->W / Ani->H;
    else
      Hp *= (FLT)Ani->H / Ani->W;

    if (Ani->Keys[VK_RBUTTON])
      sx = 30;
    sx = (1 + Ani->Keys[VK_SHIFT] * 30) * Ani->Keys[VK_RBUTTON] * -Ani->Mdx * Wp / Ani->W * Dist / DS4_RndProjDist;
    sy = (1 + Ani->Keys[VK_SHIFT] * 30) * Ani->Keys[VK_RBUTTON] * Ani->Mdy * Hp / Ani->H * Dist / DS4_RndProjDist;
    
    dv = VecAddVec(Vec3MulNum(DS4_RndCamRight, sx),
                   Vec3MulNum(DS4_RndCamUp, sy));
    DS4_RndCamAt = VecAddVec(DS4_RndCamAt, dv);
    DS4_RndCamLoc = VecAddVec(DS4_RndCamLoc, dv);

    DS4_RndCamSet(PointTransform(Vec3Set(0, Dist, 0),
                               MatrMulMatr3(MatrRotateX(Elevator),
                                            MatrRotateY(Azimuth),
                                            MatrTranslate(DS4_RndCamAt))),
                  DS4_RndCamAt,
                  Vec3Set(0, 1, 0));
   
    /*
    Uni->CamLoc =
      PointTransform(Uni->CamLoc,
                     MatrRotateX(Ani->Keys[VK_LBUTTON] *
                                 Ani->DeltaTime * Uni->CamAngleSpeed * Ani->Mdy / 200));
    
    Uni->CamLoc =
      VecAddVec(Uni->CamLoc,
                Vec3MulNum(Uni->CamDir, Ani->DeltaTime * Uni->CamSpeed * Ani->Mdz / 30));
    */       

    /*
    Uni->CamLoc = 
        VecAddVec(Uni->CamLoc, 
          Vec3MulNum(Uni->CamDir, Ani->DeltaTime * Uni->CamSpeed *
            (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT])));
    */
    /*
    DS4_RndCamLoc = Uni->CamLoc;
    DS4_RndCamAt = Uni->CamAt;
    DS4_RndCamUp = Uni->CamUp;
    DS4_RndCamSet( Uni->CamLoc, Uni->CamAt, Uni->CamUp );
    */
  }
  Uni->CamDir = Vec3Set(-DS4_RndMatrView.A[0][2], -DS4_RndMatrView.A[1][2], -DS4_RndMatrView.A[2][2]);
  Uni->CamRight = Vec3Set(DS4_RndMatrView.A[0][0], DS4_RndMatrView.A[1][0], DS4_RndMatrView.A[2][0]);
  Uni->CamDirWoY = Vec3Set(Uni->CamDir.X, CamDirY, Uni->CamDir.Z);
} /* End of 'DS4_UnitResponse' function */

/* Control unit rendering handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ds4UNIT_TEXTURE *Uni;
 *   - animation context:
 *       ds4ANIM *Ani;
 * RETURNS: None.
 */
static VOID DS4_UnitRender(ds4UNIT_CTRL *Uni, ds4ANIM *Ani)
{
  static FLT OldTime = 0;

  if (DS4_Anim.GlobalTime - OldTime > 3)
  {
    static CHAR Buf[100];

    sprintf(Buf, "DS4 Anim. FPS: %.3f\tCoordinates: X:%f, Y:%f, Z:%f", DS4_Anim.FPS, Uni->CamLoc.X, Uni->CamLoc.Y, Uni->CamLoc.Z);
    SetWindowText(DS4_Anim.hWnd, Buf);
    OldTime = DS4_Anim.GlobalTime;
  }

} /* End of 'DS4_UnitRender' function */

/* Control unit creation function.
 * ARGUMENTS: None.
 *   - object position:
 *       VEC3 DrawPos;
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