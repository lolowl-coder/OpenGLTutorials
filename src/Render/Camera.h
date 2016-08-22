#pragma once

#include "Director.h"
#include "Render/Shader.h"

#include "vmath.h"

class Camera
{
public:
   enum EProjectionType
   {
      P_ORTHO,
      P_PERSPECTIVE
   };

public:
   Camera(const Director& director);

   void update();
   void updatePosition();
   void setProjectionType(EProjectionType projectionType);
   Vector3f getVelocity() const;
   void setVelocity(const Vector3f velocity);
   Vector3f getPosition();
   void setPosition(const Vector3f& position);
   Vector3f getLookAt();
   void setLookAt(const Vector3f& lookAt);
   void setRotationDelta(const Quatf& rotationDelta);
   void setYawPitch(float yaw, float pitch);
   void setFOV(const float fov);
   Matrix4f getV();
   Matrix4f getP();

private:
   const Director& mDirector;
   EProjectionType mProjectionType;
   Vector3f mVelocity;
   Vector3f mPosition;
   Vector3f mLookAt;
   Vector3f mUp;
   float mFOV;
   float mNear;
   float mFar;
   float mYaw;
   float mPitch;
   float mAbsPitch;
   Quatf mRotationDelta;
   Matrix4f mP;
   Matrix4f mV;
   Matrix4f mVP;
};