#version 300 es

precision mediump float;

uniform sampler2D uTextureUnit;
uniform vec2 uStep;
uniform int uRadius;

in vec2 vUV;

out vec4 color;

void main (void)
{
   vec4 c0 = texture(uTextureUnit, vUV);
   vec4 c1 = vec4(0.0);
   for(int i = 1; i < uRadius + 1; i++)
   {
      c1 += texture(uTextureUnit, vUV + float(i) * uStep);
      c1 += texture(uTextureUnit, vUV - float(i) * uStep);
   }
   color = (c0 + c1) / float(uRadius * 2 + 1);
}