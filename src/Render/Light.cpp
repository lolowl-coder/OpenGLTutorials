#include "Render/Light.h"

Light::Light()
: mAttenuation(0.0f)
{
}

Light::Light(const Vector3f& position, const Vector4f& ambient, const Vector4f& diffuse, const Vector4f& specular, float attenuation)
: mPosition(position)
, mAmbient(ambient)
, mDiffuse(diffuse)
, mSpecular(specular)
, mAttenuation(attenuation)
{
}