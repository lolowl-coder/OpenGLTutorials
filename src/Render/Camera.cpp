#include "Render/Camera.h"

Camera::Camera()
: mProjectionType(P_PERSPECTIVE)
, mPosition(0.0f, 0.0f, 0.0f)
, mLookAt(0.0f, 0.0f, -1.0f)
, mUp(0.0f, 1.0f, 0.0f)
, mFOV(45.0f)
, mNear(1.0f)
, mFar(1000.0f)
, mYaw(0.0f)
, mAbsPitch(0.0f)
, mPitch(0.0f)
, mRotationDelta(1.0f, 0.0f, 0.0f, 0.0f)
{

}

void Camera::update(float timeDelta)
{
   updatePosition(timeDelta);

   Vector3f cameraDirection = mLookAt - mPosition;
   cameraDirection.normalize();

   switch (mProjectionType)
   {
   case P_ORTHO:
      {
         mP = Matrix4f::createOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
      }
      break;
   case P_PERSPECTIVE:
      {
         mP = Matrix4f::createFrustum(-1.0f, 1.0f, -1.0f, 1.0f, mNear, mFar);
      }
      break;
   }
#if 0
   //Matrix3f r = Matrix3f::createRotationAroundAxis(mPitch, mYaw, 0.0f);
   //mRotationDelta.normalize();
   Quatf q = mRotationDelta;
   q.normalize();
   Matrix3f r = q.rotMatrix();
   Vector3f direction = mLookAt - mPosition;
   direction = r * direction;
   //direction = mRotationDeltaM * direction;
   mLookAt = mPosition + direction;
   //mLookAt.normalize();
   mV = Matrix4f::createLookAt(mPosition, mLookAt, mUp);
   mMVP = mP * mV;

   setRotationDelta(Quatf(1.0f, 0.0f, 0.0f, 0.0f));
   mYaw = 0.0f;
   mPitch = 0.0f;
#else
   Vector3f direction = mLookAt - mPosition;
   Vector3f pitchAxis = direction.crossProduct(mUp);
   Quatf pitchQuat = Quatf::fromAxisRot(pitchAxis, mPitch);
   Quatf headingQuat = Quatf::fromAxisRot(mUp, mYaw);
   Quatf temp = pitchQuat * headingQuat;
   temp.normalize();
   direction = temp.rotMatrix() * direction;

   mLookAt = mPosition + direction;

   mV = Matrix4f::createLookAt(mPosition, mLookAt, mUp);
   mVP = mP * mV;

   mYaw = 0.0f;
   mPitch = 0.0f;
#endif
}

void Camera::updatePosition(float timeDelta)
{
   Vector3f forward = mLookAt - mPosition;
   forward.y = 0.0f;
   forward.normalize();

   Vector3f side = forward.crossProduct(mUp);
   side.normalize();

   Vector3f offset = (side * mVelocity.x + forward * mVelocity.z + mUp * mVelocity.y) * timeDelta;
   
   mPosition += offset;
   mLookAt += offset;
}

void Camera::setProjectionType(EProjectionType projectionType)
{
   mProjectionType = projectionType;
}

Vector3f Camera::getVelocity() const
{
   return mVelocity;
}

void Camera::setVelocity(const Vector3f velocity)
{
   mVelocity = velocity;
}

Vector3f Camera::getPosition() const
{
   return mPosition;
}

void Camera::setPosition(const Vector3f& position)
{
   mPosition = position;
}

Vector3f Camera::getLookAt() const
{
   return mLookAt;
}

void Camera::setLookAt(const Vector3f& lookAt)
{
   mLookAt = lookAt;
}

void Camera::setRotationDelta(const Quatf& rotationDelta)
{
   mRotationDelta = rotationDelta;
}

void Camera::setYawPitch(float yaw, float pitch)
{
   mYaw = yaw;
   mPitch = pitch;
   if(mAbsPitch + mPitch > mFOV)
   {
      mPitch = mFOV - mAbsPitch;
   }
   if(mAbsPitch + mPitch < -mFOV)
   {
      mPitch = -mFOV - mAbsPitch;
   }
   mAbsPitch += mPitch;
}

void Camera::setFOV(const float fov)
{
   mFOV = fov;
}

Matrix4f Camera::getV() const
{
   return mV;
}

Matrix4f Camera::getP() const
{
   return mP;
}