#version 300 es

precision mediump float;

uniform vec4 uColor;

in vec2 vUv;

out vec4 color;
uniform sampler2D uTexture;

void main (void)
{
   color = texture(uTexture, vUv) * uColor;
   //color = vec4(1.0, 0.0, 0.0, 1.0);
}