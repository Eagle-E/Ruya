#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

ruya::Camera::Camera() : mHorizontalAngle(0), mVerticalAngle(0)
{
    mCamPos = glm::vec3(0.0f, 0.0f, 25.0f);
    mCamFront = glm::vec3(0.0f, 0, -1.0f);
    mCamUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void ruya::Camera::updateAngle(float dx, float dy)
{
    if (dx == 0 && dy == 0)
        return;

    // update angle if result is within minimum and maximum angles
    if(MIN_VERTICAL_ANGLE < mVerticalAngle-dy && mVerticalAngle-dy < MAX_VERTICAL_ANGLE)
        mVerticalAngle -= dy;

    if(MIN_HORIZONTAL_ANGLE < mHorizontalAngle+dx && mHorizontalAngle+dx < MAX_HORIZONTAL_ANGLE)
        mHorizontalAngle += dx;
    else
        mHorizontalAngle = 0;

    // update view vertex
    this->mCamFront.x = sin(mHorizontalAngle) * cos(mVerticalAngle);
    this->mCamFront.z = -cos(mHorizontalAngle) * cos(mVerticalAngle);
    this->mCamFront.y = sin(mVerticalAngle);
}

float ruya::Camera::getHorizontalAngle() const
{
    return mHorizontalAngle;
}

float ruya::Camera::getVerticalAngle() const
{
    return mVerticalAngle;
}

glm::mat4 ruya::Camera::getViewMatrix() const
{
    return glm::lookAt(mCamPos, mCamPos + mCamFront, mCamUp);
}

void ruya::Camera::goForward()
{
    mCamPos = mCamPos + mCamFront * mCamSpeed;
}

void ruya::Camera::goBackWard()
{
    mCamPos = mCamPos - mCamFront * mCamSpeed;
}

void ruya::Camera::goRight()
{
    mCamPos = mCamPos + glm::normalize(glm::cross(mCamFront, mCamUp)) * mCamSpeed;
}

void ruya::Camera::goLeft()
{
    mCamPos = mCamPos - glm::normalize(glm::cross(mCamFront, mCamUp)) * mCamSpeed;
}

void ruya::Camera::goUp()
{
    glm::vec3 right = glm::cross(mCamFront, mCamUp);
    mCamPos = mCamPos + glm::normalize(glm::cross(right, mCamFront)) * mCamSpeed;
}

void ruya::Camera::goDown()
{
    glm::vec3 right = glm::cross(mCamFront, mCamUp);
    mCamPos = mCamPos - glm::normalize(glm::cross(right, mCamFront)) * mCamSpeed;
}

void ruya::Camera::calculateMovement(float& dx, float& dy)
{
    dx = sin(-mHorizontalAngle) * SENSITIVITY;
    dy = sin(mVerticalAngle) * SENSITIVITY;
//    dz = cos(mHorizontalAngle) * SENSITIVITY;
}

glm::vec3 ruya::Camera::getCamFront() const
{
    return mCamFront;
}


glm::vec3 ruya::Camera::position() const
{
    return mCamPos;
}
