#version 300 es

precision mediump float;

layout (location = 0) in vec3 aQuadVertexPosition;

uniform vec3 uOffset;
uniform vec4 uColor;
uniform vec3 uScale;
uniform vec2 uTile;

out vec4 vColor;
out vec2 vUv;

void main ( void )
{
   vColor = uColor;
   vUv = aQuadVertexPosition.xy * uTile;
   gl_Position = vec4((aQuadVertexPosition.xyz + uOffset) * uScale, 1.0);
}