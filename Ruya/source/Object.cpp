#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>


ruya::Object::Object()
    : mPosition(0.0f), mRotation(0.0f), mScale(1.0f), mColor(0.7f), mMesh(nullptr), mTexture(nullptr), mParent(nullptr)
{
}

ruya::Object::~Object()
{
}


/*
* Get the model matrix that defines the transformation (scaling, rotation 
* and translation) of this object.
*/
mat4 ruya::Object::model_matrix()
{
    mat4 scalingMatrix(1.0f);
    mat4 rotationMatrix(1.0f);
    mat4 translationMatrix(1.0f);

    scalingMatrix = glm::scale(scalingMatrix, mScale);

    // rotate around custom axis
    //if (mRotation.x != 0 || mRotation.y != 0 || mRotation.z != 0)
    //    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.w), glm::vec3(mRotation.x, mRotation.y, mRotation.z));

    // rotate separately around each axis
    if (mRotation.x != 0)   rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    if (mRotation.y != 0)   rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    if (mRotation.z != 0)   rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    translationMatrix = glm::translate(translationMatrix, mPosition);

    return translationMatrix * rotationMatrix * scalingMatrix;
}

/*
* Adds xyzDegrees to the current rotation of the object. In other words:
* the object gets rotated further. For accumulating rotations.
*/
void ruya::Object::rotate(float xDegrees, float yDegrees, float zDegrees)
{
    mRotation.x = fmod(mRotation.x + xDegrees, 360);
    mRotation.y = fmod(mRotation.y + yDegrees, 360);
    mRotation.z = fmod(mRotation.z + zDegrees, 360);
}

/*
* Adds a child to this object
*   - an object can have only one parent
*   - if a child object gets assigned to a parent obj while already 
*       being the child of another, then the child removes itself from 
*       the old parent and gets added to the new one.
*/
void ruya::Object::add_child(Object* obj)
{
    mChildren.push_back(obj);
}

bool ruya::Object::remove_child(Object& obj)
{
    for (auto ichild = mChildren.begin(); ichild != mChildren.end(); ichild++)
    {
        Object& child = **ichild;

        if (child.mUUID == obj.mUUID)
        {
            mChildren.erase(ichild);
            return true;
        }
    }

    return false;
}

bool ruya::Object::operator==(const Object& other)
{
    return this->mUUID == other.mUUID;
}

/*
* Removes self from parent, then sets parent ptr to null
*/
void ruya::Object::dislodge_from_parent()
{
    if (mParent != nullptr)
    {
        mParent->remove_child(*this);
        mParent = nullptr;
    }
}

