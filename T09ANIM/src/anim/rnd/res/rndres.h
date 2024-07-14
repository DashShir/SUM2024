/* FILE NAME  : rndres.h
 * PROGRAMMER : DS4
 * LAST UPDATE: 06.07.2024
 * PURPOSE    : 3D animation project.
 *              Render resourse declaration module.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "def.h"

#define DS4_STR_MAX 1000

/* Resourses initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndResInit( VOID );

/* Resourses close function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndResClose( VOID );

/***
 * Shaders support
 ***/

/* Shaders stock maximum size */
#define DS4_MAX_SHADERS 30

/* Shader program store type */
typedef struct tagds4SHADER ds4SHADER;
struct tagds4SHADER
{
  CHAR Name[DS4_STR_MAX]; /* Shader filename prefix */
  UINT ProgId;            /* Shader program Id */
};

/* Array of shaders */
extern ds4SHADER DS4_RndShaders[DS4_MAX_SHADERS];
/* Shadres array store size */
extern INT DS4_RndShadersSize;

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load:
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT DS4_RndShdAdd( CHAR *ShaderFileNamePrefix );

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdUpdate( VOID );

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdInit( VOID );

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndShdClose( VOID );

/***
 * Textures support
 ***/

/* Texture representation type */
typedef struct tagds4TEXTURE
{
  CHAR Name[DS4_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} ds4TEXTURE;

/* Textures stock maximum size */
#define DS4_MAX_TEXTURES 300

/* Tetxure stock */
extern ds4TEXTURE DS4_RndTextures[DS4_MAX_TEXTURES]; /* Array of textures */
extern INT DS4_RndTexturesSize;                      /* Textures array store size */

/* Get texture from image function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits );

/* Get texture from file function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTexAddFromFile( CHAR *FileName );

/* Get texture from file function
 * ARGUMENTS: None.
 * RETURNS: Number added texture.
 */
INT DS4_RndTextureFree( INT TexNo );

/* Texture initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexInit( VOID );

/* All textures pushing function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndTexClose( VOID );

/***
 * Materials support
 ***/

/* Material store type */
#define MAT_N (sizeof(MatLib) / sizeof(MatLib[0]))

typedef struct tagds4MATERIAL
{
  CHAR Name[DS4_STR_MAX]; /* Material name */

  /* Illumination coefficients */    
  VEC3 Ka, Kd, Ks;          /* Ambient, diffuse, specular coefficients */
  FLT Ph;                   /* Phong power coefficient */

  FLT Trans;                /* Transparency factor */

  INT Tex[8];               /* Texture references from texture table (or -1) */

  INT ShdNo;                /* Shader number in shader table */

  INT UBONo;                /* UBO buffer resource number */
} ds4MATERIAL;

/* Material stock maximum size */
#define DS4_MAX_MATERIALS 300

/* Material stock */
extern ds4MATERIAL DS4_RndMaterials[DS4_MAX_MATERIALS]; /* Array of materials */
extern INT DS4_RndMaterialsSize;                        /* Materials array */

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) added material number in DS4_RndMaterials.
 */
INT DS4_RndMtlAdd( ds4MATERIAL *Mtl );

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ds4MATERIAL) base material.
 */
ds4MATERIAL DS4_RndMtlGetDef( VOID );

/* Return base material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (INT) shader ProgId.
 */
INT DS4_RndMtlApply( INT MtlNo );

/* Material system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndMtlInit( VOID );

/* Material system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndMtlClose( VOID );

/* Getting material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   None.
 */
ds4MATERIAL * DS4_RndMtlGet( INT MtlNo );

/***
 * Font handle functions
 ***/

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndFntInit( VOID );

/* Deinit font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndFntClose( VOID );

/* Load font from .G3DF file function.
 * ARGUMENTS:
 *   - font file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL DS4_RndFntLoad( CHAR *FileName );

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
VOID DS4_RndFntDraw( CHAR *Str, VEC3 Pos, FLT Size, VEC3 Color );

/***
 * UBO handle functions
 ***/

/* UBO representation type */
typedef struct tagds4UBO
{
  CHAR Name[DS4_STR_MAX]; /* UBO name */
  INT Size;               /* UBO size in bytes */
  UINT UBOId;             /* OpenGL UBO Id */ 
  UINT BindPoint;         /* OpenGL UBO bind point */
} ds4UBO;

/* UBOs stock */
#define DS4_MAX_UBOS 30
extern ds4UBO DS4_RndUBOs[DS4_MAX_UBOS]; /* Array of UBO */
extern INT DS4_RndUBOsSize;              /* UBO array store size */

/* UBO handle initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndUBOInit( VOID );

/* UBO handle deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID DS4_RndUBOClose( VOID );

/* Add UBO to stock function.
 * ARGUMENTS:
 *   - UBO name:
 *       CHAR *Name;
 *   - OpenGL UBO bind point:
 *       UINT BindPoint;
 *   - UBO size in bytes:
 *       INT Size;
 *   - UBO data:
 *       VOID *Bits;
 * RETURNS:
 *   (INT) UBO stock number (-1 if error is occured).
 */
INT DS4_RndUBOCreate( CHAR *Name, UINT BindPoint, INT Size, VOID *Bits );

/* UBO data update function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 *   - UBO offset in bytes:
 *       INT Offset;
 *   - UBO update data size in bytes:
 *       INT Size;
 *   - UBO data ('Size' bytes):
 *       VOID *Bits;
 * RETURNS: None.
 */
VOID DS4_RndUBOUpdate( INT UBONo, INT Offset, INT Size, VOID *Bits );

/* UBO apply function.
 * ARGUMENTS:
 *   - UBO number in stock:
 *       INT UBONo;
 * RETURNS: None.
 */
VOID DS4_RndUBOApply( INT UBONo );

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */
