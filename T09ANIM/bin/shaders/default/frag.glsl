/**/
#version 330

// out parameters - drawing color
layout(location = 0) out vec4 OutColor;

// in parameters (varying)
in vec4 DrawColor;
in vec3 DrawNormal;
in vec3 DrawPos;
in vec2 DrawTexCoord;

uniform float Time;

void main( void )
{
  /*
  if (DrawPos.x > 0.30)
    discard;
  if (DrawPos.y > 0.12 + 0.12 * sin(Time))
    discard;
  */
  OutColor = DrawColor;
  //OutColor *= vec4(1, 1, 1, 1);
  OutColor.xyz *= DrawNormal;

  //if (abs(mod(DrawPos.x + 0.30 * Time, 0.20)) < 0.01)
  //  OutColor.xyz += vec3(0, 1, 0);

  float time_an = 2;
  vec3 L = normalize(vec3(cos(Time * time_an) * 1, 1, cos(Time * time_an) * 1));

  float nl = dot(normalize(DrawNormal), L);
  if (nl < 0.1)
    nl = 0.1;
  OutColor.xyz = DrawColor.rgb * nl;

  float q = 0; /* pow(abs(sin((DrawPos.x + 0.30 * Time) * 3)), 102 * 1000); */
  float r = 0; /* pow(abs(sin((DrawPos.y + 0.30 * Time) * 3)), 102 * 800); */
  float p = 0; /* pow(abs(sin((DrawPos.z + 0.30 * Time) * 3)), 102 * 800); */
  OutColor.xyz += vec3(p, q, r);
                                /*
  if (mod(gl_FragCoord.x, 2) < 1)
    discard;
  */
  float cf = 1;

  if ((int(DrawTexCoord.x * 30) ^ int(DrawTexCoord.y * 30)) % 2 == 0)
    cf = 1;

  float fs = 10, fe = 50;
  vec3 loc = vec3(110, 0, 0);
  float d = distance(loc, DrawPos);
  vec3 fc = vec3(0.30, 0.47, 0.8);

  d = clamp((fe - d) / (fe - fs), 0, 1);
  d = pow(d, 0.1);
  OutColor.xyz = mix(OutColor.xyz * cf, fc, d);



}
