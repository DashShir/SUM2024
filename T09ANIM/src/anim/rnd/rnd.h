/* FILE NAME  : rnd.h         
 * PROGRAMMER : DS4
 * LAST UPDATE: 28.06.2024
 * PURPOSE    : 3D animation project.
 *          Render declaration module.
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "res/rndres.h"

/***
 * Render handle function group
 ***/

extern HWND DS4_hRndWnd;                  /* Work window handle */
extern HDC DS4_hRndDC;               /* Work window memory device context  */
extern HGLRC DS4_hRndGLRC;                /* Work OpenGL rendering context */
extern INT DS4_RndFrameW, DS4_RndFrameH;  /* Work window size */
extern FLT
  DS4_RndProjSize,     /* Project plane fit square */
  DS4_RndProjDist,     /* Distance to project plane from viewer (near) */
  DS4_RndProjFarClip;  /* Distance to project far clip plane (far) */
 
extern MATR
  DS4_RndMatrView, /* View coordinate system matrix */
  DS4_RndMatrProj, /* Projection coordinate system matrix */
  DS4_RndMatrVP;   /* Stored (View * Proj) matrix */    

extern VEC3 
  DS4_RndCamLoc,         /* Camera location */
  DS4_RndCamAt,          /* Camera destination */
  DS4_RndCamDir,         /* Camera direction */
  DS4_RndCamRight,       /* Camera right direction */
  DS4_RndCamUp;          /* Camera up direction */

/* Frame overlay layer device context */
extern HDC DS4_hRndFrameDC;

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
VOID DS4_RndCopyFrame( VOID );

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
 *       VEC3 Loc;
 *   - camera view to point:
 *       VEC3 At;
 *   - camera up direction (aproximatly):
 *       VEC3 Up;  
 * RETURNS: None.
 */
VOID DS4_RndCamSet( VEC3 Loc, VEC3 At, VEC3 Up );

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id;
 *   - message severity:
 *       UINT severity;
 *   - message text length:
 *       INT Length;
 *   - message text:
 *       CHAR *Message;
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                             INT Length, const CHAR *Message,
                             const VOID *UserParam );

/***
 * Primitive handle function group
 ***/

/* Vertex representation type */
typedef struct tagds4VERTEX
{
  VEC3 P;   /* Vertex position */
  VEC2 T;   /* Texture coordinate */ 
  VEC3 N;   /* Normale */
  VEC4 C;   /* Colour (r, g, b, a) */
} ds4VERTEX;

/* Primitive type representation type */
typedef enum tagds4PRIM_TYPE
{
  DS4_RND_PRIM_POINTS,   /* Array of points */
  DS4_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  DS4_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  DS4_RND_PRIM_LINES,    /* Line segments (by 2 points) */
} ds4PRIM_TYPE;

/* Primitive representation type */
typedef struct tagds4PRIM
{
  ds4PRIM_TYPE Type;  /* Primitive type */
  INT
    VA,               /* OpenGL vertex array Id */
    VBuf,             /* OpenGL vertex buffer Id */
    IBuf;             /* OpenGL index buffer Id */
  INT NumOfElements;  /* Number of elements */
 
  MATR Trans;         /* Additional transformation matrix */
  VEC3 MinBB, MaxBB;  /* Bound box corner coordinates */

  INT MtlNo;          /* Material number in material array */
} ds4PRIM;

/* Create empty primitive function.
 * ARGUMENTS:
 *   - pointer to primitive to create:
 *       ds4PRIM *Pr;
 *   - primitive type:
 *       ds4PRIM_TYPE Type;
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 */
VOID DS4_RndPrimCreate( ds4PRIM *Pr, ds4PRIM_TYPE Type,
                        ds4VERTEX *V, INT NoofV, INT *Ind, INT NoofI );
 
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
 *       VEC3 Org, Du, Dv;  
 * RETURNS: None.
 */
VOID DS4_RndPrimCreateQuad( ds4PRIM *Pr, VEC3 Org, VEC3 Du, VEC3 Dv );

/* Creating triangle function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - vectors:
 *       VEC3 Org, Du, Dv;  
 * RETURNS: None.
 */
VOID DS4_RndPrimCreateTrian( ds4PRIM *Pr, VEC3 Org, VEC3 Du, VEC3 Dv );

/* Cutting polygonal figure to trinangles function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 * RETURNS: None.
 */
/* BOOL DS4_RndPrimCutToTrians( ds4PRIM *Pr ); */

/* Creating sphere function.
 * ARGUMENTS:
 *   - primitives:
 *       ds4PRIM *Pr;
 *   - center coordinates:
 *       VEC3 Coord;   
 *   - radius of sphere:
 *       FLT R;
 *   - heigh and width of sphere (parallels and meridians):
 *       FLT Par, Mer;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
/*
BOOL DS4_RndPrimCreateSphere( ds4PRIM *Pr, VEC3 Coord, FLT R, FLT Par, FLT Mer );
*/

/* Loading model from .OBJ file function.
 * ARGUMENTS: 
 *   - primitive:
 *       ds4PRIM *Pr;
 *   - model file name:
 *       CHAR *FileName;
 * RETURNS: (BOOL) 1 if success, otherwise 0.
 */
BOOL DS4_RndPrimLoad( ds4PRIM *Pr, CHAR *FileName );

/* Primitive bound box obtaining function.
 * ARGUMENTS:
 *   - pointer to result min-max vectors:
 *       VEC *MinBB, *MaxBB;
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 * RETURNS: None.
 */
VOID DS4_RndPrimEvalBB( VEC3 *MinBB, VEC3 *MaxBB, ds4VERTEX *V, INT NoofV );

/* Primitive vertex normals evaluate function.
 * ARGUMENTS:
 *   - vertex attributes array:
 *       ds4VERTEX *V;
 *   - vertex attributes array size:
 *       INT NoofV;
 *   - primitive vertex index array:
 *       INT *Ind;
 *   - primitive vertex index array size:
 *       INT NoofI;
 * RETURNS: None.
 */
VOID DS4_RndPrimTrimeshAutoNormals( ds4VERTEX *V, INT NoofV, INT *Ind, INT NoofI );

/***
 * Grid handle function group
 ***/
                         
/* Grid topology representation type */
typedef struct tagds4GRID
{
  INT W, H;      /* Grid size (in vertices) */
  ds4VERTEX *V;  /* Array (2D) of vertex – W * H vertices */
} ds4GRID;

/* Create grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreate( ds4GRID *G, INT W, INT H );

/* Free grid function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndGridFree( ds4GRID *G );

/* Build grid normals function.
 * ARGUMENTS:
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndGridAutoNormals( ds4GRID *G );

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - grid data:
 *       ds4GRID *G;
 * RETURNS: None.
 */
VOID DS4_RndPrimFromGrid( ds4PRIM *Pr, ds4GRID *G );

/* Sphere grid init function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - sphere radius:
 *       FLT R;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreateSphere( ds4GRID *G, FLT R, INT W, INT H );

/* Torus grid init function.
 * ARGUMENTS:
 *   - primitive to be create:
 *       ds4PRIM *Pr;
 *   - torus radiuses - inside and outside:
 *       FLT RIn, FLT ROut;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndGridCreateTorus( ds4GRID *G, FLT RIn, FLT ROut, INT W, INT H );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */