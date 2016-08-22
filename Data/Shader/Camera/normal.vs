#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform mat4 uMVP;

out vec4 vColor;

void main ( void )
{
   vec4 position = vec4(aPosition, 1.0);
   vColor = position;
   
   gl_Position = uMVP * position;
}