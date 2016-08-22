#pragma once

#include "Render/Buffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "vmath.h"

#include <vector>

template <typename T>
struct Range
{
   T mMin;
   T mMax;

   Range()
   : mMin((T)0)
   , mMax((T)0)
   {
   }

   Range(const T min, const T max)
   : mMin(min)
   , mMax(max)
   {
   }

   T get()
   {
      return (T)rand() / RAND_MAX * (mMax - mMin) + mMin;
   }
};

typedef Range<float> Rangef;

class Emitter
{
public:
   Emitter(){}
   virtual Vector3f getPosition() = 0;
   virtual Vector3f getVelocity() = 0;
};

class PointEmitter : public Emitter
{
public:
   PointEmitter();
   void setPosition(const Vector3f& position);
   virtual Vector3f getPosition();

   void setVelocityRange(const Rangef& rangeX, const Rangef& rangeY, const Rangef& rangeZ);
   virtual Vector3f getVelocity();

private:
   Vector3f mPosition;
   Rangef mVelocityRangeX;
   Rangef mVelocityRangeY;
   Rangef mVelocityRangeZ;
};

class ParticleSystemShader : public Shader
{
public:
   ParticleSystemShader();
   
   void setMVP(const Matrix4f& mvp);
   void setTextureUnit(const int textureUnit);

private:
   GLint mMVPLocation;
   GLint mTextureUnitLocation;
};

class ParticleSystem
{
public:
   ParticleSystem();

   void setCount(const int count);
   void setTtl(const float ttl);
   void setSize(const float size);
   void setEmitter(Emitter* emitter);
   void setTexture(const Texture& texture);

   void init();
   void deinit();

   void render(const Matrix4f& v, const Matrix4f& p);

private:
   int mCount;
   float mTtl;
   float mSize;

   Matrix4f mM;
   Emitter* mEmitter;
   
   Buffer mQuad;
   Buffer mPositionsTtl;
   Buffer mCurrentPositionsAge;
   Rangef mColorRangeR;
   Rangef mColorRangeG;
   Rangef mColorRangeB;
   Rangef mColorRangeA;
   Buffer mColors;
   Buffer mVelocitiesSize;
   Buffer mAlpha;

   Texture mTexture;
   ParticleSystemShader mRenderShader;
   Shader mComputeShader;
};