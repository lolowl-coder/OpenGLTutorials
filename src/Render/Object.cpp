#include "Render/Object.h"

Object::Object()
: mNormals(GL_ARRAY_BUFFER, 0, 0)
{
   mNormals.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));
}

void Object::deinit()
{
}

void Object::setTransform(const Matrix4f& transform)
{
   mTransform = transform;
}

void Object::setMaterial(const Material& material)
{
   mMaterial = material;
}

void Object::setLight(const Light& light)
{
   mLight = light;
}