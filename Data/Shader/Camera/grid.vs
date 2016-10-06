#version 300 es

precision mediump float;

uniform mat4 uMVP;
uniform mat4 uM;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

void main(void)
{
   vPosition = (uM * vec4(aPosition, 1.0)).xyz;
   vNormal = (uM * vec4(aNormal, 1.0)).xyz;
   
   vec4 position = uMVP * vec4(aPosition, 1.0);
   gl_Position = position;
}