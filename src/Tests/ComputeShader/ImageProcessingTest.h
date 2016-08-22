#pragma once

#include "Render/Buffer.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Tests/Test.h"

namespace ComputeShaders
{

   enum EImageType
   {
      IT_CHECKER,
      IT_SPOT
   };

   struct ImageInfo
   {
      ImageInfo(const int size, const Vector4f color, const EImageType imageType)
      : mSize(size)
      , mColor(color)
      , mImageType(imageType)
      {
      }

      int mSize; //pixels
      Vector4f mColor;
      EImageType mImageType;
   };

   class ImageProcessingShader : public Shader
   {
   public:
      ImageProcessingShader();

      virtual void init();
   };

   class RenderShader : public Shader
   {
   public:
      RenderShader();

      virtual void init();
      virtual void setUniforms(const Vector3f& offset, const Vector3f scale);

   protected:
      GLint mOffsetLocation;
      GLint mScaleLocation;
      GLint mTextureUnitLocation;
   };

   //Pass many images to compute shader as SSBO
   class ImageProcessingTest : public Test
   {
   public:
      ImageProcessingTest(const Director& director, const std::string& name);

      virtual void init();
      virtual void deinit();
      virtual void run();

   private:
      void logCapacities();
      void initShaders();
      void initBgTexture();
      void genImage(Texture& texture, const ImageInfo& imageInfo);
      ImageInfo genRndImageInfo();
      void initFlatImageData();
      void initDstTexture();
      void updateTexture(int offset);
      void renderQuad(RenderShader& shader, const Vector3f& offset, const Vector3f& scale);

   private:
      ImageProcessingShader mComputeShader;
      RenderShader mRenderShader;
      Buffer mQuad;
      Texture mBgTexture;
      Buffer mImageInfos;
      Buffer mFlatImageData;
      Texture mDstTexture;
      int mUpdateImageDelay; //delay for texture update, frames
      int mFramesSinceImageUpdate;
   };

}