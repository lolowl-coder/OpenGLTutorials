#include "Render/Object/Object.h"

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

Matrix4f Object::getTranslation() const
{
   return mTranslation;
}

void Object::setRotation(const Matrix4f& rotation)
{
   mRotation = rotation;
}

Matrix4f Object::getRotation() const
{
   return mRotation;
}

void Object::setMaterial(const Material& material)
{
   mMaterial = material;
}

Material Object::getMaterial() const
{
   return mMaterial;
}

void Object::setLight(const Light& light)
{
   mLight = light;
}

Light Object::getLight() const
{
   return mLight;
}