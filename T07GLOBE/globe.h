/* FILE NAME: globe.h
 * PROGRAMMER: DS4
 * LAST UPDATE: 14.06.2024
 * PURPOSE: 
 */

#ifndef __globe_h_
#define __globe_h_

#include <commondf.h>

/* Definitions */
#define WND_CLASS_NAME "My window class"
#define PI 3.14159265358979323846264338327950288419716939937510582
#define GRID_H 18
#define GRID_W 30
                 
typedef double DBL;
typedef struct
{
  DBL X, Y, Z;
} VEC;

/* Globe initialisation function.
 * ARGUMENTS:
 *   - radius:
 *       DBL R;  
 * RETURNS: None.
 */
VOID GLB_Init( DBL R );

/* Globe resizing function.
 * ARGUMENTS:
 *   - window width and height:
 *       INT W, H; 
 * RETURNS: None.
 */
VOID GLB_Resize( INT W, INT H );

/* Globe resizing
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC; 
 * RETURNS: None.
 */
VOID GLB_Draw( HDC hDC );

#endif /* __globe_h_ */

/* END OF 'globe.h' FILE */