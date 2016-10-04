#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform mat4 uMVP;
uniform vec4 uColor;

out vec4 vColor;

void main ( void )
{
   vec4 position = vec4(aPosition, 1.0);
   vColor = uColor;
   
   gl_Position = uMVP * position;
}