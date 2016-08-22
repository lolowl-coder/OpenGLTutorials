#version 300 es

in vec3 a_position;
in vec2 a_uv;
in vec3 a_objectPosition;                

out	vec2 v_uv;
out float v_alpha;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

//#define GENERATE_POSITION

void main (void)
{
   v_uv = a_uv;
   
#ifdef GENERATE_POSITION
   float x = rand(vec2(float(gl_InstanceID), 0.0)) * 2.0 - 1.0;
   float y = rand(vec2(float(gl_InstanceID) * 1.3, 0.0)) * 2.0 - 1.0;
   vec3 offset = vec3(x, y, 0.0);
#else
   vec3 offset = a_objectPosition;
#endif
   v_alpha = float(gl_InstanceID) / 50.0;
   gl_Position = vec4 ( a_position * 0.1 + offset, 1.0 );
}