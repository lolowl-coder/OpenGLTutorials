#version 300 es

precision mediump float;

layout (location = 0) in vec3 aPosition;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModelMatrix;

out vec4 color;

void main (void)
{
   gl_Position = uLightSpaceMatrix * uModelMatrix * vec4(aPosition, 1.0f);
}