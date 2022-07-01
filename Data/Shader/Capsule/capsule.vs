#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform mat4 uMVP;

void main ( void )
{
   gl_Position = uMVP * vec4(aPosition, 1.0);
   gl_PointSize = 3.0;
}