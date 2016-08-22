#version 300 es

precision mediump float;

in vec2 vUv;
in vec4 vColor;

out vec4 color;
uniform sampler2D uTexture;

void main (void)
{
   color = texture(uTexture, vUv) * vColor;
}