#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>


ruya::Object::Object()
    : mPosition(0.0f), mRotation(0.0f), mScale(1.0f), mColor(0.0f), mMesh(nullptr), mParent(nullptr)
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

    if (mRotation.x != 0 || mRotation.y != 0 || mRotation.z != 0)
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.w), glm::vec3(mRotation.x, mRotation.y, mRotation.z));

    translationMatrix = glm::translate(translationMatrix, mPosition);

    return translationMatrix * rotationMatrix * scalingMatrix;
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

