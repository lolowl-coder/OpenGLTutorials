#version 310 es

uniform int u_frameId;
uniform vec3 u_step;
uniform vec2 u_dimensions;

#define USE_BLI 0

layout( std140, binding=1 ) buffer Pos
{
   vec4 Position[ ];
};

layout( std140, binding=2 ) buffer Col
{
   vec4 Color[ ];
};

layout( std140, binding=3 ) buffer TpFlDn
{
   //temperature, fuel, density
   vec4 TFD[ ];
};

layout( std140, binding=4 ) buffer Vel
{
	vec4 Velocity[ ];
};

layout( local_size_x = 64, local_size_y = 1, local_size_z = 1 ) in;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

//temperature range: 0-12000, K
vec3 computeColor(float temperature)
{
   const float maxTemperature = 12000.0f;
   const float temperatureStep = 2000.0f;

   vec3 colors[6];
   colors[0] = vec3(0.0, 0.0, 0.0);
   colors[1] = vec3(1.0, 0.0, 0.0);
   colors[2] = vec3(1.0, 1.0, 0.0);
   colors[3] = vec3(1.0, 1.0, 1.0);
   colors[4] = vec3(0.0, 0.0, 1.0);
   colors[5] = vec3(1.0, 0.0, 1.0);

   float findex;
   float t = modf(temperature / temperatureStep, findex);
   int index = int(findex) - 1;

   vec3 c0 = colors[max(index, 0)];
   vec3 c1 = colors[min(index + 1, 5)];

   return mix(c0, c1, t);
   /*float i = temperature / 12000.0;
   return vec3(i, i, i);*/
}

vec3 avg(vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, vec3 t)
{
   return (v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7) / 8.0;
}

void main()
{
   uint gid = gl_GlobalInvocationID.x;
   //float l = length(Position[gid].xy);

   //if(l > 0.0)
   //{
      float dt = 0.01;
      vec3 v = Velocity[gid].xyz * dt;

      //find previous position
      vec3 prevPos = Position[gid].xyz - v;
      //distance from origin
      //indices of neighbors
      float v0IndexX;
      float v0IndexY;

      //find cell index and local t parameter
      vec3 t = vec3(
         modf(prevPos.x / u_step.x, v0IndexX),
         modf(prevPos.y / u_step.y, v0IndexY),
         0.0);
   #if USE_BLI
      uint v0Index = uint((v0IndexY) * u_dimensions.x + v0IndexX);
      uint v1Index = uint((v0IndexY) * u_dimensions.x + v0IndexX + 1.0);
      uint v2Index = uint((v0IndexY + 1.0) * u_dimensions.x + v0IndexX + 1.0);
      uint v3Index = uint((v0IndexY + 1.0) * u_dimensions.x + v0IndexX);

      float d0 = (1.0 - t.x) * (1.0 - t.y);
      float d1 = t.x * (1.0 - t.y);
      float d2 = t.x * t.y;
      float d3 = (1.0 - t.x) * t.y;

      vec3 tfd0 = TFD[v0Index].xyz;
      vec3 tfd1 = TFD[v1Index].xyz;
      vec3 tfd2 = TFD[v2Index].xyz;
      vec3 tfd3 = TFD[v3Index].xyz;

      vec3 v0 = Velocity[v0Index].xyz;
      vec3 v1 = Velocity[v1Index].xyz;
      vec3 v2 = Velocity[v2Index].xyz;
      vec3 v3 = Velocity[v3Index].xyz;

      vec3 vel = v0 * d0 + v1 * d1 + v2 * d2 + v3 * d3;
      /*vec3 noise = vec3(
         rand(vec2(float(gid), 0.0)) - 0.5,
         rand(vec2(float(gid + uint(u_frameId)), 0.0)) - 0.5, 0.0);*/
      Velocity[gid].xyz = vel/* + noise * 0.03*/;
      vec3 tfd = tfd0 * d0 + tfd1 * d1 + tfd2 * d2 + tfd3 * d3;
      //TFD[gid].xyz = tfd;
   #else
      uint v0Index = uint((v0IndexY - 1.0) * u_dimensions.x + v0IndexX - 1.0);
      uint v1Index = uint((v0IndexY + 1.0) * u_dimensions.x + v0IndexX + 1.0);
      uint v2Index = uint((v0IndexY - 1.0) * u_dimensions.x + v0IndexX + 1.0);
      uint v3Index = uint((v0IndexY + 1.0) * u_dimensions.x + v0IndexX - 1.0);
      uint v4Index = uint((v0IndexY      ) * u_dimensions.x + v0IndexX - 1.0);
      uint v5Index = uint((v0IndexY      ) * u_dimensions.x + v0IndexX + 1.0);
      uint v6Index = uint((v0IndexY - 1.0) * u_dimensions.x + v0IndexX      );
      uint v7Index = uint((v0IndexY + 1.0) * u_dimensions.x + v0IndexX      );
      vec3 tfd0 = TFD[v0Index].xyz;
      vec3 tfd1 = TFD[v1Index].xyz;
      vec3 tfd2 = TFD[v2Index].xyz;
      vec3 tfd3 = TFD[v3Index].xyz;
      vec3 tfd4 = TFD[v4Index].xyz;
      vec3 tfd5 = TFD[v5Index].xyz;
      vec3 tfd6 = TFD[v6Index].xyz;
      vec3 tfd7 = TFD[v7Index].xyz;

      vec3 v0 = Velocity[v0Index].xyz;
      vec3 v1 = Velocity[v1Index].xyz;
      vec3 v2 = Velocity[v2Index].xyz;
      vec3 v3 = Velocity[v3Index].xyz;
      vec3 v4 = Velocity[v4Index].xyz;
      vec3 v5 = Velocity[v5Index].xyz;
      vec3 v6 = Velocity[v6Index].xyz;
      vec3 v7 = Velocity[v7Index].xyz;
      
      Velocity[gid].xyz = avg(v0, v1, v2, v3, v4, v5, v6, v7, t) * 0.98;

      TFD[gid].xyz = avg(tfd0, tfd1, tfd2, tfd3, tfd4, tfd5, tfd6, tfd7, t) * 0.98;
#endif
   /*}
   else
   {
      TFD[gid].xyz = vec3(0.0, 0.0, 0.0);
      Velocity[gid].xy = reflect(Velocity[gid].xy, Position[gid].xy);
      //Velocity[gid].xy = vec2(0.0, 0.0);
   }*/
   Color[gid] = vec4(computeColor(TFD[gid].x), 1.0);

   /*if(Temperature[gid] > IGNITION_TEMP)
   {
      Fuel[gid] -= FUEL_DECREASE;
   }*/
}