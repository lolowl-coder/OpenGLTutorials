#include "Render/Object/ParticleSystem.h"
#include "Render/RenderContext.h"
#include "Platform/Log.h"

const int quadVerticesCount = 4;

Vector3f Emitter::getPosition()
{
   return Vector3f();
}

PointEmitter::PointEmitter()
: Emitter()
{

}

void PointEmitter::setPosition(const Vector3f& position)
{
   mPosition = position;
}

Vector3f PointEmitter::getPosition()
{
   return mPosition;
}

void PointEmitter::setVelocityRange(const Rangef& rangeX, const Rangef& rangeY, const Rangef& rangeZ)
{
   mVelocityRangeX = rangeX;
   mVelocityRangeY = rangeY;
   mVelocityRangeZ = rangeZ;
}

Vector3f PointEmitter::getVelocity()
{
   return Vector3f(mVelocityRangeX.get(), mVelocityRangeY.get(), mVelocityRangeZ.get());
}

ParticleSystemShader::ParticleSystemShader()
: mMVPLocation(-1)
{

}

void ParticleSystemShader::setMVP(const Matrix4f& mvp)
{
   if(mMVPLocation < 0)
   {
      mMVPLocation = uniformLocation("uMVP");
   }

   uniformMatrix4(mMVPLocation, 1, &mvp);
}

void ParticleSystemShader::setTextureUnit(const int textureUnit)
{
   if(mTextureUnitLocation < 0)
   {
      mTextureUnitLocation = uniformLocation("uTextureUnit");
   }

   uniform1i(mTextureUnitLocation, 1, &textureUnit);
}

struct InParticle
{
   InParticle(const Vector4f& positionTtl, const Vector4f& velocitySize, const Vector4f& alphaAge)
      : mPositionTtl(positionTtl)
      , mVelocitySize(velocitySize)
      , mAlphaAge(alphaAge)
   {
   }

   Vector4f mPositionTtl;
   Vector4f mVelocitySize;
   Vector4f mAlphaAge;
};

struct OutParticle
{
   OutParticle(const Vector4f& positionSize, const Vector4f& color)
      : mPositionSize(positionSize)
      , mColor(color)
   {
   }

   Vector4f mPositionSize;
   Vector4f mColor;
};

ParticleSystem::ParticleSystem()
: mCount(0)
, mTtl(0.0f)
, mSize(0.0f)
, mEmitter(NULL)
, mQuad(GL_ARRAY_BUFFER, 0, 0)
, mInParticles(0, 0, 0)
, mOutParticles(1, 1, 1)
, mColorRangeR(0.2f, 1.0f)
, mColorRangeG(0.2f, 1.0f)
, mColorRangeB(0.2f, 1.0f)
, mColorRangeA(0.5f, 1.0f)
{
   mQuad.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));

   mOutParticles.addAttribute(Attribute(4, GL_FLOAT, GL_FALSE, sizeof(OutParticle), 0));                 //current position, size
   mOutParticles.addAttribute(Attribute(4, GL_FLOAT, GL_FALSE, sizeof(OutParticle), sizeof(Vector4f)));  //color

   GLint stride;
   glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, &stride);   logMsg("stride: %d", stride);
}

void ParticleSystem::setCount(const int count)
{
   mCount = count;
}

void ParticleSystem::setTtl(const float ttl)
{
   mTtl = ttl;
}

void ParticleSystem::setSize(const float size)
{
   mSize = size;
}

void ParticleSystem::setEmitter(Emitter* emitter)
{
   mEmitter = emitter;
}

void ParticleSystem::setTexture(const Texture& texture)
{
   mTexture = texture;
}

static Vector3f vertices[] = {
   Vector3f ( -0.5f, -0.5f, 0.0f ),
   Vector3f ( -0.5f,  0.5f, 0.0f ),
   Vector3f (  0.5f, -0.5f, 0.0f ),
   Vector3f (  0.5f,  0.5f, 0.0f )
};

void ParticleSystem::init()
{
   if(mEmitter && mCount > 0)
   {
      mQuad.setData(quadVerticesCount * sizeof(Vector3f), vertices);

      std::vector<InParticle> inParticles;
      std::vector<OutParticle> outParticles;

      for(int i = 0; i < mCount; i++)
      {
         Vector3f position = mEmitter->getPosition();
         InParticle inParticle(
            Vector4f(position, mTtl),
            Vector4f(mEmitter->getVelocity(), mSize),
            Vector4f(mColorRangeA.get(), mTtl * i / mCount, 0.0f, 0.0f)
            );
         inParticles.push_back(inParticle);

         OutParticle outParticle(
            Vector4f(Vector3f(), mSize),
            Vector4f(mColorRangeR.get(), mColorRangeG.get(), mColorRangeB.get(), 0.0f)
            );
         outParticles.push_back(outParticle);
      }

      mInParticles.setData(mCount * sizeof(InParticle), &inParticles[0]);
      mOutParticles.setData(mCount * sizeof(OutParticle), &outParticles[0]);

      mComputeShader.load("Data/Shader/ParticleSystem/particleSystem.cs", GL_COMPUTE_SHADER);
      mRenderShader.load("Data/Shader/ParticleSystem/particleSystem.vs", "Data/Shader/ParticleSystem/ParticleSystem.fs");
   }
}

void ParticleSystem::deinit()
{
   mQuad.deinit();
   mInParticles.deinit();
   mComputeShader.deinit();
   mRenderShader.deinit();
}

void ParticleSystem::render(const Matrix4f& v, const Matrix4f& p)
{
   mInParticles.bindShaderStorage();
   mOutParticles.bindShaderStorage();

   mComputeShader.bind();
   const int workGroupSize = 64;
   glDispatchCompute(mCount / workGroupSize, 1, 1);
   glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
   checkGLError("invoke compute shader");

   mTexture.bind();

   mRenderShader.bind();
   Matrix4f mvp = p * v * mM;
   mRenderShader.setMVP(mvp);
   //reverse view matrix
   const int textureUnit = 0;
   mRenderShader.setTextureUnit(textureUnit);

   mQuad.enable();
   mOutParticles.enable();

   glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quadVerticesCount, mCount);
   checkGLError("instanced draw");

   mQuad.disable();
   mOutParticles.disable();
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   checkGLError("render particle system");
}