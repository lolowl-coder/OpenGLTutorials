#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform mat4 uMVP;

void main ( void )
{
   vec3 pos = aPosition;
   pos.z -= 1.5;
   float offset = sign(pos.z) * 2.0;
   pos.z += offset;
   gl_Position = uMVP * vec4(pos, 1.0);
   gl_PointSize = .0;
}