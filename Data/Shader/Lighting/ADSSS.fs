#version 300 es

precision mediump float;

uniform vec4 uDiffuse;
uniform vec3 uEye;

struct ObjectMaterial
{
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float shininess;
};

uniform ObjectMaterial objectMaterial;
uniform sampler2D s_shadowMap;

struct Light
{
   vec3 position;
   vec4 ambient;
   vec4 diffuse;
   vec4 specular;
   float attenuation;
};

uniform Light light;

in vec3 vPosition;
in vec3 vNormal;
in vec4 vPosLightSpace;

out vec4 color;

float calcShadow(vec3 normal, vec3 lightDir)
{
	vec3 projCoords = vPosLightSpace.xyz / vPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float bias = 0.0005;
	float currentDepth = projCoords.z;
	
	#if 0
		float closestDepth = texture(s_shadowMap, projCoords.xy).r;
	
		float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
	#else
		float shadow = 0.0;
		vec2 texelSize = 1.0 / vec2(textureSize(s_shadowMap, 0));
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(s_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
			}    
		}
		shadow /= 9.0;
		shadow = 1.0 - shadow;
	#endif
	
	return shadow;
}

void main (void)
{
   vec4 ambientColor = objectMaterial.ambient * light.ambient;
   
   vec3 lightDir = light.position - vPosition;
   lightDir = normalize(lightDir);
   vec3 normal = normalize(vNormal);
   float lightAngleFactor = dot(normal, lightDir);
   vec3 camDir = uEye - vPosition;
   camDir = normalize(camDir);
   float cameraAngleFactor = dot(normal, camDir);
   float diffuse = lightAngleFactor * cameraAngleFactor;
   vec4 diffuseColor = objectMaterial.diffuse * light.diffuse * diffuse;
   
   vec3 reflectedDir = reflect(-lightDir, normal);
   float specular = pow(max(dot(camDir, reflectedDir), 0.0), objectMaterial.shininess);
   vec4 specularColor = objectMaterial.specular * light.specular * specular * diffuse;
   
   float distToLight = length(lightDir);
   float attenuation = 1.0 - min(1.0, distToLight / light.attenuation);
   
   float shadow = calcShadow(normal, lightDir);
   
   color = ambientColor + (diffuseColor + specularColor) * attenuation * shadow;
}