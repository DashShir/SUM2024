/**/
#version 460

// what to put somewhere
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// global variables (Matrixes multiply: World * View * Proj)
uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform float Time;
uniform int ScrW, ScrH;

// out parameters (varying)
out vec4 DrawColor;
out vec2 DrawTexCoord;

layout(binding = 0) uniform sampler2D Tex0;

/* Matrix orthographics projection setup function.
 * ARGUMENTS:
 *   - view volume border coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
mat4 MatrOrtho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
  return mat4(vec4(2 / (Right - Left), 0, 0, 0),
              vec4(0, 2 / (Top - Bottom), 0, 0),
              vec4(0, 0, -2 / (Far - Near), 0),
              vec4(-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1));
} /* End of 'MatrOrtho' function */


void main( void )
{            
  gl_Position = MatrOrtho(0, ScrW, -(ScrH), 0, -1, 1) * MatrW * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}


