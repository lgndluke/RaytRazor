#include "Light_Component.h"

Light_Component::Light_Component(const boost::uuids::uuid uuid,
                                 const string& name,
                                 const optional<glm::vec3> position,
                                 const optional<glm::vec3> rotation,
                                 const optional<glm::vec3> scale,
                                 const float intensity,
                                 const glm::vec3 color)
                                 : Base_Component(uuid, LIGHT, name, position, rotation, scale)
{
    this->intensity = intensity;
    this->color = color;
}

float Light_Component::get_intensity() const
{
    return this->intensity;
}

void Light_Component::set_intensity(const float new_intensity)
{
    this->intensity = new_intensity;
}

glm::vec3 Light_Component::get_color() const
{
    return this->color;
}

void Light_Component::set_color(const glm::vec3 new_color)
{
    this->color = new_color;
}
