#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>

ruya::Object::Object()
{
}

ruya::Object::~Object()
{
}

/*
* Get the model matrix that defines the transformation (scaling, rotation 
* and translation) of this object.
*/
glm::mat4 ruya::Object::getModelMatrix()
{
    glm::mat4 scalingMatrix(1.0f);
    glm::mat4 rotationMatrix(1.0f);
    glm::mat4 translationMatrix(1.0f);

    scalingMatrix = glm::scale(scalingMatrix, mScale);

    if (mRotation.x != 0 || mRotation.y != 0 || mRotation.z != 0)
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(mRotation.w), glm::vec3(mRotation.x, mRotation.y, mRotation.z));

    translationMatrix = glm::translate(translationMatrix, mPosition);

    return translationMatrix * rotationMatrix * scalingMatrix;
}

