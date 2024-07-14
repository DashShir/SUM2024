/**/
#version 460

// out parameters - drawing color
layout(location = 0) out vec4 OutColor;

// in parameters (varying)
in vec4 DrawColor;
in vec2 DrawTexCoord;

uniform float Time;
uniform vec3 Ka, Kd, Ks, CamLoc;
uniform float Ph;

uniform bool IsWireframe;

uniform vec3 AddonV0;

uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D Tex0;

void main( void )
{
  if (IsWireframe)
  {
    OutColor = vec4(1, 1, 1, 1);
    return;
  }
  vec4 tc = texture(Tex0, DrawTexCoord);
  if (tc.a < 0.30)
    discard;
  OutColor = vec4(tc.rgb * AddonV0, tc.a);
}
