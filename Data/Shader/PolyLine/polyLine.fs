#version 300 es

precision mediump float;

uniform vec4 uColor;

out vec4 color;

void main (void)
{
   color = uColor;
}