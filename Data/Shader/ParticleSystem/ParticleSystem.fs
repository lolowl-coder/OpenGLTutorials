#version 300 es

precision mediump float;

uniform sampler2D uTextureUnit;

in vec4 vColor;
in vec2 vUV;

out vec4 color;

void main (void)
{
   color = texture(uTextureUnit, vUV) * vColor;
}