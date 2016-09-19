#version 300 es

precision mediump float;

layout (location = 0) in vec3 aQuadVertexPosition;

uniform mat4 uMVP;

out vec2 vUV;

void main(void)
{
   vUV = aQuadVertexPosition.xy;
   gl_Position = vec4((aQuadVertexPosition.xyz - vec3(0.5, 0.5, 0.0)) * 2.0, 1.0);
}