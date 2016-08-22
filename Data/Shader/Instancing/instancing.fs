#version 300 es

precision mediump float;

in vec2 v_uv;
in float v_alpha;

uniform sampler2D u_texture;
out vec4 color;

void main (void)
{
   color = texture ( u_texture, v_uv );
   //color.a = v_alpha;
}