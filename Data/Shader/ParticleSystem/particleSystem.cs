#version 310 es

//xyz - position, w - ttl
layout(std140, binding = 1) buffer PosTtl
{
   vec4 PositionTtl[];
};

//xyz - current position, w - age
layout(std140, binding = 2) buffer CurPosAge
{
	vec4 CurrentPositionAge[];
};

//rgba - color
layout(std140, binding = 3) buffer Col
{
   vec4 Color[];
};

//xyz - velocity, w - size
layout(std140, binding = 4) buffer VelSz
{
	vec4 VelocitySize[];
};

//x - alpha
layout(std140, binding = 5) buffer Alph
{
	vec4 Alpha[];
};

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

const float dt = 1.0 / 60.0;

void main()
{
   uint gid = gl_GlobalInvocationID.x;
   vec3 p = CurrentPositionAge[gid].xyz;
   float age = CurrentPositionAge[gid].w;
   float ttl = PositionTtl[gid].w;
   vec4 color = Color[gid];
   vec3 v = VelocitySize[gid].xyz;
   float size = VelocitySize[gid].w;
   float alpha = Alpha[gid].x;
   
   p = p + v * dt;
   color.a = (1.0 - age / ttl) * alpha;
   
   age = age + dt;
   if(age > ttl)
   {
	   age = age - ttl;
	   p = PositionTtl[gid].xyz;
   }

   CurrentPositionAge[gid].xyz = p;
   CurrentPositionAge[gid].w = age;
   Color[gid] = color;
   VelocitySize[gid].xyz = v;
   VelocitySize[gid].w = size;
}