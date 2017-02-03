#version 310 es

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct InParticle
{
   vec4 positionTtl;
   vec4 velocitySize;
   vec4 alphaAge;
};

struct OutParticle
{
   vec4 positionSize;
   vec4 color;
};

layout(std140, binding = 0) buffer InPtcs
{
   InParticle particles[];
} inputData;

layout(std140, binding = 1) buffer OutPtcs
{
   OutParticle particles[];
} outputData;

float rand(vec2 co)
{
   return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

const float dt = 1.0 / 60.0;

void main()
{
   uint gid = gl_GlobalInvocationID.x;
   float ttl = inputData.particles[gid].positionTtl.w;
   vec3 v = inputData.particles[gid].velocitySize.xyz;
   float size = inputData.particles[gid].velocitySize.w;
   float alpha = inputData.particles[gid].alphaAge.x;
   float age = inputData.particles[gid].alphaAge.y;
   
   vec3 p = outputData.particles[gid].positionSize.xyz;
   p = p + v * dt;
   alpha = (1.0 - age / ttl) * alpha;
   
   age = age + dt;
   if(age > ttl)
   {
	   age = age - ttl;
	   p = inputData.particles[gid].positionTtl.xyz;
   }

   outputData.particles[gid].positionSize = vec4(p, size);
   outputData.particles[gid].color.a = alpha;
   
   inputData.particles[gid].alphaAge.y = age;
   inputData.particles[gid].velocitySize = vec4(v, size);
}