/* FILE NAME  : rnd.h         
 * PROGRAMMER : DS4
 * LAST UPDATE: 21.06.2024
 * PURPOSE    : 3D animation project.
 *          Render declaration module.
 */

#include "def.h"

/***
 * Render handle function group
 ***/

extern HWND DS4_hRndWnd;        /* Work window handle */
extern HDC DS4_hRndDCFrame;     /* Work window memory device context  */
extern HBITMAP DS4_hRndBmFrame; /* Work window background bitmap handle */
extern INT DS4_RndFrameW, DS4_RndFrameH; /* Work window size */
extern DWORD *DS4_RndFrameBits; /* Frame buffer pixel data (from DIB section) */
extern DBL
  DS4_RndProjSize,     /* Project plane fit square */
  DS4_RndProjDist,     /* Distance to project plane from viewer (near) */
  DS4_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  DS4_RndMatrView, /* View coordinate system matrix */
  DS4_RndMatrProj, /* Projection coordinate system matrix */
  DS4_RndMatrVP;   /* Stored (View * Proj) matrix */    

/* Render initialization function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;  
 * RETURNS: None.
 */
VOID DS4_RndInit( HWND hWnd );

/* Render deinitialization function.
 * ARGUMENTS: None. 
 * RETURNS: None.
 */
VOID DS4_RndClose( VOID );

/* Resize function.
 * ARGUMENTS:
 *   - height, wight:
 *       INT W, H;  
 * RETURNS: None.
 */
VOID DS4_RndResize( INT W, INT H );

/* Frame copy function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;  
 * RETURNS: None.
 */
VOID DS4_RndCopyFrame( HDC hDC );

/* Render frame start function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndStart( VOID );

/* Render frame ending function.
 * ARGUMENTS: None.  
 * RETURNS: None.
 */
VOID DS4_RndEnd( VOID );

/* Set projection parameters function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndProjSet( VOID );

/* Camera setting function.
 * ARGUMENTS:
 *   - camera location:
 *       VEC Loc;
 *   - camera view to point:
 *       VEC At;
 *   - camera up direction (aproximatly):
 *       VEC Up;  
 * RETURNS: None.
 */
VOID DS4_RndCamSet( VEC Loc, VEC At, VEC Up );


/***
 * Primitive handle function group
 ***/

/* Vertex representation type */
typedef struct tagds4VERTEX
{
  VEC P;  /* Vertex position */
} ds4VERTEX;

/* Primitive representation type */
typedef struct tagds4PRIM
{
  ds4VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */
 
  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */
 
  MATR Trans;   /* Additional transformation matrix */
} ds4PRIM;

/* Primitive creating function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ds4PRIM *Pr;
 *   - num of vertexes, indexes:
 *       INT NoofV, NoofI;
 * RETURNS: 1 if okey, otherwise 0.
 */
BOOL DS4_RndPrimCreate( ds4PRIM *Pr, INT NoofV, INT NoofI );

/* Free primitive function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ds4PRIM *Pr;  
 * RETURNS: None.
 */
VOID DS4_RndPrimFree( ds4PRIM *Pr );

/* Camera setting function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       ds4PRIM *Pr;
 *   - world matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID DS4_RndPrimDraw( ds4PRIM *Pr, MATR World );


/* Creating quadlirateral function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC Org, Du, Dv;  
 * RETURNS: None.
 */
BOOL DS4_RndPrimCreateQuad( ds4PRIM *Pr, VEC Org, VEC Du, VEC Dv );

/* Creating triangle function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC Org, Du, Dv;  
 * RETURNS: None.
 */
BOOL DS4_RndPrimCreateTrian( ds4PRIM *Pr, VEC Org, VEC Du, VEC Dv );

/* Cutting polygonal figure to trinangles function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
/* BOOL DS4_RndPrimCutToTrians( ds4PRIM *Pr ); */

/* Creating sphere function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - center coordinates:
 *       VEC Coord;   
 *   - radius of sphere:
 *       DBL R;
 *   - heigh and width of sphere (parallels and meridians):
 *       DBL Par, Mer;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
BOOL DS4_RndPrimCreateSphere( ds4PRIM *Pr, VEC Coord, DBL R, DBL Par, DBL Mer );

/* Loading model from .OBJ file function.
 * ARGUMENTS: 
 *   - primitive:
 *       ds4PRIM *Pr;
 *   - model file name:
 *       CHAR *FileName;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
BOOL DS4_RndPrimLoad( ds4PRIM *Pr, CHAR *FileName );


/* END OF 'rnd.h' FILE */