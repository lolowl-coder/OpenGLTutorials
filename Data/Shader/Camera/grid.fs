#version 300 es

precision mediump float;

uniform vec4 uDiffuse;
uniform vec3 uEye;

const vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
const vec3 lightPosition = vec3(3.0, 3.0, 3.0);
const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
const float shininess = 4.0;

float attnMax = 8.0;

in vec3 vPosition;
in vec3 vNormal;

out vec4 color;

void main (void)
{
   vec4 ambientColor = ambient * lightColor;
   
   vec3 lightDir = lightPosition - vPosition;
   lightDir = normalize(lightDir);
   float lightAngleFactor = dot(vNormal, lightDir);
   vec3 camDir = uEye - vPosition;
   camDir = normalize(camDir);
   float cameraAngleFactor = dot(vNormal, camDir);
   float diffuse = lightAngleFactor * cameraAngleFactor;
   vec4 diffuseColor = uDiffuse * diffuse;
   
   vec3 reflectedDir = reflect(-lightDir, vNormal);
   float specular = pow(max(dot(camDir, reflectedDir), 0.0), shininess);
   vec4 specularColor = lightColor * specular * diffuse;
   
   float distToLight = length(lightDir);
   float attn = 1.0 - min(1.0, distToLight / attnMax);
   
   color = ambientColor + (diffuseColor + specularColor) * attn;
}