#version 300 es

precision mediump float;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;
uniform mat4 uLightSpaceMatrix;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;
out vec4 vPosLightSpace;

void main(void)
{
   vPosition = (uM * vec4(aPosition, 1.0)).xyz;
   mat3 rotation = transpose(inverse(mat3(uM)));
   vNormal = rotation * aNormal;
   vPosLightSpace = uLightSpaceMatrix * vec4(vPosition, 1.0);
   
   gl_Position = uP * uV * uM * vec4(aPosition, 1.0);
}