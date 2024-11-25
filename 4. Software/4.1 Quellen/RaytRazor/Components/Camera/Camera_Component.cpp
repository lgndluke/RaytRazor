#include "Camera_Component.h"

Camera_Component::Camera_Component(glm::vec3 position, glm::vec3 front, glm::vec3 up,
                                   float fov, float near, float far)
    : position(position), front(front), up(up), fov(fov), nearClip(near), farClip(far)
{
}

glm::mat4 Camera_Component::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera_Component::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera_Component::updateCameraPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

void Camera_Component::updateCameraOrientation(glm::vec3 newFront, glm::vec3 newUp)
{
    front = newFront;
    up = newUp;
}

glm::vec3 Camera_Component::getPosition() const
{
    return position;
}
