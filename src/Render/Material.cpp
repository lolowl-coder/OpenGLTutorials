#include "Render/Material.h"

Material::Material()
: mShininess(0.0f)
{
}

Material::Material(Vector4f& ambient, Vector4f& diffuse, Vector4f& specular, float shininess)
: mAmbient(ambient)
, mDiffuse(diffuse)
, mSpecular(specular)
, mShininess(shininess)
{
}