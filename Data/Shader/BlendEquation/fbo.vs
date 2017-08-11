#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform vec3 uOffset;
uniform vec3 uScale;

out vec2 vUv;

void main ( void )
{
   vUv = aPosition.xy;
   gl_Position = vec4(aPosition * uScale + uOffset, 1.0);
}