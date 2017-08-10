#include "Render/Object.h"

Object::Object()
: mNormals(GL_ARRAY_BUFFER, 0, 0)
{
   mNormals.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));
}

void Object::deinit()
{
}

void Object::setTranslation(const Matrix4f& transform)
{
   mTranslation = transform;
}

void Object::setRotation(const Matrix4f& rotation)
{
   mRotation = rotation;
}

void Object::setMaterial(const Material& material)
{
   mMaterial = material;
}

void Object::setLight(const Light& light)
{
   mLight = light;
}