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

out vec4 color;

void main (void)
{
   vec4 ambientColor = objectMaterial.ambient * light.ambient;
   
   vec3 lightDir = light.position - vPosition;
   lightDir = normalize(lightDir);
   float lightAngleFactor = dot(vNormal, lightDir);
   vec3 camDir = uEye - vPosition;
   camDir = normalize(camDir);
   float cameraAngleFactor = dot(vNormal, camDir);
   float diffuse = lightAngleFactor * cameraAngleFactor;
   vec4 diffuseColor = objectMaterial.diffuse * light.diffuse * diffuse;
   
   vec3 reflectedDir = reflect(-lightDir, vNormal);
   float specular = pow(max(dot(camDir, reflectedDir), 0.0), objectMaterial.shininess);
   vec4 specularColor = objectMaterial.specular * light.specular * specular * diffuse;
   
   float distToLight = length(lightDir);
   float attenuation = 1.0 - min(1.0, distToLight / light.attenuation);
   
   color = ambientColor + (diffuseColor + specularColor) * attenuation;
}