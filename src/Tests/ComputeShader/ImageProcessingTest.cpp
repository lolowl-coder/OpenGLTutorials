#include "MathUtils.h"
#include "Platform/Log.h"
#include "Render/RenderContext.h"
#include "Tests/ComputeShader/ImageProcessingTest.h"

#define IMAGES_COUNT 25
#define SRC_IMAGE_SIZE 200
#define DST_IMAGE_SIZE 1024
#define QUAD_VERTICES_COUNT 4
#define GROUP_SIZE 1

namespace ComputeShaders
{

   ImageProcessingShader::ImageProcessingShader()
   : Shader()
   {

   }

   void ImageProcessingShader::init()
   {
      load("Data/Shader/ComputeShader/imageProcessing.cs", GL_COMPUTE_SHADER);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
   }

   RenderShader::RenderShader()
   : Shader()
   , mOffsetLocation(-1)
   , mScaleLocation(-1)
   , mTextureUnitLocation(-1)
   {
   }

   void RenderShader::init()
   {
      load("Data/Shader/ComputeShader/imageProcessing.vs", "Data/Shader/ComputeShader/imageProcessing.fs");

      mOffsetLocation = uniformLocation("uOffset");
      mScaleLocation = uniformLocation("uScale");
      mTextureUnitLocation = uniformLocation("uTexture");
   }

   void RenderShader::setUniforms(const Vector3f& offset, const Vector3f scale)
   {
      uniform3(mOffsetLocation, 1, &offset);
      uniform3(mScaleLocation, 1, &scale);
      const int textureUnit = 0;
      uniform1i(mTextureUnitLocation, 1, &textureUnit);
   }

   ImageProcessingTest::ImageProcessingTest(Director& director, const std::string& name)
   : Test(director, name)
   , mQuad(GL_ARRAY_BUFFER, 0, 0)
   , mImageInfos(0, 0, 0)
   , mFlatImageData(1, 0, 0)
   , mUpdateImageDelay(10)
   , mFramesSinceImageUpdate(0)
   {
      mQuad.addAttribute(Attribute(3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), 0));
   }

   static Vector3f quadVertices[] = {
      Vector3f ( -0.5f, -0.5f, 0.0f ),
      Vector3f ( -0.5f,  0.5f, 0.0f ),
      Vector3f (  0.5f, -0.5f, 0.0f ),
      Vector3f (  0.5f,  0.5f, 0.0f )
   };

   void ImageProcessingTest::logCapacities()
   {
      GLint maxComputeShaderStorageBlocks;
      glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &maxComputeShaderStorageBlocks);
      logMsg("max compute shader storage blocks: %i", maxComputeShaderStorageBlocks);

      GLint maxUBOSize;
      glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE , &maxUBOSize);
      logMsg("max UBO size: %i", maxUBOSize);

      GLint maxTextureSize;
      glGetIntegerv(GL_MAX_TEXTURE_SIZE , &maxTextureSize);
      logMsg("max texture size: %i", maxTextureSize);

      GLint maxComputeUniformComponents;
      glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &maxComputeUniformComponents);
      logMsg("max compute uniform components: %i", maxComputeUniformComponents);

      GLint maxComputeUniformBlocks;
      glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &maxComputeUniformBlocks);
      logMsg("max compute uniform blocks: %i", maxComputeUniformBlocks);

      GLint maxUniformBlockSize;
      glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
      logMsg("max uniform block size: %i", maxUniformBlockSize);

      /*GLint maxTextureBufferSize;
      glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE_OES , &maxTextureBufferSize);
      logMsg("max texture buffer size: %i", maxTextureBufferSize);*/
   }

   void ImageProcessingTest::initShaders()
   {
      mComputeShader.init();
      mRenderShader.init();
   }

   void ImageProcessingTest::initBgTexture()
   {
      const Vector4f bgSpotColor(0.0f, 0.9f, 1.0f, 1.0f);
      mBgTexture.genSpot(DST_IMAGE_SIZE, bgSpotColor);
      mBgTexture.create();
   }

   //info for image generation
   //image info, that will be stored in SSBO
   struct ImageInfoGPU
   {
      Vector2i mSize; //pixels
      Vector2i mSrcOffset; //elements, not bytes
      Vector2i mDstOffset; //pixels
   };

   void ImageProcessingTest::genImage(Texture& texture, const ImageInfo& imageInfo)
   {
      switch(imageInfo.mImageType)
      {
      case IT_CHECKER:
         {
            const Vector3f checkerColor0(0.0f, 0.0f, 0.0f);
            const Vector3f checkerColor1(imageInfo.mColor.r, imageInfo.mColor.g, imageInfo.mColor.b);
            texture.genChecker(checkerColor0, checkerColor1, imageInfo.mSize);
         }
         break;
      case IT_SPOT:
         {
            const Vector4f color(imageInfo.mColor.r, imageInfo.mColor.g, imageInfo.mColor.b, 1.0f);
            texture.genSpot(imageInfo.mSize, color);
         }
         break;
      }
   }

   ImageInfo ImageProcessingTest::genRndImageInfo()
   {
      static const Vector4f colors[] =
      {
         Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
         Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
         Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
         Vector4f(1.0f, 1.0f, 0.0f, 1.0f),
         Vector4f(0.0f, 1.0f, 1.0f, 1.0f),
         Vector4f(1.0f, 0.0f, 1.0f, 1.0f)
      };

      ImageInfo imageInfo
      (
         SRC_IMAGE_SIZE,
         colors[(int)MathUtils::rnd(0.0f, sizeof(colors) / sizeof(Vector4f) - 1)],
         (EImageType)((int)MathUtils::rnd())
      );

      return imageInfo;
   }
   
   void ImageProcessingTest::initFlatImageData()
   {
      //info for image generation
      std::vector<ImageInfo> imageInfos;
      
      for(int i = 0; i < IMAGES_COUNT; i++)
      {
         ImageInfo imageInfo = genRndImageInfo();

         imageInfos.push_back(imageInfo);
      }

      int flatImagesSizeInBytes = 0;

      for(int i = 0; i < IMAGES_COUNT; i++)
      {
         const ImageInfo& imageInfo = imageInfos[i];

         flatImagesSizeInBytes += imageInfo.mSize * imageInfo.mSize;
      }

      flatImagesSizeInBytes *= 4;
      //find next power of 2 size of image
      //int nextPow2 = (int)pow(2.0f, (float)ceil(log(sqrt((float)flatImagesSizeInBytes / 4.0f)) / log(2.0f)));
      //Vector2i pow2FlatImageSize(nextPow2, nextPow2);

      //here sticked image will be stored
      unsigned char* flatImageData = new unsigned char[flatImagesSizeInBytes];
      //array of image infos to pass to compute shader as SSBO
      std::vector<ImageInfoGPU> imageInfosGPU;
      imageInfosGPU.resize(IMAGES_COUNT);

      int offset = 0;

      //generate images
      for(int i = 0; i < IMAGES_COUNT; i++)
      {
         const ImageInfo& imageInfo = imageInfos[i];
         ImageInfoGPU& imageInfoGPU = imageInfosGPU[i];
         imageInfoGPU.mSize = Vector2i(imageInfo.mSize, imageInfo.mSize);
         imageInfoGPU.mSrcOffset = Vector2i(offset, 0);
         int row = i / 5;
         int col = i - row * 5;
         imageInfoGPU.mDstOffset = Vector2i(col, row) * SRC_IMAGE_SIZE;

         Texture texture;

         genImage(texture, imageInfo);
         
         const int imageSize = imageInfo.mSize * imageInfo.mSize;
         memcpy(&flatImageData[offset * 4], texture.getData(), imageSize * 4);
         
         offset += imageSize;
      }
      
      //init buffers
      mImageInfos.setData(IMAGES_COUNT * sizeof(ImageInfoGPU), (void*)&imageInfosGPU[0]);
      mFlatImageData.setData(flatImagesSizeInBytes, flatImageData);

      delete [] flatImageData;
   }

   void ImageProcessingTest::initDstTexture()
   {
      const Vector3f checkerColor0(0.0f, 0.0f, 0.0f);
      const Vector3f checkerColor1(1.0f, 1.0f, 1.0f);
      mDstTexture.genChecker(checkerColor0, checkerColor1, DST_IMAGE_SIZE);
      mDstTexture.create();
   }

   void ImageProcessingTest::init()
   {
      logCapacities();
      mQuad.setData(sizeof(quadVertices), quadVertices);
      initShaders();
      initBgTexture();
      initFlatImageData();
      initDstTexture();
   }

   void ImageProcessingTest::deinit()
   {
      mComputeShader.deinit();
      mRenderShader.deinit();
      mQuad.deinit();
      mBgTexture.deinit();
      mDstTexture.deinit();
      mImageInfos.deinit();
      mFlatImageData.deinit();
   }

   void ImageProcessingTest::renderQuad(RenderShader& shader, const Vector3f& offset, const Vector3f& scale)
   {
      shader.bind();
      shader.setUniforms(offset, scale);
      mQuad.enable();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      mQuad.disable();
      checkGLError("ImageProcessingTest::renderQuad");
   }

   void ImageProcessingTest::updateTexture(int offset)
   {
      ImageInfo imageInfo = genRndImageInfo();
      Texture texture;
      genImage(texture, imageInfo);

      mFlatImageData.bindShaderStorage();
      glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, imageInfo.mSize * imageInfo.mSize * 4, texture.getData());
   }

   void ImageProcessingTest::run()
   {
      initFlatImageData();
      mComputeShader.bind();
      mImageInfos.bindShaderStorage();
      mDstTexture.unbind();
      glBindImageTexture(0, mDstTexture.getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
      checkGLError("bind dst image");
      mFlatImageData.bindShaderStorage();

      if(mFramesSinceImageUpdate > mUpdateImageDelay)
      {
         int imageIndex = (int)(MathUtils::rnd() * IMAGES_COUNT);
         updateTexture(imageIndex * SRC_IMAGE_SIZE * SRC_IMAGE_SIZE * 4);
         mFramesSinceImageUpdate = 0;
      }

      glDispatchCompute(IMAGES_COUNT, 1, 1);

      mBgTexture.bind();
      renderQuad(mRenderShader, Vector3f(), Vector3f(2.0f, 2.0f, 2.0f));
      mDstTexture.bind();
      renderQuad(mRenderShader, Vector3f(), Vector3f(1.0f, 1.0f, 1.0f));

      mFramesSinceImageUpdate++;
   }

}