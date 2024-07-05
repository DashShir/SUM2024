/**/
#version 330

// what to put somewhere
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// global variables (Matrixes multiply: World * View * Proj)
uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform float Time;

// out parameters (varying)
out vec4 DrawColor;
out vec3 DrawNormal;
out vec2 DrawTexCoord;
out vec3 DrawPos;

void main( void )
{            
  gl_Position = MatrWVP * vec4(InPosition +
                               vec3(0, 0, 0) +
                               InPosition.x * InPosition.x * 0 + 0 * vec3(0, sin(Time * 0 + InPosition.x), 0),
                               1);
  DrawColor = InColor;
  DrawNormal = transpose(inverse(mat3(MatrW))) * InNormal;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawTexCoord = InTexCoord;
}


