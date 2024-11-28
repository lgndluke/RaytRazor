#include "Camera_Component.h"

Camera_Component::Camera_Component(const boost::uuids::uuid uuid,
                                   const string& name,
                                   const optional<glm::vec3> position,
                                   const optional<glm::vec3> rotation,
                                   const optional<glm::vec3> scale,
                                   const float fov,
                                   const float aspect_ratio,
                                   const float near_clip,
                                   const float far_clip)
                                   : Base_Component(uuid, name, position, rotation, scale)
{
    this->fov = fov;
    this->aspect_ratio = aspect_ratio;
    this->near_clip = near_clip;
    this->far_clip = far_clip;
}

float Camera_Component::get_fov() const
{
    return this->fov;
}

void Camera_Component::set_fov(const float new_fov)
{
    this->fov = new_fov;
}

float Camera_Component::get_aspect_ratio() const
{
    return this->aspect_ratio;
}

void Camera_Component::set_aspect_ratio(const float new_aspect_ratio)
{
    this->aspect_ratio = new_aspect_ratio;
}

float Camera_Component::get_near_clip() const
{
    return this->near_clip;
}

void Camera_Component::set_near_clip(const float new_near_clip)
{
    this->near_clip = new_near_clip;
}

float Camera_Component::get_far_clip() const
{
    return this->far_clip;
}

void Camera_Component::set_far_clip(const float new_far_clip)
{
    this->far_clip = new_far_clip;
}


glm::mat4 Camera_Component::getViewMatrix() const
{
    const glm::vec3 position = this->get_position();
    const glm::vec3 rotation = this->get_rotation();

    const float pitch = glm::radians(rotation.x);
    const float yaw   = glm::radians(rotation.y);

    glm::vec3 direction;
    direction.x = cos(pitch) * cos(yaw);
    direction.y = sin(pitch);
    direction.z = cos(pitch) * sin(yaw);
    direction = glm::normalize(direction);

    const glm::vec3 target = position + direction;
    constexpr auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(position, target, up);
}

glm::mat4 Camera_Component::getProjectionMatrix() const
{
    return glm::perspective(this->fov, this->aspect_ratio, this->near_clip, this->far_clip);
}
