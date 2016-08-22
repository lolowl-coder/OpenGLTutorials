#version 300 es

precision mediump float;

layout (location = 0) in vec3 aQuadVertexPosition;
layout (location = 2) in vec4 aCurrentPositionAge;
layout (location = 3) in vec4 aColor;
layout (location = 4) in vec4 aVelocitySize;

uniform mat4 uMVP;

out vec4 vColor;
out vec2 vUV;

void main(void)
{
#if 0
   vColor = vec4(1.0, 0.0, 0.0, 1.0);
#else
   vColor = aColor;
#endif
   vUV = aQuadVertexPosition.xy + vec2(0.5);
   gl_Position = uMVP * vec4(aCurrentPositionAge.xyz, 1.0) + vec4(aQuadVertexPosition.xy * aVelocitySize.w, 0.0, 1.0);
}