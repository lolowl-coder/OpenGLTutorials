#version 300 es

precision mediump float;

uniform vec4 uColor;
uniform sampler2D uTexture;

in vec2 vUv;

out vec4 color;

void main (void)
{
   color = texture(uTexture, vUv) * uColor;
}