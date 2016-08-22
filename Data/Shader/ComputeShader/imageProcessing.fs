#version 300 es

precision mediump float;

in vec2 vUv;

out vec4 color;
uniform sampler2D uTexture;

void main (void)
{
   color = texture(uTexture, vUv);
   //color = vec4(0.0, 1.0f, 0.0, 1.0);
}