#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 uMVP;
uniform vec4 uColor;
uniform vec3 uEye;

out vec4 vColor;

const vec3 lightPosition = vec3(0.5, 1.0, 0.5);

void main ( void )
{
   vec4 position = uMVP * vec4(aPosition, 1.0);
   vec3 dir = uEye - aPosition;
   //vec3 dir = lightPosition - aPosition;
   normalize(dir);

   vColor = vec4(dot(aNormal, dir) * uColor.rgb, uColor.a);
   //vColor = uColor;
   
   gl_Position = position;
}