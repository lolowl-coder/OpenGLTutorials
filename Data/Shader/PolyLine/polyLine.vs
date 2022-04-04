#version 300 es

precision mediump float;

uniform float uPointSize;

layout (location = 0) in vec2 aPosition;

void main ( void )
{
   gl_Position = vec4(aPosition * 2.0 - 1.0, 0.0, 1.0);
   gl_PointSize = uPointSize;
}