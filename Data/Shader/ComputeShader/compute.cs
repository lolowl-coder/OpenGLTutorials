#version 310 es

uniform int u_frameId;
uniform vec3 u_step;
uniform vec2 u_dimensions;
uniform float u_dt;

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

   /*vec3 c0 = colors[max(index, 0)];
   vec3 c1 = colors[min(index + 1, 5)];*/
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

vec3 avg2(vec3 v0, vec3 v1)
{
   return (v0 + v1) * 0.5;
}

float samplePressure(float row, float col, uint gid)
{
   /*const float border = 0.01;
   if(col < border || col > 1.0 - border ||
      row < border || row > 1.0 - border)
   {
      return 0.0;
   }*/

   //return TFD[gid].y;
   return rand(vec2(row, col));
}

void main()
{
   uint gid = gl_GlobalInvocationID.x;
   
   vec2 v = Velocity[gid].zw;

   //find previous position
   vec2 prevPos = Position[gid].xy - v * u_dt * 50.0;
   //col, row of prev position
   float col = clamp(trunc(prevPos.x / u_step.x), 0.0, u_dimensions.x);
   float row = clamp(trunc(prevPos.y / u_step.y), 0.0, u_dimensions.y);
   float prevCol = clamp(col - 1.0, 0.0, u_dimensions.x);
   float nextCol = clamp(col + 1.0, 0.0, u_dimensions.x);
   float prevRow = clamp(row - 1.0, 0.0, u_dimensions.y);
   float nextRow = clamp(row + 1.0, 0.0, u_dimensions.y);

   //advect
   bool obstacle = col > 0.0 && col < 130.0 && row > 100.0 && row < 200.0;
   if(obstacle)
   {
      //set force inside rectangular area
      //vec2 force = vec2(1.0, 0.0);
      vec2 force = vec2(1.0, rand(vec2(float(gid) * 10.0, 0.0)) * 1.0);
      v += force * u_dt;
   }
   
   // Clamp velocity at borders to zero.
   if(col > u_dimensions.x - 1.0 ||
      row > u_dimensions.y - 1.0 ||
      col < 0.0 ||
      row < 0.0)
   {
      v = vec2(0.0, 0.0);
   }

   Velocity[gid].xy = v;

   //divergence
   uint leftGid   = uint(clamp(row     * u_dimensions.x + prevCol, 0.0, u_dimensions.x));
   uint rightGid  = uint(clamp(row     * u_dimensions.x + nextCol, 0.0, u_dimensions.x));
   uint bottomGid = uint(clamp(prevRow * u_dimensions.x + col    , 0.0, u_dimensions.y));
   uint topGid    = uint(clamp(nextRow * u_dimensions.x + col    , 0.0, u_dimensions.y));
   float x0 = Velocity[leftGid].x;
   float x1 = Velocity[rightGid].x;
   float y0 = Velocity[bottomGid].y;
   float y1 = Velocity[topGid].y;
   float divergence = ((x1 - x0) + (y1 - y0)) * 0.5;
   //TFD[gid].x = divergence;

   //pressure
   if(obstacle)
   {
      x0 = 0.0;
      x1 = 0.0;
      y0 = 0.0;
      y1 = 0.0;
   }
   else
   {
      x0 = samplePressure(row, col, leftGid);
      x1 = samplePressure(row, col, rightGid);
      y0 = samplePressure(row, col, bottomGid);
      y1 = samplePressure(row, col, topGid);
   }

   TFD[gid].y = (x0 + x1 + y0 + y1 - divergence) * 0.25;

   //subtract pressure
   vec2 pressureGradient = (vec2(x1, y1) - vec2(x0, y0)) * 0.5;
   Velocity[gid].zw = Velocity[gid].xy - pressureGradient;

   //Color[gid] = vec4(Velocity[gid].xy * 0.1, 0.0, 1.0);
   Color[gid] = vec4(Velocity[gid].xy, 0.0, 1.0);
   //Color[gid] = vec4(vec3(prevRow / 512.0), 1.0);


}