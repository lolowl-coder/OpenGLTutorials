#include "Render/ParticleSystem.h"
#include "Render/RenderContext.h"

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

ParticleSystem::ParticleSystem()
: mCount(0)
, mTtl(0.0f)
, mSize(0.0f)
, mEmitter(NULL)
, mQuad(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0)
, mPositionsTtl(GL_SHADER_STORAGE_BUFFER, 1, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), 1)
, mCurrentPositionsAge(GL_SHADER_STORAGE_BUFFER, 2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), 1)
, mColorRangeR(0.2f, 1.0f)
, mColorRangeG(0.2f, 1.0f)
, mColorRangeB(0.2f, 1.0f)
, mColorRangeA(0.5f, 1.0f)
, mColors(GL_SHADER_STORAGE_BUFFER, 3, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), 1)
, mVelocitiesSize(GL_SHADER_STORAGE_BUFFER, 4, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), 1)
, mAlpha(GL_SHADER_STORAGE_BUFFER, 5, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f), 1)
{

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

      std::vector<Vector4f> positionTtl;
      std::vector<Vector4f> color;
      std::vector<Vector4f> velocitySize;
      std::vector<Vector4f> currentPositionAge;
      std::vector<Vector4f> alpha;

      for(int i = 0; i < mCount; i++)
      {
         Vector3f position = mEmitter->getPosition();
         positionTtl.push_back(Vector4f(position, mTtl));
         currentPositionAge.push_back(Vector4f(Vector3f(), mTtl * i / mCount));
         color.push_back(Vector4f(mColorRangeR.get(), mColorRangeG.get(), mColorRangeB.get(), 0.0f));
         Vector3f velocity = mEmitter->getVelocity();
         velocitySize.push_back(Vector4f(velocity, mSize));
         alpha.push_back(Vector4f(mColorRangeA.get(), 0.0f, 0.0f, 0.0f));
      }

      mPositionsTtl.setData(mCount * sizeof(Vector4f), &positionTtl[0]);
      mCurrentPositionsAge.setData(mCount * sizeof(Vector4f), &currentPositionAge[0]);
      mColors.setData(mCount * sizeof(Vector4f), &color[0]);
      mVelocitiesSize.setData(mCount * sizeof(Vector4f), &velocitySize[0]);
      mAlpha.setData(mCount * sizeof(Vector4f), &alpha[0]);

      mComputeShader.load("Data/Shader/ParticleSystem/particleSystem.cs", GL_COMPUTE_SHADER);
      mRenderShader.load("Data/Shader/ParticleSystem/particleSystem.vs", "Data/Shader/ParticleSystem/ParticleSystem.fs");
   }
}

void ParticleSystem::deinit()
{
   mQuad.deinit();
   mPositionsTtl.deinit();
   mColors.deinit();
   mVelocitiesSize.deinit();
   mCurrentPositionsAge.deinit();
   mAlpha.deinit();
   mComputeShader.deinit();
   mRenderShader.deinit();
}

void ParticleSystem::render(const Matrix4f& v, const Matrix4f& p)
{
   mPositionsTtl.bindShaderStorage();
   mColors.bindShaderStorage();
   mVelocitiesSize.bindShaderStorage();
   mCurrentPositionsAge.bindShaderStorage();
   mAlpha.bindShaderStorage();
   checkGLError("bind ssbo");

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
   mPositionsTtl.enable();
   mCurrentPositionsAge.enable();
   mColors.enable();
   mVelocitiesSize.enable();
   mAlpha.enable();

   glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quadVerticesCount, mCount);
   checkGLError("instanced draw");

   mQuad.disable();
   mPositionsTtl.disable();
   mColors.disable();
   mCurrentPositionsAge.disable();
   mAlpha.disable();
   
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   checkGLError("render particle system");
}