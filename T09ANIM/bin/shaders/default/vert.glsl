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
uniform mat4 MatrWInv;
uniform float Time;

// out parameters (varying)
out vec4 DrawColor;
out vec3 DrawNormal;
out vec2 DrawTexCoord;
out vec3 DrawPos;

layout(binding = 0) uniform sampler2D Tex0;

void main( void )
{            
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  /* DrawNormal = transpose(inverse(mat3(MatrW))) * InNormal; */
  DrawNormal = mat3(MatrWInv) * InNormal;
  DrawPos = (MatrW * vec4(InPosition, 1)).xyz;
  DrawTexCoord = InTexCoord;
}


