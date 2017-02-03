#version 300 es

precision mediump float;

struct InParticle
{
   vec4 positionSize;
   vec4 color;
};

layout (location = 0) in vec3 aQuadVertexPosition;
layout (location = 1) in InParticle aParticle;

uniform mat4 uMVP;

out vec4 vColor;
out vec2 vUV;

void main(void)
{
#if 0
   vColor = vec4(1.0, 0.0, 0.0, 1.0);
#else
   vColor = aParticle.color;
#endif
   vUV = aQuadVertexPosition.xy + vec2(0.5);
   gl_Position = uMVP * vec4(aParticle.positionSize.xyz, 1.0) + vec4(aQuadVertexPosition.xy * aParticle.positionSize.w, 0.0, 1.0);
}