/**/
#version 460

// out parameters - drawing color
layout(location = 0) out vec4 OutColor;

// in parameters (varying)
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

uniform float Time;
/* uniform vec3 Ka, Kd, Ks, CamLoc; */
uniform vec3 CamLoc;
/* uniform float Ph, Trans; */

layout (std140, binding = 2) uniform Material
{
  vec4 MtlKa;
  vec4 MtlKdTrans;
  vec4 MtlKsPh;
  bvec4 MtlIsTextures[2];
};
#define Ka MtlKa.xyz
#define Kd MtlKdTrans.xyz
#define Trans MtlKdTrans.w
#define Ks MtlKsPh.xyz
#define Ph MtlKsPh.w
#define IsTexture0 MtlIsTextures[0].x
#define IsTexture1 MtlIsTextures[0].y
#define IsTexture2 MtlIsTextures[0].z

uniform int AddonI0, AddonI1;

uniform bool IsWireframe;

//uniform bool IsTexture0;
layout(binding = 0) uniform sampler2D Tex0;

vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LC )
{
  vec3 color = vec3(0);
  vec3 V = normalize(P - CamLoc);

  N = faceforward(N, V, N);

  // Ambient
  color = min(vec3(0.1), Ka);

  // Diffuse
  vec3 dif_color = Kd;
  if (IsTexture0)
  {
    dif_color *= texture(Tex0, DrawTexCoord).rgb;
  }
  color += max(0, dot(N, L)) * dif_color * LC;

  // Specular
  vec3 R = reflect(V, N);
  color += pow(max(0, dot(R, L)), Ph) * Ks * LC;

  return color;
}

void main( void )
{
  //if (abs(sin(Time)) * AddonI0 >= AddonI1)
  //if (AddonI1 == 8)
  //  discard;
  if (IsWireframe)
  {
    OutColor = vec4(1, 1, 1, 1);
    return;
  }
  float time_an = 2;
  vec3 L = normalize(vec3(cos(Time * time_an) * 1, 1, cos(Time * time_an) * 1));

  OutColor = vec4(Shade(DrawPos, normalize(DrawNormal), L, vec3(1, 1, 1)), Trans);
}
