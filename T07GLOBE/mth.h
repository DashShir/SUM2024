/* FILE NAME: mth.h
 * PROGRAMMER: DS4
 * DATE: 20.06.2024
 * PURPOSE: 3D math implementation module.
 */
 
#ifndef __mth_h_
#define __mth_h_
 
#include <math.h>
 
/* Pi math constant */
#define PI 3.14159265358979323846264338327950288419716939937510582097494459
 
/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
 
/* Base float point types */
typedef double DBL;
typedef double FLT;
 
/* Space vector/point representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct
{
  DBL A[4][4];
} MATR;

static MATR UnitMatrix = 
{
  { 
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/***
 * Vector handle functions
 ***/

/* Setting vector function.
 * ARGUMENTS:
 *   - coordinates:
 *       DBL X, Y, Z;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v = {X, Y, Z};
 
  return v;
} /* End of 'VecSet' function */

/* Setting vector by one component function.
 * ARGUMENTS:
 *   - coordinates value:
 *       DBL X;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecSet1( DBL X )
{
  VEC v = {X, X, X};
 
  return v;
} /* End of 'VecSet1' function */

/* Dot multiply two vectors function.
 * ARGUMENTS:
 *   - vectors to be dot multiplied:
 *       VEC V1, V2;
 * RETURNS:
 *   (DBL) result.
 */
_inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec function' */


/* Cross multiply two vectors function.
 * ARGUMENTS:
 *   - vectors to be cross multiplied:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
_inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

/* Substracting two vectors function.
 * ARGUMENTS:
 *   - vectors to be substacted:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
_inline VEC VecSubVec( VEC V1, VEC V2 )       /* V1 - V2 */
{                         
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}  /* End of 'VecSubVec' function */

/* Multiply one vector and number function.
 * ARGUMENTS:
 *   - vector to be multiplied:
 *       VEC V1;
 *   - number:
 *       DBL N
 * RETURNS:
 *   (VEC) result vector.
 */
_inline VEC VecMulNum( VEC V1, DBL N )        /* V1 * N */
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

/* Division vector and number function.
 * ARGUMENTS:
 *   - vector to be divised:
 *       VEC V1;
 *   - number:
 *       N;
 * RETURNS:
 *   (VEC) result vector.
 */
_inline VEC VecDivNum( VEC V1, DBL N )        /* V1 / N */
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

/* Negative vector function.
 * ARGUMENTS:
 *   - vector to be negatived:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
_inline VEC VecNeg( VEC V )                   /* -V */
 {
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* Vector normalizing function.
 * ARGUMENTS:
 *   - vector to be normalized:
 *       VEC V;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecNormalize( VEC V )            /* V / |V| */
{
  DBL len = VecDotVec(V, V);
 
  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
} /* End of 'VecNormalize' function */

/* Add two vectors f  unction.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* Transform point function.
 * ARGUMENTS:
 *   - vector to be transformed:
 *       VEC V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC PointTransform( VEC V, MATR M ) /* V * M4x3 */
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' function */

/* Transform vector function.
 * ARGUMENTS:
 *   - vector to be transformed:
 *       VEC V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VectorTransform( VEC V, MATR M ) /* V * M3x3 */
{
    VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
           V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
           V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' function */

/* Full transform point with homogenius factor function.
 * ARGUMENTS:
 *   - vector to be transformed:
 *       VEC V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecMulMatr( VEC V, MATR M ) /* V * M4x3 */
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/***
 * Matrix handle functions
 ***/

/* Set matrix function.
 * ARGUMENTS:
 *   - Matrix elements:
 *       DBL A00, A01, A02, A03, A10, A11, A12, A13, A20, A21, A22, A23, A30, A31, A32, A33;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrSet( DBL A00, DBL A01, DBL A02, DBL A03,
                       DBL A10, DBL A11, DBL A12, DBL A13,
                       DBL A20, DBL A21, DBL A22, DBL A23,
                       DBL A30, DBL A31, DBL A32, DBL A33 )
{
  MATR r =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33}
    }
  };
 
  return r;
} /* End of 'MatrSet' function */

/* Multiply two matrixes function.
 * ARGUMENTS:
 *   - matrixes to be multiplied:
 *       MATR M1, M2;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  int i, j, k;
  MATR r = {{{0}}};
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' function */

/* Helpful nultiplication macro functions */
#define MatrMulMatr3(A, B, C) MatrMulMatr(A, MatrMulMatr(B, C))
#define MatrMulMatr4(A, B, C, D) MatrMulMatr(MatrMulMatr(A, B), MatrMulMatr(C, D))
#define MatrMulMatr5(A, B, C, D, E) MatrMulMatr(MatrMulMatr(MatrMulMatr(A, B), MatrMulMatr(C, D)), E)
#define MatrMulMatr6(A, B, C, D, E, F) MatrMulMatr(MatrMulMatr3(A, B, C), MatrMulMatr3(D, E, F))
#define MatrMulMatr7(A, B, C, D, E, F, G) MatrMulMatr(MatrMulMatr6(A, B, C, D, E, F), G)
#define MatrMulMatr8(A, B, C, D, E, F, G, H) MatrMulMatr(MatrMulMatr4(A, B, C, D), MatrMulMatr4(E, F, G, H))
#define MatrMulMatr9(A, B, C, D, E, F, G, H, I) MatrMulMatr3(MatrMulMatr3(A, B, C), MatrMulMatr3(D, E, F), MatrMulMatr3(G, H, I))

/* Transpose matrix function.
 * ARGUMENTS:
 *   - matrix to be transposed:
 *       MATR M;
 * RETURNS:
 *   (MATR) result transposed matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  int i, j;
  MATR r = {{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

/* Set matrix function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  return MatrSet(1, 0, 0, 0, 
                 0, 1, 0, 0, 
                 0, 0, 1, 0, 
                 0, 0, 0, 1);
} /* End 'MatrIdentity' function */

/* Set matrix to translate transform function.
 * ARGUMENTS:
 *   - translating vector:
 *       VEC D;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC D )
{
  MATR r =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {D.X, D.Y, D.Z, 1},
    }
  };
 
  return r;
} /* End of 'MatrTranslate' function */

/* Set matrix to rotate around Z axis transform function.
 * ARGUMENTS:
 *   - rotation vector in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL Angle = D2R(AngleInDegree), si = sin(Angle), co = cos(Angle);
  MATR r =
  {
    {
      {co, si, 0, 0},
      {-si, co, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
    }
  };
 
  return r;
} /* End of 'MatrRotateZ' function */

/* Set matrix to rotate around X axis transform function.
 * ARGUMENTS:
 *   - rotation vector in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL Angle = D2R(AngleInDegree), si = sin(Angle), co = cos(Angle);
  MATR r =
  {
    {
      {1, 0, 0, 0},
      {0, co, si, 0},
      {0, -si, co, 0},
      {0, 0, 0, 1},
    }
  };
 
  return r;
} /* End of 'MatrRotateX' function */

/* Set matrix to rotate around Y axis transform function.
 * ARGUMENTS:
 *   - rotation vector in degrees:
 *       DBL AngleInDegree;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL Angle = D2R(AngleInDegree), si = sin(Angle), co = cos(Angle);
  MATR r =
  {
    {
      {co, 0, -si, 0},
      {0, 1, 0, 0},
      {si, 0, co, 0},
      {0, 0, 0, 1}
    }
  };
 
  return r;
} /* End of 'MatrRotateY' function */

/* Set matrix to rotate around arbitrary axis transform function.
 * ARGUMENTS:
 *   - rotation vector in degrees:
 *       DBL AngleInDegree;
 *   - vector to be rotated around:
 *       VEC V;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  DBL Angle = D2R(AngleInDegree), s = sin(Angle), c = cos(Angle);
  VEC A = VecNormalize(V);
  MATR r =
  {
    {
      {c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s, 0},
      {A.Y * A.X * (1 - c) - A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * s, 0},
      {A.Z * A.X * (1 - c) + A.Y * s, A.Z * A.Y * (1 - c) - A.X * s, c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1},
    }
  };
 
  return r;
} /* End of 'MatrRotate' function */

/* Count 3x3 matrix determinant function.
 * ARGUMENTS:
 *   - matrix elements:
 *       DBL A11, A12, A13, A21, A22, A23, A31, A32, A33;
 * RETURNS:
 *   (DBL) determinant.
 */
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 ) 
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
         A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* Count matrix determinant function.
 * ARGUMENTS:
 *   - matrix elements:
 *       MATR M;
 * RETURNS:
 *   (DBL) determinant.
 */
__inline DBL MatrDeterm( MATR M ) 
{
  return M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3], 
                                   M.A[2][1], M.A[2][2], M.A[2][3], 
                                   M.A[3][1], M.A[3][2], M.A[3][3]) - 
         M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3], 
                                   M.A[2][0], M.A[2][2], M.A[2][3],
                                   M.A[3][0], M.A[3][2], M.A[3][3]) +
         M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3], 
                                   M.A[2][0], M.A[2][1], M.A[2][3],
                                   M.A[3][0], M.A[3][1], M.A[3][3]) -
         M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2], 
                                   M.A[2][0], M.A[2][1], M.A[2][2],
                                   M.A[3][0], M.A[3][1], M.A[3][2]);  
} /* End of 'MatrDeterm' function */

/* Matrix inversion function.
 * ARGUMENTS:
 *   - Matrix:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  /* build adjoint matrix */
  r.A[0][0] = 
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3], 
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] = 
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3], 
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] = 
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3], 
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] = 
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2], 
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][1] = 
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3], 
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3], 
                  M.A[3][0], M.A[3][2], M.A[3][3]);  
  r.A[2][1] = 
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3], 
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2], 
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][2] = 
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3], 
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] = 
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3], 
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3], 
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] = 
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2], 
                  M.A[3][0], M.A[3][1], M.A[3][2]);
  r.A[0][3] = 
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3], 
                  M.A[2][1], M.A[2][2], M.A[2][3]);
  r.A[1][3] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3], 
                  M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] = 
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3], 
                  M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2], 
                  M.A[2][0], M.A[2][1], M.A[2][2]);

  /* dividing to det */
  r.A[0][0] /= det;
  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;

  r.A[0][1] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;

  r.A[0][2] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;

  r.A[0][3] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;

  return r;
} /* End of 'MatrInverse' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrViev' function */

__inline MATR MatrFrustum( DBL Left, DBL Right, DBL Bottom, DBL Top, DBL Near, DBL Far )
{
  MATR r =
  {
    {
      {2 * Near / (Right - Left), 0, 0, 0},
      {0, 2 * Near / (Top - Bottom), 0, 0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), -1},
      {0, 0, -2 * Near * Far / (Far - Near), 0}
    }
  };
 
  return r;
} /* End of 'MatrFrustum' function */

#endif /* __mth_h_ */
 
/* END OF 'mth.h' FILE */ 