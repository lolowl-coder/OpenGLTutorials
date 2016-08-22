#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aOffset;
layout (location = 2) in vec4 aColor;

uniform vec3 u_origin;

out vec4 vColor;

void main ( void )
{
   float scale = 0.004;
   vColor = aColor;
   gl_Position = vec4(aPosition * scale + aOffset + u_origin, 1.0);
}